#include"position.h"

//預設建構子，如果沒有指定，物件出生就是(0.0)
Position::Position(){
    x=0;
    y=0;
}
Position::Position(int startX,int startY){
    x=startX;
    y=startY;
}

int Position::getX()const{return x;}

int Position::getY()const{return y;}

void Position::setX(int newX){this->x=newX;}

void Position::setY(int newY){this->y=newY;}