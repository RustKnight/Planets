#pragma once
#include "olcPGE.h"
#include "Vec2.h"


using namespace std;
// a planet is a filled circle
// if there is another planet that is smaller, it will get attracked by the bigger planet
// and it will attach/become a a satellite of that planet
// and orbit around it


// Maybe add a feature where you stop all planets, and allign them from top outer rim to center, that all planets are on they most south side

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
		GravFieldSize {0},
		speed {20.0f}, //speed {float(10 + rand() % 20)}
		GravFieldStrenght { float(radius) }
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
	void recalculateGravField();

	void attachPlanet(Planet& plnt);

	//debug
	void displayRadius();

private:
	void storeGravPoints();
	void sortGravPoints();
	void updateGravPoints();
	bool planetInGravField(const Planet& plnt) const;

	void interactWithPlanets(float fElapsedTime);

	int closestGravPointTo(Planet& plnt);
	void attract(Planet& plnt, Vec2 here, float fElapsedTime);
	void move(Planet& plnt, Vec2& here);
	void broadcastGravSzToPlanets();
	float getTick() const;
	void setGravField(int val);

	//recur
	int mysteriousCalculation(vector<Planet*>& vPlnts, int radius, int recurStep);
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
	int GravFieldSize;
	float GravFieldStrenght;
	float fTicker;
	float speed;

	bool deployed;
};