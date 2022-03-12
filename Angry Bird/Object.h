#pragma once
#include "tools.h"
#include <graphics.h>

const float a_G = -1.0f;
const int birdSize = 20;
const int ImgSize = 25;

class Object
{
public:
	Object();
	Object(bool mvb);

	//void drawObj(int x, int y);
	IMAGE img;
private:
	bool moveable;  //ÊÇ·ñ¿ÉÒÆ¶¯

	//int wideth, highth;

};







