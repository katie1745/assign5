#ifndef GEM_H
#define GEM_H

#include "gameobject.h"
#include "position.h"
#include "raylib.h"

class Gem:public GameObject{
private:
    Position position;
    Texture2D texture;
public:
    Gem(Position pos, Texture2D tex){
        position=pos;
        texture=tex;
    }
    void render() override{
        DrawTextureEx(texture,(Vector2){(float)position.getX()-40,(float)position.getY()-40},0.0f,0.023f,WHITE);
    }
 
    int getX() const{return position.getX();}
    int getY() const{return position.getY();}




};


#endif