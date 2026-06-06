#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "player.h"
#include "position.h"
#include "coin.h"
#include "enemy.h"
#include "bullet.h"
#include "gem.h"
#include "shield.h"
#include "raylib.h"

class GameObjectFactory {
public:
    // 專門建立 Player 的靜態工廠函式
    static Player* createPlayer(Position pos) {
        return new Player(pos);
    }

    static Coin* createCoin(Position coin){
        return new Coin(coin);
    }

    //新增怪生產線
    static Enemy* createEnemy(Position pos,int hp,float spd,int size){
        return new Enemy(pos,hp,spd,size);
    }

    //新增子彈生產線
    static Bullet* createBullet(int startX,int startY,int targetX,int targetY){
        return new Bullet(startX,startY,targetX,targetY);
    }

    static Gem* createGem(Position pos,Texture2D tex){
        return new Gem(pos,tex);
    }

    static Shield* createShield(Position pos,Texture2D tex){
        return new Shield(pos,tex);
    }
};

#endif