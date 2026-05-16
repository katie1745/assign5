#ifndef COIN_H
#define COIN_H

#include "position.h"
#include "gameobject.h"
#include "raylib.h"

class Coin:public GameObject{
private:
    Position position;
public:
    Coin(Position pos){
        this->position=pos;
    }
    void render() override{
        DrawCircle(this->position.getX(),this->position.getY(),15,YELLOW);
    }
};
#endif