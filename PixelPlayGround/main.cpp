#define OLC_PGE_APPLICATION
#include "olcPGE.h"
#include "string"
#include <vector>
#include "conio.h"


using namespace std;


class MyShape {

public:
	MyShape(int x, int y, olc::PixelGameEngine& ptrPge) : x{ x }, y{ y }, ptrPge{ &ptrPge } {}
	virtual void draw() = 0;

protected:
	int x;
	int y;
	int size = 6;
	olc::PixelGameEngine* ptrPge;
};


class Circle : public MyShape {

public:
	Circle(int x, int y, olc::PixelGameEngine& ptrPge) : MyShape(x, y, ptrPge) {}

	void draw() override { ptrPge->FillCircle(x, y, size, olc::RED); }
	void follow() {x = ptrPge->GetMouseX() - 10; y = ptrPge->GetMouseY() - 10; }

	void modSize(int s) { size += s; }

};


class Demo : public olc::PixelGameEngine
{

// DEMO Constructor
public:
	Demo() : c(ScreenWidth() / 2, ScreenHeight() / 2, *this)
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

		if (GetMouse(0).bPressed)
			followMouse = !followMouse;

		if (followMouse)
			switch (GetMouseWheel()) {

			case 120:
				c.modSize(+1);
				break;

			case -120:
				c.modSize(-1);
				break;
			
			default:
				break;
			}


		if (followMouse)
			c.follow();	

		c.draw();

		mouseSnoop = GetMouseWheel();

		if (mouseSnoop != temp)
			cout << mouseSnoop << endl;

		temp = mouseSnoop;

		return true;
	}



// DEMO Data Members
private:

	int mouseSnoop = 0;
	int temp = -55;
	Circle c;
	bool followMouse = false;
};







int main() {

	
	Demo demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
		

	//_getch();
	return 0;
}