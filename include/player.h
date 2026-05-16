#ifndef PLAYER_H
#define PLAYER_H
#include "position.h"
#include "gameobject.h"
#include "position.h"
#include "playerstate.h"
#include <string>

using namespace PlayerState;

class Player:public GameObject{
private:
    std::string name;
    Position position;
    int health;
    int maxHealth;
    int attack;

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
    




};
#endif