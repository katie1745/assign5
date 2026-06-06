#include "player.h"
#include "raylib.h"
#include "playerstate.h"
using namespace PlayerState;
Player::Player(Position initialPosition){
position=initialPosition;
name="Katie";
maxHealth=100;
health=maxHealth;
attack=10;
texture=LoadTexture("../resources/knight.png");
}

int Player::getHealth() { return health; } 
int Player::getMaxHealth() { return maxHealth; } 
std::string Player::getName() { return name; }
Position Player::getPosition() { return position; }
int Player::getX() const { return position.getX(); }
int Player::getY() const { return position.getY(); }

void Player::setPosition(Position position) { 
    this->position = position; 
}

Player::~Player(){
    UnloadTexture(texture);
}
void Player::render(){
    float scale=0.5f;
    float imgWidth=texture.width*scale;
    float imgHeight=texture.height*scale;
    //讓圖片正中心對準騎士真正座標
    int startX=position.getX()-(imgWidth/2);
    int startY=position.getY()-(imgHeight/2);
    DrawTextureEx(texture,(Vector2){(float)startX,(float)startY},0.0f,scale,WHITE);
}
MoveState Player::move(){
bool isMoving=false;

if(IsKeyDown(KEY_D)==true&&this->position.getX()<760){
this->position.setX(this->position.getX()+2);
isMoving=true;
}
if(IsKeyDown(KEY_A)==true&&this->position.getX()>0){
this->position.setX(this->position.getX()-2);
isMoving=true;
}
if(IsKeyDown(KEY_S)==true&&this->position.getY()<410){
this->position.setY(this->position.getY()+2);
isMoving=true;
}
if(IsKeyDown(KEY_W)==true&&this->position.getY()>0){
this->position.setY(this->position.getY()-2);
isMoving=true;
}
if(isMoving==true){
    return PlayerState::MOVE;
}
else{
    return PlayerState::IDLE;
}

}



