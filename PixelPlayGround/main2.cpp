#define OLC_PGE_APPLICATION
#include "olcPGE.h"
#include "string"
#include <vector>
#include "conio.h"

#include "Planet.h"


using namespace std;



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

		checkMouse();



		
		for (Planet& plnt : vTotalPlanets)
			plnt.draw();
	
		return true;
	}

	// DEMO Function Members
	void createPlanet();
	void checkMouse();
	bool allPlanetsDeployed();
	Planet& getUndeployedPlanet();


	// DEMO Data Members
private:
	vector<Planet> vTotalPlanets;
};



























int main() {


	Demo demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();


	//_getch();
	return 0;
}

void Demo::createPlanet()
{
	
	vTotalPlanets.push_back(Planet{ *this, 10, Vec2{ float(GetMouseX() - 10), float(GetMouseY() - 10) }, olc::Pixel {uint8_t (rand() % 255), uint8_t(rand() % 255), uint8_t(rand() % 255) } });


}

void Demo::checkMouse()
{

	// no planet ON MOUSE and we just pressed RIGHT CLICK 
	if (GetMouse(1).bPressed && allPlanetsDeployed()) {
		createPlanet();
	}

	// Planet ON MOUSE and is ready for DEPLOY
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

		if (GetMouse(1).bPressed)
			plnt.deploy();
	}
}

bool Demo::allPlanetsDeployed()
{
	for (Planet& plnt : vTotalPlanets)
		if (!plnt.isDeployed())
			return false;

	return true;
}

Planet& Demo::getUndeployedPlanet()
{
	for (Planet& plnt : vTotalPlanets)
		if (!plnt.isDeployed())
			return plnt;

	//WARNING: RETURN CONTROL PATH SHOULD BE FULLY IMPLEMENTED
}
