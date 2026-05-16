#include "player.h"
#include "raylib.h"
using namespace PlayerState;
Player::Player(Position initialPosition){
position=initialPosition;
name="Katie";
maxHealth=100;
health=maxHealth;
attack=10;
}

int Player::getHealth() { return health; } 
Position Player::getPosition() { return position; }
int Player:: getMaxHealth() { return maxHealth; } 
std::string Player:: getName() { return name; }
void Player::setPosition(Position position) { this->position=position; }

Player::~Player(){}
MoveState Player::move(){
bool isMoving=false;

if(IsKeyDown(KEY_D)==true&&this->position.getX()<760){
this->position.setX(this->position.getX()+1);
isMoving=true;
}
if(IsKeyDown(KEY_A)==true&&this->position.getX()>0){
this->position.setX(this->position.getX()-1);
isMoving=true;
}
if(IsKeyDown(KEY_S)==true&&this->position.getY()<410){
this->position.setY(this->position.getY()+1);
isMoving=true;
}
if(IsKeyDown(KEY_W)==true&&this->position.getY()>0){
this->position.setY(this->position.getY()-1);
isMoving=true;
}
if(isMoving==true){
    return PlayerState::MOVE;
}
else{
    return PlayerState::IDLE;
}

}


void Player::render() {
    // DrawRectangle(x, y, 寬, 高, 顏色)
    DrawRectangle(position.getX(), position.getY(), 40, 40, BLUE); 
}