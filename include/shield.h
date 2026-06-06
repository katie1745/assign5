#ifndef SHIELD_H
#define SHIELD_H

#include "gameobject.h"
#include "position.h"
#include "raylib.h"


class Shield:public GameObject{
private:
    Position position;
    Texture2D texture;
    float existTimer;
public:
    Shield(Position pos,Texture2D tex){
        position=pos;
        texture=tex;
        existTimer=10.0f;
    }
    float getExiestTimer() const{return existTimer;}
    void render() override{
    DrawTextureEx(texture,(Vector2){(float)position.getX()-30,(float)position.getY()-30},0.0f,0.01f,WHITE);
    }
    void decreaseTimer(float dt){existTimer-=dt;}
    bool isExpired() const{return existTimer<=0.0f;}

    int getX() const{return position.getX();}
    int getY() const{return position.getY();}
};

#endif