#include "Planet.h"

void Planet::draw()
{
	pge.FillCircle(position.x, position.y, radius, color);
}

void Planet::update(float fElapsedTime)
{

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
	storeGravPoints();
}

void Planet::storeGravPoints()
{

	int x0 = 0;
	int y0 = radius + radius * 2.0f;
	int d = 3 - 2 * radius;
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

}

void Planet::makeThemGravitate(float fElapsedTime)
{
	if (!vGravField.empty())
		for (Planet* plnt : vGravField)
			plnt->move
}

void Planet::move(Planet& plnt, Vec2& here) const
{
	plnt.position = here;
}


void Planet::showGrav()
{
	
		int x0 = 0;
		int y0 = radius + radius * 2.0f;
		int d = 3 - 2 * radius;
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

void Planet::followMouse()
{
	position = Vec2{ float(pge.GetMouseX() - 10), float(pge.GetMouseY() - 10) };
}
