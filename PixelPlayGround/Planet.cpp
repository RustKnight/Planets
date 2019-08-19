#include "Planet.h"

void Planet::draw()
{
	pge.FillCircle(position.x, position.y, radius, color);
}


int Planet::closestGravPointTo(Planet& plnt)
{
	float closest = vGravPoints[0].GetLengthSq();
	float testCase;
	int result = 0;


	for (int i = 0; i < vGravPoints.size(); i++)
	{
		testCase = (vGravPoints[i] - plnt.position).GetLengthSq();

		if (testCase < closest) {
			closest = testCase;
			result = i;
		}
	}


	return result;
}

void Planet::attract(Planet& plnt, Vec2 here, float fElapsedTime)
{
	Vec2 moveHere = (here - plnt.position).GetNormalized() * fElapsedTime * (position - plnt.position).GetLength() * 0.5f;
	plnt.position += moveHere;

	plnt.deltaPos = moveHere;	//calculate next position delta
	plnt.updateGravPointsByVelDelta();				//update all gravPoints according to delta
}


void Planet::update(float fElapsedTime)
{
	fTicker += fElapsedTime * speed;

	if (fTicker >= gravFieldTotalPointsOfBigger)
		fTicker = 0.0f;

	interactWithPlanets(fElapsedTime);

}


void Planet::interactWithPlanets(float fElapsedTime)
{

	if (!vOrbitingPlanets.empty()) 

		for (Planet* plnt : vOrbitingPlanets) {

			switch (plnt->state) {


			case STABLE:		
				plnt->state = PULLED;
				break;


			case PULLED:
				if (planetInGravField(*plnt)) {
					plnt->fTicker = closestGravPointTo(*plnt);
					plnt->createGravPoints();
					broadcastGravSzToPlanets();
					plnt->state = ORBITING;
					break;
				}		
				attract(*plnt, position, fElapsedTime);	//pull
				break;


			case ORBITING:
				move(*plnt, vGravPoints[plnt->getTick()]);
				break;
			}

		}
	
}



void Planet::updateGravPointsByVelDelta()
{
	for (Vec2& point : vGravPoints)
		point += deltaPos;
	
}

bool Planet::planetInGravField(const Planet& plnt) const
{
	// if the distance between small and big is less than the radius of the grav field of puller, return true
	
	return (position - plnt.position).GetLengthSq() < (Vec2{ 0, gravFieldStrenght}).GetLengthSq();

	
}




void Planet::move(Planet& plnt, Vec2& here)
{
	// move should actually make the planet move from its position to the Vec2 position


	plnt.deltaPos = here - plnt.position;	//calculate next position delta
	plnt.updateGravPointsByVelDelta();				//update all gravPoints according to delta

	plnt.position = here;					//update position

}

void Planet::broadcastGravSzToPlanets()
{
	if (!vOrbitingPlanets.empty())
		for (Planet* plnt : vOrbitingPlanets) {
			plnt->gravFieldTotalPointsOfBigger = vGravPoints.size();
			plnt->fTicker = closestGravPointTo(*plnt);	// if we change the gravFieldTotalPointsOfBigger we also need to me sure the current Ticker of each small planet is not stuck with its old ticker value, which could be bigger than the entire newGravField size
		}
}

float Planet::getTick() const
{
	return fTicker;
}





bool Planet::isDeployed() const
{
	return deployed;
}

void Planet::deploy()
{
	deployed = true;
	createGravPoints();
}

void Planet::createGravPoints()
{
	if (!vGravPoints.empty())		// if for some reason we need to recalculate the whole thing -> delete all and make a clean start
		vGravPoints.clear();


	int x0 = 0;
	int y0 = gravFieldStrenght;
	int d = 3 - 2 * gravFieldStrenght;
	if (!radius) return;

	while (y0 >= x0) // only formulate 1/8 of circle
	{
		vGravPoints.push_back (Vec2 (position.x + x0, position.y - y0));
		vGravPoints.push_back (Vec2 (position.x + y0, position.y - x0));
		vGravPoints.push_back (Vec2 (position.x + y0, position.y + x0));
		vGravPoints.push_back (Vec2 (position.x + x0, position.y + y0));
		vGravPoints.push_back (Vec2 (position.x - x0, position.y + y0));
		vGravPoints.push_back (Vec2 (position.x - y0, position.y + x0));
		vGravPoints.push_back (Vec2 (position.x - y0, position.y - x0));
		vGravPoints.push_back (Vec2 (position.x - x0, position.y - y0));

		if (d < 0) d += 4 * x0++ + 6;
		else d += 4 * (x0++ - y0--) + 10;
	}

	sortGravPoints();
}

