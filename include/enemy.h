#ifndef ENEMY_H
#define ENEMY_H
#include "gameobject.h"
#include "position.h"
#include "player.h"
#include "raylib.h"

// 🧟 繼承 GameObject 
class Enemy:public GameObject{
private:
    Position enemyPos;// 怪物的當前座標 
    int hp;// 怪物的血量
    float speed;// 怪物的移動速度
    int pixelSize;// 👾 像素格子的大小（決定體型）
    int radius;//  根據體型自動算出的「碰撞半徑」
    Texture2D texture;//存怪物的圖片
public:
    // 出生時指定位置、血量、速度
    Enemy(Position pos, int initialHp=3, float s=1.5f, int pSize=3):enemyPos(pos),hp(initialHp),speed(s),pixelSize(pSize){
        texture=LoadTexture("../resources/monster.png");
        //根據自動縮小後的圖片自動計算出碰撞半徑
        float scale=0.1f;
        radius=(texture.width*scale)/2;
    }
    ~Enemy(){
        UnloadTexture(texture);
    }

    //被子彈打到時呼叫的扣血功能
    void takeDamage(int amount){
        hp-=amount;
    }
    
    // 檢查怪物是不是死掉了
    bool isDead() const{
        return hp<=0;
    }
   

    // 核心追逐邏輯：讓小怪朝著主角的位置移動
    void chasePlayer(Player* player){
        int playerX= player -> getX();
        int playerY= player -> getY();

        int enemyX=enemyPos.getX();
        int enemyY=enemyPos.getY();
        //小怪開始追主角的動作
        if(enemyX<playerX){enemyX+=speed;}
        else if(enemyX>playerX){enemyX-=speed;}
        
        if(enemyY<playerY){enemyY+=speed;}
        else if(enemyY>playerY){enemyY-=speed;}

            enemyPos.setX(enemyX);
            enemyPos.setY(enemyY);

    }


    // 畫出紫色圓形小怪
    void render() override {
       float scale=0.1f;
       float imgWidth=texture.width*scale;
       float imgHeight=texture.height*scale;
       int startX=enemyPos.getX()-(imgWidth/2);
       int startY=enemyPos.getY()-(imgHeight/2);
       DrawTextureEx(texture,(Vector2){(float)startX,(float)startY},0.0f,scale,WHITE);
    }//end void render() override

    //  取得座標，方便之後算碰撞
   int getX() const{return enemyPos.getX();}
   int getY() const{return enemyPos.getY();}
   int getRadius() const{return radius;}//大隻怪獸會有大半徑
};
#endif