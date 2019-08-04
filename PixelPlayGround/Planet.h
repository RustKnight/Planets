#pragma once
#include "olcPGE.h"
#include "Vec2.h"


using namespace std;
// a planet is a filled circle
// if there is another planet that is smaller, it will get attracked by the bigger planet
// and it will attach/become a a satellite of that planet
// and orbit around it




class Planet {

	Planet(olc::PixelGameEngine& pge, int radius, Vec2& position, olc::Pixel color)
		:
		pge{pge}, radius {radius}, position { position }, color { color}
	{}





private:
	int radius;
	Vec2 position;
	olc::Pixel color;
	olc::PixelGameEngine& pge;
};