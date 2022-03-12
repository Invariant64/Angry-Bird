#pragma once
#include "Object.h"
#include "Ground.h"

const float REDUCE_K = 0.8f;

class Animals :
    public Object
{
public:
    Animals() {};
    Animals(float vx, float vy, float w1, int r1, int bld, float x1, float y1);
    /*
    float vx() { return v_x; };
    float vy() { return v_y; };
    float w1() { return w; };
    float r1() { return r; };
    */
    float w;
    float x, y;  //����
    float v_x, v_y;  //�ٶ��趨Ϊ���ϡ�����Ϊ������
    int r;  //�뾶�����趼����
    void speedChange();
    void AnmKnock(Animals* anm);  //�붯����ײ
    //void Knock(Ground grd);  //��̨����ײ

    bool isTouchAnm(Animals* anm);
    bool isTouchGrd(Ground* grd);
    Ground* nearGrd;

    bool animalTouch;  //�Ƿ�����������Ӵ�
    bool isRolling;

    void draw();
    void move();

    int key;

    //int imgSize_X, imgSize_Y;

private:
    
    int blood;  //Ѫ��
    
    GroundTouch groundTouch;  //�����Ӵ�������
    
};




