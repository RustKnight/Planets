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
		pge{pge}, radius {radius}, position { position }, color { color}, state {STABLE}, deployed {false}
	{}

	void draw();
	void modifySize(int mod);
	void followMouse();
	bool isDeployed() const;
	void deploy();

	enum State {STABLE, ORBITING};




private:
	int radius;
	Vec2 position;
	olc::Pixel color;
	olc::PixelGameEngine& pge;
	State state;
	vector<Planet*>vPlanets;

	bool deployed;
};