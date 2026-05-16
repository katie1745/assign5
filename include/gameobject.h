#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject{
public:
    //讓所有遊戲物件都要有自己的畫畫方式
    virtual void render()=0;
    virtual ~GameObject(){}
};
#endif
