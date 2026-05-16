#include "raylib.h"
#include "gameobject.h"
#include "position.h"
#include <vector>
#include "player.h"
#include "gameobjectfactory.h"

int main(){
	InitWindow(800,450,"OOP Game");
	SetTargetFPS(60);
	Position startPos(400,225);
	Player* myPlayer=GameObjectFactory::createPlayer(startPos);
	Position coinPos(300,200);
	GameObject* myCoin=GameObjectFactory::createCoin(coinPos);
	int score=0;
	while(!WindowShouldClose()){
		myPlayer->move();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		myPlayer->render();
		myCoin->render();
		std::string scoreText="Score:"+std::to_string(score);
		DrawText(scoreText.c_str(),10,10,20,DARKGRAY);
		DrawText("Move with logic,score with heart!",10,420,20,DARKGRAY);
		EndDrawing();
	}
delete myPlayer;
delete myCoin;
CloseWindow();
return 0;




}