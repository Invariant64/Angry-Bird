#include "Animals.h"
#include "Knock.h"

Animals::Animals(float vx, float vy, float w1, int r1, int bld, float x1, float y1) {
    v_x = vx, v_y = vy, w = w1;
    r = r1, blood = bld;
    x = x1, y = y1;
    //imgSize_X = ix, imgSize_Y = iy;
    animalTouch = isRolling = false;
    groundTouch = UNTOUCH;
    key = 0;
}

void Animals::speedChange() {
    
    if (!isRolling) {
        //if (v_y >= a_G)
            v_y -= a_G;
        //else
           // v_y = 0;
    }
    else {
        //v_y = 0;
        //if(nearGrd == )
        v_x *= 0.99;
    }
    
    if (abs(v_x) <= 0.05) { v_x = 0; }
    if (abs(v_y) <= 0.05) { v_y = 0; }
}

void Animals::AnmKnock(Animals* anm) {
    /*
    float v1_x = v_x, v1_y = v_y, v2_x = anm->v_x, v2_y = anm->v_y;
    float w1 = w, w2 = anm->w;
    
    v_x = ((w1 - w2) * v1_x + 2 * w2 * v2_x) / (w1 + w2);
    v_y = ((w1 - w2) * v1_y + 2 * w2 * v2_y) / (w1 + w2);
    anm->v_x = ((w2 - w1) * v2_x + 2 * w1 * v1_x) / (w1 + w2);
    anm->v_x = ((w2 - w1) * v2_y + 2 * w1 * v1_y) / (w1 + w2);

    v_x *= REDUCE_K, v_y *= REDUCE_K;
    anm->v_x *= REDUCE_K, anm->v_y *= REDUCE_K;
*/
    
    float vx = v_x, vy = v_y;
    v_x = anm->v_x, v_y = anm->v_y;
    anm->v_x = vx, anm->v_y = vy;
    
    //Sleep(1000);
}

bool Animals::isTouchAnm(Animals* anm) {
    if (sqrt((x - anm->x) * (x - anm->x) + (y - anm->y) * (y - anm->y)) <= r + anm->r) {
        animalTouch = true;
        anm->animalTouch = true;
        return true;
    }
    animalTouch = false;
    anm->animalTouch = false;
    return false;
}

bool Animals::isTouchGrd(Ground *grd) {
    //Sleep(500);
    if (grd->p[0].y - y <= r + 2 && grd->p[0].y - y >= 0 && x - r <= grd->p[1].x && x + r >= grd->p[0].x) {
        //if(abs(v_y) < 0.1)
            isRolling = true;
        nearGrd = grd;
        //y = grd->p[0].y - r;
        //Sleep(500);
        return true;
        
    }
    nearGrd = NULL;
    isRolling = false;
    return false;
}

void Animals::draw() {
    putimage(x - r, y - r, r * 2, r * 2, &img, 0, 0);
}

void Animals::move() {
    x += v_x;
    y += v_y;
}