void Planet::sortGravPoints()
{
	
	// sort to find middle of Y first
	sort(vGravPoints.begin(), vGravPoints.end(), [](Vec2& a, Vec2& b) {
		return a.y < b.y;
	});
	const int Ymiddle = vGravPoints[vGravPoints.size() - 1].y - ( (vGravPoints[vGravPoints.size() - 1].y - vGravPoints[0].y) / 2);
	

	// sort all Vec2 according to their X from LEFT to RIGHT
	sort(vGravPoints.begin(), vGravPoints.end(), [](Vec2& a, Vec2& b) {
		return a.x < b.x;
	});
	const int Xmiddle = vGravPoints[vGravPoints.size() - 1].x - ((vGravPoints[vGravPoints.size() - 1].x - vGravPoints[0].x) / 2);


	// sort top half of coords
	sort(vGravPoints.begin(), vGravPoints.end(), [Xmiddle](Vec2& a, Vec2& b) {
		
		if ((int)a.x == (int)b.x)
			if (a.x < Xmiddle)
				return a.y > b.y;
			else
				return a.y < b.y;

		return a.x < b.x;
	});

	


	vector<Vec2> orderedCoords;

	// get the first half of circel into NEW vec2
	for (auto i = vGravPoints.begin(), e = vGravPoints.end(); i != e; ++i)
	{
		if (i->y <= Ymiddle) 
			orderedCoords.push_back(*i);
	}


	// sort X axis in reverse order
	sort(vGravPoints.begin(), vGravPoints.end(), [](Vec2& a, Vec2& b) {
		return a.x > b.x;
	});
	// deal with equal X points
	sort(vGravPoints.begin(), vGravPoints.end(), [Xmiddle](Vec2& a, Vec2& b) {

		if ((int)a.x == (int)b.x)
			if (a.x < Xmiddle)
				return a.y > b.y;
			else
				return a.y < b.y;

		return a.x > b.x;
	});


	// add lower half of circel
	for (auto i = vGravPoints.begin(), e = vGravPoints.end(); i != e; ++i)
	{
		if (i->y > Ymiddle)
			orderedCoords.push_back(*i);
	}


	
	vGravPoints = orderedCoords;

}






void Planet::showGrav()
{

	for (Vec2& p : vGravPoints)
		pge.Draw(p.x, p.y, olc::RED);

}

void Planet::setGravField(int val)
{
	gravFieldStrenght = val;
}






void Planet::displayRadius()
{
	pge.DrawString(position.x - radius / 2, position.y - radius * 2.0f, to_string(radius), olc::RED, 1);
}



void Planet::modifySize(int mod)
{
	radius += mod;
	gravFieldStrenght = getRadius();
	createGravPoints();
}

void Planet::modFieldStr(int mod)
{
	gravFieldStrenght += mod;
	createGravPoints();
	broadcastGravSzToPlanets();
}

int Planet::getRadius() const
{
	return radius;
}

int Planet::getDiameter() const
{
	return radius * 2;
}

bool Planet::isStable() const
{
	return state == STABLE;
}


// When planet with plantes is joining, make sure we extend GravField of attracter only by the biggest chained planets value
// SPEED tweak of orbiting planets in relation with chain, might fix unesthetic looking rotations

int Planet::getAttachedPlanetsCount(vector<Planet*>& vPlnts)
{
	if (vPlnts.empty())
		return 1;

	int holder = 0;

	for (Planet* plnt : vPlnts)
		holder += getAttachedPlanetsCount(plnt->vOrbitingPlanets);

	return holder + 1;
}

int Planet::getDeepestChainPlanetCount(vector<Planet*>& vPlnts, int level)
{
	if (vPlnts.empty())
		return level;

		vector<int> holder;

	for (Planet* plnt : vPlnts)	 
			holder.push_back(getDeepestChainPlanetCount(plnt->vOrbitingPlanets, level + 1));
			
	// return biggest value from holder
	vector<int>::iterator result;
	result = max_element(holder.begin(), holder.end());

	return *result;
}


int Planet::mysteriousCalculation(vector<Planet*>& vPlnts, int radius, int recurStep)
{
	// enter with 1 and increase recurStep by multiple of 2

	if (vPlnts.empty()) 
			return radius * recurStep / 2;

	vector<int> holder;

	for (Planet* plnt : vPlnts)
		holder.push_back (mysteriousCalculation(plnt->vOrbitingPlanets, plnt->radius, recurStep * 2) );

	vector<int>::iterator result;
	result = max_element(holder.begin(), holder.end());

	return *result + radius * recurStep / 2;
}


void Planet::attachPlanet(Planet& plnt)
{

	int incomingPlanetGravFieldSpaceNeeds = plnt.radius + plnt.mysteriousCalculation(plnt.vOrbitingPlanets, 0, 1);
	int currentGravFieldAccomodation = mysteriousCalculation(vOrbitingPlanets, 0, 1);	
	
	vOrbitingPlanets.push_back(&plnt);


	if (currentGravFieldAccomodation < incomingPlanetGravFieldSpaceNeeds) {
		setGravField(radius + mysteriousCalculation(vOrbitingPlanets, 0, 1));
		createGravPoints();
	}

	broadcastGravSzToPlanets();		// communicate how many points of GravField we currently have to the newly attached planets so they can cycle
}

void Planet::recalculateGravFieldStrength()
{
	setGravField(radius + mysteriousCalculation(vOrbitingPlanets, 0, 1));
	createGravPoints();
	broadcastGravSzToPlanets();
}


void Planet::followMouse()
{
	position = Vec2{ float(pge.GetMouseX() - 10), float(pge.GetMouseY() - 10) };
}
