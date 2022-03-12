#include "RedBirds.h"

//RedBirds::RedBirds(){}

RedBirds::RedBirds(float vx, float vy, float w1, float r1, int bld, float x1, float y1) : 
	Animals(vx, vy, w1, r1, bld, x1, y1)
{

	//Animals(20, 20, 20, 30, 10000, 0, 0);
	//Animals(vx, vy, w1, r1, bld, x1, y1);
	loadimage(&img, _T("./resources/birds/redbird.jpeg"), r * 2, r * 2, true);

}
