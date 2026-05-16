#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "player.h"
#include "position.h"
#include "coin.h"

class GameObjectFactory {
public:
    // 專門建立 Player 的靜態工廠函式
    static Player* createPlayer(Position pos) {
        return new Player(pos);
    }
    static GameObject* createCoin(Position coin){
        return new Coin(coin);
    }
};

#endif