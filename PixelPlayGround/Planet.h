#pragma once
#include "olcPGE.h"
#include "Vec2.h"


using namespace std;
// a planet is a filled circle
// if there is another planet that is smaller, it will get attracked by the bigger planet
// and it will attach/become a a satellite of that planet
// and orbit around it




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
		GravFieldSize {0}

	{}

public:

	void draw();
	void update(float fElapsedTime);
	void modifySize(int mod);
	void followMouse();
	bool isDeployed() const;
	void deploy();	
	void showGrav();
	enum State {STABLE, ORBITING};

	void attachPlanet(Planet& plnt);

private:
	void storeGravPoints();
	void sortGravPoints();
	void updateGravPoints();

	void makeThemGravitate();
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
	vector<Planet*>vGravField;
	vector<Vec2>vGravPoints;
	int GravFieldSize;
	float fTicker;

	bool deployed;
};