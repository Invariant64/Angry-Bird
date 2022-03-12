#pragma once
#include <cmath>

struct Point {
	float x, y;
	Point() {};
	Point(float x1, float y1) {
		x = x1, y = y1;
	}
};

enum GroundTouch {
	UNTOUCH = 0, _UP, _DOWN, _LEFT, _RIGHT
};

enum Direction {
	INSIDE = 0, UP, DOWN, LEFT, RIGHT
};
