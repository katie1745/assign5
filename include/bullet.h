#ifndef BULLET_H
#define BULLET_H
#include "position.h"
#include "raylib.h"
#include <cmath>//為了使用開根號sqrt()

class Bullet:public GameObject{
private:
    Position position;
    float speedX;
    float speedY;
    float speed;//子彈的移動速度
public:
    Bullet(int startX,int startY,int targetX,int targetY){
        position.setX(startX);
        position.setY(startY);
        speed=8.0f;
        //算起點到目標的總距離
        float dx=(float)targetX-(float)startX;
        float dy=(float)targetY-(float)startY;
        float dist=std::sqrt((dx*dx)+(dy*dy));
        if(dist>0){
            speedX=(dx/dist)*speed;
            speedY=(dy/dist)*speed;
        }
        else{
            speedX=0;
            speedY=-speed;//預設往上飛
        }
    }//end Bullet建構子
    void move(){
        position.setX(position.getX()+speedX);
        position.setY(position.getY()+speedY);
    }
    void render() override{
        DrawCircle(position.getX(),position.getY(),4,SKYBLUE);
        DrawCircle(position.getX(),position.getY(),2,WHITE);
    }
    int getX() const{ return position.getX();}
    int getY() const{ return position.getY();}
};

#endif