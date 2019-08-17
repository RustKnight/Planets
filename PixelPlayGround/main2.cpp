#define OLC_PGE_APPLICATION
#include "olcPGE.h"
#include "string"
#include <vector>
#include "conio.h"

#include "Planet.h"


using namespace std;

// Do not take refrences or pointers to Vector Elements -> these elements are prone to change their memeory address, leaving you with an invalid pointer

class Demo : public olc::PixelGameEngine
{

	// DEMO Constructor
public:
	Demo()
	{
		sAppName = "Example";
	}



public:


	bool OnUserCreate() override
	{

		

		return true;
	}



	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);

		//Debug
			
			//Control Index display
			DrawString(5, 5, to_string(controlIndex1), olc::YELLOW, 1);
			DrawString(20, 5, to_string(controlIndex2), olc::RED, 1);

			// G key to Show/Hide Grav Field
			if (GetKey(olc::G).bPressed)
				showGravity = !showGravity;

			// Display small square to show which Attach-Mode we're in
			if (attachToBiggest)
				FillRect(5, 20, 5, 5, olc::GREY);
			else
				FillRect(5, 20, 5, 5, olc::YELLOW);

			// Change attach-mode rules;
			if (GetKey(olc::SPACE).bPressed)
				attachToBiggest = !attachToBiggest;

			// Displays gravitity Field
			if (showGravity)
				for (Planet* plnt : vTotalPlanets)
					plnt->showGrav();


		//	Selects which planet we want to manipulate
			// N- M+ for controlIndex1 selection
			// J- H+ for controlIndex2 selection
		if (GetKey(olc::M).bPressed)
			controlIndex1++;
		if (GetKey(olc::N).bPressed)
			controlIndex1--;		
		if (GetKey(olc::J).bPressed)
			controlIndex2++;
		if (GetKey(olc::H).bPressed)
			controlIndex2--;


		// L key to add the GravField value of Planet[ControlIndex2] to GravField of Planet[ControlIndex1]
		if (GetKey(olc::L).bPressed)
			vTotalPlanets[controlIndex1]->modFieldStr(vTotalPlanets[controlIndex2]->getRadius());


		// O- P+ to increase or decrease GravField of selected controlIndex1 Planet
		if (GetKey(olc::O).bPressed)
			vTotalPlanets[controlIndex1]->modFieldStr(-5);
		if (GetKey(olc::P).bPressed)
			vTotalPlanets[controlIndex1]->modFieldStr(5);


		// Draw gravField first, update plantes, draw planets
		for (int i = 0; i < vTotalPlanets.size(); i++)
		{
			vTotalPlanets[i]->update(fElapsedTime);
			vTotalPlanets[i]->draw();
			vTotalPlanets[i]->displayRadius();			
		}


		// Control creation process of Planets and attach rules
		checkMouse();


		return true;
	}

	// DEMO Function Members
	void createPlanet();
	void checkMouse();
	bool allPlanetsDeployed();
	Planet& getUndeployedPlanet();
	bool isSmaller();
	Planet& getBiggest();
	Planet& getPreviousBiggest();

	// DEMO Data Members
private:
	vector<Planet*> vTotalPlanets;
	int controlIndex1 = 0;
	int controlIndex2 = 0;
	bool showGravity = true;
	bool attachToBiggest = true;
};
















int main() {


	Demo demo;
	if (demo.Construct(400, 400, 4, 4))
		demo.Start();


	//_getch();
	return 0;
}

void Demo::createPlanet()
{
	vTotalPlanets.push_back(new Planet{ *this, 10, Vec2{ float(GetMouseX() - 10), float(GetMouseY() - 10) }, olc::Pixel {uint8_t (rand() % 255), uint8_t(rand() % 255), uint8_t(rand() % 255) } });

}

void Demo::checkMouse()
{

	// No planet ON MOUSE and we just pressed RIGHT CLICK 
	if (GetMouse(1).bPressed && allPlanetsDeployed()) 
		createPlanet();
	

	// Planet ON MOUSE and is ready for DEPLOY
	// In this phase we can resize the planet and move it around
	else if (!allPlanetsDeployed()) {

		Planet& plnt = getUndeployedPlanet();
	
		switch (GetMouseWheel()) {

			case 120:
				plnt.modifySize(+1);
				break;
			case -120:
				plnt.modifySize(-1);
				break;
			default:
				break;
		}

		plnt.followMouse();
	
		if (GetMouse(1).bPressed) {		// deploy planet and attach to first created planet, if we have more than one planet deployed...

			plnt.deploy();

			if (vTotalPlanets.size() > 1) {			

				Planet& last_created_planet = *vTotalPlanets[vTotalPlanets.size() - 1];

				if (attachToBiggest) {
					
					// if newly created planet is smaller than current planets, go to biggest
					if (isSmaller())
						getBiggest().attachPlanet(last_created_planet);
					
					// if newly created planet is bigger than all current planets, attach next smallest planet to it
					else
						last_created_planet.attachPlanet(getPreviousBiggest());					
				}
				else
					vTotalPlanets[controlIndex1]->attachPlanet(last_created_planet);
			}
		}
	}
}

bool Demo::allPlanetsDeployed()
{
	for (Planet* plnt : vTotalPlanets)
		if (!plnt->isDeployed())
			return false;

	return true;
}

Planet& Demo::getUndeployedPlanet()
{
	for (Planet* plnt : vTotalPlanets)
		if (!plnt->isDeployed())
			return *plnt;


	//WARNING: RETURN CONTROL PATH SHOULD BE FULLY IMPLEMENTED
}

bool Demo::isSmaller()
{

	Planet& testedPlanet = *vTotalPlanets[vTotalPlanets.size() - 1];
	bool result = false;

	for (int i = 0; i < vTotalPlanets.size() - 1; i++)		//test all but the last one
	{
		if (vTotalPlanets[i]->getRadius() >= testedPlanet.getRadius())
			return true;
	}

	return result;
}

Planet & Demo::getBiggest()
{

	Planet* biggest = vTotalPlanets[0];

	for (int i = 0; i < vTotalPlanets.size(); i++)		
	{
		if (vTotalPlanets[i]->getRadius() > biggest->getRadius())
			biggest = vTotalPlanets[i];
	}

	return *biggest;
}

Planet & Demo::getPreviousBiggest()
{

	Planet* biggest = vTotalPlanets[0];

	for (int i = 0; i < vTotalPlanets.size() - 1; i++)		//get second biggest
	{
		if (vTotalPlanets[i]->getRadius() > biggest->getRadius())
			biggest = vTotalPlanets[i];
	}

	return *biggest;
	
}
