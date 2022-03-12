#include "Ground.h"
#include <iostream>

Ground::Ground(
    float dot0_x, float dot0_y,
    float dot1_x, float dot1_y,
    float dot2_x, float dot2_y,
    float dot3_x, float dot3_y) {

    p[0] = Point(dot0_x, dot0_y);
    p[1] = Point(dot1_x, dot1_y);
    p[2] = Point(dot2_x, dot2_y);
    p[3] = Point(dot3_x, dot3_y);  //赋值需要由左上角起顺时针赋值

    len1 = p[1].x - p[0].x;
    len2 = p[0].y - p[3].y;
}
/*
int Ground::touchType(float x, float y) {
    if (x < p[0].x && y > p[0].y)
        return 1;
    if (x > p[0].x && x <= p[1].x && y > p[0].y)
        return 2;
    if (x > p[1].x && y > p[1].y)
        return 3;
    if (x <= p[0].x && y > p[3].y && y <= p[0].y)
        return 4;
    if (x > p[0].x && x <= p[1].x && y > p[3].y)
        return 5;
    if(x > p[1].x && y > p[3].y && )
}
*/

void Ground::draw() {
    //std::cout << p[0].x << ' ' << p[0].y << std::endl;
    fillrectangle(p[0].x, p[0].y, p[3].x, p[3].y);
}
