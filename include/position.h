#ifndef POSITION_H
#define POSITION_H

class Position{
private:
    int x;
    int y;
public:
    Position();
    Position(int startX,int startY);

    int getX()const;
    int getY()const;
    void setX(int newX);
    void setY(int newY);
};


#endif