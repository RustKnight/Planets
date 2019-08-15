#pragma once
#include "olcPGE.h"
#include "Vec2.h"


using namespace std;
// a planet is a filled circle
// if there is another planet that is smaller, it will get attracked by the bigger planet
// and it will attach/become a a satellite of that planet
// and orbit around it


// BUG FIX

// when readjusting undeployed planet size, it's grav field does not adjust its size as it should
	// if starting with different planet size, planetInGravField() does not work properly



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
		speed {20.0f},
		GravFieldStrenght { (float)radius * 1.5f}
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

	void attachPlanet(Planet& plnt);

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
	
private:
	int radius;
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