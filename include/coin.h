#ifndef COIN_H
#define COIN_H

#include "position.h"
#include "gameobject.h"
#include "raylib.h"

class Coin:public GameObject{
private:
    Position position;
    Texture2D texture;
public:
    Coin(Position pos){
        this->position=pos;
        this->texture=LoadTexture("../resources/coin.png");
    }
    ~Coin(){
        UnloadTexture(texture);
    }
    void render() override{
        DrawTextureEx(texture,(Vector2){(float)position.getX()-15,(float)position.getY()-15},0.0f,0.01f,WHITE);
    }
};
#endif