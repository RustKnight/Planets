#include "Planet.h"
//#include <algorithm>

void Planet::draw()
{
	pge.FillCircle(position.x, position.y, radius, color);
	showGrav();
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
	plnt.position += (here - plnt.position).GetNormalized() * fElapsedTime * (position - plnt.position).GetLength() * 0.5f;
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
// why is planetInGravField() considering planet is inside Gravfield if we expand it ? false representation of gravField?
// this is why planets get stuck if we expand with planetInGravField outside the switch case: 
		// they constantly pass the test that they are in field and get allocated to the closest GravField point


void Planet::updateGravPoints()
{
	for (Vec2& point : vGravPoints)
		point += deltaPos;
}

bool Planet::planetInGravField(const Planet& plnt) const
{
	// if the distance between small and big is less than the radius of the grav field of puller, return true
	
	return (position - plnt.position).GetLengthSq() < (Vec2{ 0, GravFieldStrenght + radius}).GetLengthSq();

	
}




void Planet::move(Planet& plnt, Vec2& here)
{
	// move should actually make the planet move from its position to the Vec2 position


	plnt.deltaPos = here - plnt.position;	//calculate next position delta
	plnt.position = here;					//update position

	plnt.updateGravPoints();				//update all gravPoints according to delta


	//plnt.storeGravPoints();				//horrible way of "updating" gravPoints
}

void Planet::broadcastGravSzToPlanets()
{
	if (!vOrbitingPlanets.empty())
		for (Planet* plnt : vOrbitingPlanets) {
			plnt->GravFieldSize = vGravPoints.size();
			plnt->fTicker = closestGravPointTo(*plnt);	// if we change the GravFieldSize we also need to me sure the current Ticker of each small planet is not stuck with its old ticker value, which could be bigger than the entire newGravField size
		}
}

float Planet::getTick() const
{
	return fTicker;
}

void Planet::update(float fElapsedTime)
{
	fTicker += fElapsedTime * speed;

	if (fTicker >= GravFieldSize)
		fTicker = 0.0f;

	interactWithPlanets(fElapsedTime);

	//string s = to_string(fTicker);
	//pge.DrawString(5, 5, s, olc::WHITE, 1);
}

void Planet::modifySize(int mod)
{
	radius += mod;
	GravFieldStrenght = radius * 1.5f;
}

bool Planet::isDeployed() const
{
	return deployed;
}

void Planet::deploy()
{
	deployed = true;
	storeGravPoints();
}

void Planet::storeGravPoints()
{
	if (!vGravPoints.empty())		// if for some reason we need to recalculate the whole thing -> delete all and make a clean start
		vGravPoints.clear();


	int x0 = 0;
	int y0 = radius + GravFieldStrenght;
	int d = 3 - 2 * (radius + GravFieldStrenght);
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
	broadcastGravSzToPlanets();
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
	
		int x0 = 0;
		int y0 = radius + GravFieldStrenght;
		int d = 3 - 2 * (radius + GravFieldStrenght);
		if (!radius) return;

		while (y0 >= x0) // only formulate 1/8 of circle
		{
			pge.Draw(position.x + x0, position.y - y0, olc::RED);
			pge.Draw(position.x + y0, position.y - x0, olc::RED);
			pge.Draw(position.x + y0, position.y + x0, olc::RED);
			pge.Draw(position.x + x0, position.y + y0, olc::RED);
			pge.Draw(position.x - x0, position.y + y0, olc::RED);
			pge.Draw(position.x - y0, position.y + x0, olc::RED);
			pge.Draw(position.x - y0, position.y - x0, olc::RED);
			pge.Draw(position.x - x0, position.y - y0, olc::RED);
			if (d < 0) d += 4 * x0++ + 6;
			else d += 4 * (x0++ - y0--) + 10;
		}
	
}

void Planet::modFieldStr(int mod)
{
	GravFieldStrenght += mod;
	storeGravPoints();
}

void Planet::attachPlanet(Planet& plnt)
{
	vOrbitingPlanets.push_back(&plnt);
	broadcastGravSzToPlanets();
}

void Planet::followMouse()
{
	position = Vec2{ float(pge.GetMouseX() - 10), float(pge.GetMouseY() - 10) };
}
