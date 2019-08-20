#pragma once
#include "olcPGE.h"
#include "Vec2.h"


using namespace std;
// a planet is a filled circle
// if there is another planet that is smaller, it will get attracked by the bigger planet
// and it will attach/become a a satellite of that planet
// and orbit around it


// Maybe add a feature where you stop all planets, and allign them from top outer rim to center, that all planets are on their most south side
	// one way to implement, is to let it rotate, and always check if current Y position is equal to the toppest point with respect to screen, if so, so top internal Ticker
	// recurrsive stop = give order to stable planet to change state of longestChain Planet to stop at TOP
			// when it stops, gives further order down to stop at their bigger planet most BOTTOM rim until the last planet
				// maybe a check to see if we're stable, if so it means the next planet needs to go to top - if we're obiting, we need to stop at bottom
			// Bonus - see if there is an easy way to elegantly reduce speed when approaching to a stop


// Possible feature: after determining the GravField size, make a gradual expansion until it hits its established limit.
	// it can be as smooth as attract() 

// add a global resize function for all planets and place Stable planet in center (maybe by mouse scroll resize)


//IMPORTANT! when we calculate a GravField, it will be 2 times our radius.
// Bug, if deploying planets in alternative modes program crashes : currently it's better to stick to standard-auto mode.

class Planet {
public:
	Planet(olc::PixelGameEngine& pge, int radius, Vec2 position, olc::Pixel color)
		:
		pge		{pge}, 
		radius	{radius}, 
		position{ position }, 
		color	{ color}, 
		state	{STABLE}, 
		deployed{false}, 
		fTicker {1.0f},
		gravFieldTotalPointsOfBigger {0},
		speed {20.0f}, //speed {float(10 + rand() % 20)}
		gravFieldStrenght { float(radius) }
	{}

public:

	void draw();
	void update(float fElapsedTime);
	void modifySize(int mod);
	void followMouse();
	bool isDeployed() const;
	void deploy();	
	void showGrav();
	enum State {STABLE, PULLED, ORBITING};
	void modFieldStr(int mod);
	int getRadius() const;
	int getDiameter() const;
	bool isStable() const;
	void recalculateGravFieldStrength();

	void attachPlanet(Planet& plnt);

	//debug
	void displayRadius();

private:
	void createGravPoints();	//needs to have GravFieldStr already calculated
	void sortGravPoints();
	void updateGravPointsByVelDelta();
	bool planetInGravField(const Planet& plnt) const;

	void interactWithPlanets(float fElapsedTime);

	int closestGravPointTo(Planet& plnt);
	void attract(Planet& plnt, Vec2 here, float fElapsedTime);
	void move(Planet& plnt, Vec2& here);
	void broadcastGravSzToPlanets();
	float getTick() const;
	void setGravField(int val);

	//recur
	int getGravFieldNeedsOfSmallerPlanets(vector<Planet*>& vPlnts, int radius, int recurStep);
	int getAttachedPlanetsCount(vector<Planet*>& vPlnts);
	int getDeepestChainPlanetCount(vector<Planet*>& vPlnts, int level);

	

private:
	int radius;
	int diameter;
	Vec2 position;
	Vec2 deltaPos;
	olc::Pixel color;
	olc::PixelGameEngine& pge;
	State state;
	vector<Planet*>vOrbitingPlanets;
	vector<Vec2>vGravPoints;
	int gravFieldTotalPointsOfBigger;
	float gravFieldStrenght;
	float fTicker;
	float speed;

	bool deployed;
};