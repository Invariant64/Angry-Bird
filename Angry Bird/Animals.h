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
    float x, y;  //坐标
    float v_x, v_y;  //速度设定为向上、向右为正方向
    int r;  //半径（假设都是球）
    void speedChange();
    void AnmKnock(Animals* anm);  //与动物碰撞
    //void Knock(Ground grd);  //与台面碰撞

    bool isTouchAnm(Animals* anm);
    bool isTouchGrd(Ground* grd);
    Ground* nearGrd;

    bool animalTouch;  //是否与其它动物接触
    bool isRolling;

    void draw();
    void move();

    int key;

    //int imgSize_X, imgSize_Y;

private:
    
    int blood;  //血量
    
    GroundTouch groundTouch;  //与地面接触的类型
    
};




