#define OLC_PGE_APPLICATION
#include "olcPGE.h"
#include "string"
#include <vector>


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
	

		return true;
	}



// DEMO Data Members
private:


};


int main() {


		Demo demo;
		if (demo.Construct(256, 240, 4, 4))
			demo.Start();
	

	return 0;
}