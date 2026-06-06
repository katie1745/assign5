#ifndef PLAYER_H
#define PLAYER_H
#include "position.h"
#include "gameobject.h"
#include "position.h"
#include "playerstate.h"
#include <string>
#include "raylib.h"

using namespace PlayerState;

class Player:public GameObject{
private:
    std::string name;
    Position position;
    int health;
    int maxHealth;
    int attack;
    Texture2D texture;//儲存騎士的圖片
    bool isInvincible=false;
    float invincibleTimer=0.0f;
    float speed=2.0f;
public:
    Player(Position initialPosition);
    ~Player();
    void render() override;
    
    Position getPosition();
    void setPosition(Position position);
    int getMaxHealth();
    int getHealth();
    std::string getName();
    MoveState move();
    int getX() const;
    int getY() const;
    int getHealth() const{return health;}
    void takeDamage(int amount){
        health-=amount;
        if(health<0){
            health=0;
        }
    }
    void setHealth(int h){health=h;}
    void activateShield(float time){
        isInvincible=true;
        invincibleTimer=time;
    }
    void updateInvincible(float dt){
        if(isInvincible){
            invincibleTimer-=dt;
            if(invincibleTimer<=0.0f){
                isInvincible=false;
            }
        }
    }//end void updateInvincible(float dt)
    void renderShield(){
        if(isInvincible){
            DrawCircle(getX(),getY(),40,Fade(SKYBLUE,0.4f));
            DrawCircleLines(getX(),getY(),40,BLUE);
        }
    }//end void renderShield()
    bool checkInvincible() const{ return isInvincible;}
    void setSpeed(float newSpeed){
        speed=newSpeed;
    }

};
#endif