#pragma once
#include "Object.h"
#include "tools.h"
class Ground :  //����ƽ̨
    public Object
{
public:
    Ground() {};
    Ground(
        float dot0_x, float dot0_y,
        float dot1_x, float dot1_y,
        float dot2_x, float dot2_y,
        float dot3_x, float dot3_y);
    //int touchType(float x, float y);
    Point p[4];  //�����ĸ�������

    void draw();

private:
    
    //float k[4];  //����������б��
    float len1, len2;  //������
};


