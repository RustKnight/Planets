#include "Planet.h"

void Planet::draw()
{
	pge.FillCircle(position.x, position.y, radius, color);
}

void Planet::modifySize(int mod)
{
	radius += mod;
}

bool Planet::isDeployed() const
{
	return deployed;
}

void Planet::deploy()
{
	deployed = true;
}

void Planet::followMouse()
{
	position = Vec2{ float(pge.GetMouseX() - 10), float(pge.GetMouseY() - 10) };
}
