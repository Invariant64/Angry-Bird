#include "Object.h"
#include <graphics.h>
#include <cstring>

//const int birdSize = 50;

Object::Object() {}

Object::Object(bool mvb) {
	moveable = mvb;
	//loadimage(&img, pImgFile, birdSize, birdSize, true);
}

//void Object::drawObj(int x, int y) {
//	putimage(x, y, &img);
//}