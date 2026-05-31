#include "raylib.h"
#include "gameobject.h"
#include "position.h"
#include <vector>
#include "player.h"
#include "gameobjectfactory.h"
#include "enemy.h"
#include "bullet.h"

int main(){
	// 初始化
	InitWindow(800,450,"OOP Game");
	SetTargetFPS(60);
	Position startPos(400,225);
	Player* myPlayer=GameObjectFactory::createPlayer(startPos);
	Position coinPos(300,200);
	GameObject* myCoin=GameObjectFactory::createCoin(coinPos);
	int score=0;
	float survivalTime=0.0f;//記錄存活秒數
	float spawnTimer=0.0f;// 用來算每 2 秒生一隻怪的計時器
	int killCount=0;//擊殺數

	std::vector<Enemy*> enemies;// 用來存活著的怪物們的容器
	std::vector<Bullet*> bullets;//子彈們的容器
	std::vector<Position> gems;//寶石們的容器

	float shootTimer=0.0f;//射擊計時器，限制射擊頻率
	float playerInvincibleTimer = 0.0f;
	Texture2D gemTexture=LoadTexture("../resources/gem.png");
	// 遊戲畫面狀態列舉
	enum GameScreen{TITLE,GAMEPLAY,ENDING};
	GameScreen currentScreen=TITLE;

	Texture2D shieldTexture=LoadTexture("../resources/shield.png");
	float shieldSpawnTimer=0.0f;
	bool isShieldOnGround=false;
	Position groundShieldPos(0,0);
	bool inInvincible=false;//主角現在無敵嗎
	float invincibleTimer=0.0f;//防護罩剩餘時間	
	
	// 遊戲主迴圈
	while(!WindowShouldClose()){
		// ==========================================
        // Update 邏輯區塊
        // ==========================================
	switch(currentScreen){	
		case TITLE:
		
		if(IsKeyPressed(KEY_ENTER)){
				currentScreen=GAMEPLAY;
		}
		break;
		
		case GAMEPLAY:

		if(myPlayer->getHealth()>0){
		myPlayer->move();
		survivalTime+=GetFrameTime();
		spawnTimer+=GetFrameTime();
		shootTimer+=GetFrameTime();
		if(playerInvincibleTimer>0.0f){
			playerInvincibleTimer-=GetFrameTime();
		}
		if(inInvincible){
			invincibleTimer-=GetFrameTime();
			if(invincibleTimer<=0.0f){
				inInvincible=false;
			}
		}//end if(inInvincible)
		}//end if(myPlayer->getHealth>0)
		// 每 2 秒生一隻怪
		if(spawnTimer>=2.0f&& myPlayer->getHealth()>0){
			Position spawnPos(GetRandomValue(50,750),GetRandomValue(50,400));
			int enemyHp=3;
			float enemySpeed=1.5f;
			int enemySize=3;
			if(survivalTime>45.0f){
				enemyHp=15;
				enemySpeed=3.2f;
				enemySize=6;
			}
			else if(survivalTime>20.0f){
				enemyHp=7;
				enemySpeed=2.3f;
				enemySize=4;
			}
			Enemy* newEnemy =new Enemy(spawnPos,enemyHp,enemySpeed,enemySize);
			enemies.push_back(newEnemy);
			spawnTimer=0.0f;
		}//end if(spawnTimer>=2.0f)
		// 按空白鍵射擊，射擊頻率限制為 0.5 秒一次
		if(shootTimer>=0.4f&& !enemies.empty()&&myPlayer->getHealth()>0){
			//尋找最近的怪物
			float minDistSq=999999.0f;
			Enemy* closestEnemy=nullptr;
			for(Enemy* enemy : enemies) {
                float dx = (float)myPlayer->getX() - (float)enemy->getX();
                float dy = (float)myPlayer->getY() - (float)enemy->getY();
                float distSq = (dx * dx) + (dy * dy);
                if(distSq < minDistSq) { 
					minDistSq = distSq; 
					closestEnemy = enemy;
				}
			}//end for(Enemy* enemy : enemies)
				if(closestEnemy!=nullptr){
					bullets.push_back(new Bullet(myPlayer->getX()+20,myPlayer->getY()+20,closestEnemy->getX(),closestEnemy->getY()));;
				}
				shootTimer=0.0f;
        }//end if(shootTimer>=0.5f&& !enemies.empty())
		//子彈飛行與「精準碰撞」判定
		for(auto bIt=bullets.begin();bIt!=bullets.end();){
			(*bIt)->move();
			bool bulletDestroyed=false;
			for(auto eIt=enemies.begin();eIt!=enemies.end();){
				float dx = (float)(*bIt)->getX() - (float)(*eIt)->getX();
                float dy = (float)(*bIt)->getY() - (float)(*eIt)->getY();
                float rSum = 4.0f + (float)(*eIt)->getRadius();
                bool isHit = ((dx * dx) + (dy * dy)) <= (rSum * rSum);
				if(isHit){
					(*eIt)->takeDamage(1);
					if((*eIt)->isDead()){
						gems.push_back(Position((*eIt)->getX(),(*eIt)->getY()));
						killCount++;
						delete *eIt;
						eIt=enemies.erase(eIt);
					}
					else{
						eIt++;
					}
					bulletDestroyed=true;
					break;//一顆子彈只能打中一隻怪，所以打到怪物後就跳出怪物迴圈，繼續處理下一顆子彈
				}//end if(dist<=0)
				else{
					eIt++;
				}
			}//end for(auto eIt=enemies.begin();eIt!=enemies.end();)
			if(bulletDestroyed||(*bIt)->getY()<0){
				bIt=bullets.erase(bIt);
			}
			else{
				bIt++;
			}
		}//end for(auto bIt=bullets.begin();bIt!=bullets.end
		
			for(auto eIt = enemies.begin(); eIt != enemies.end(); eIt++){
            if (myPlayer->getHealth() > 0) {
                (*eIt)->chasePlayer(myPlayer);
            }
            
            float dx = (float)myPlayer->getX() - (float)(*eIt)->getX();
            float dy = (float)myPlayer->getY() - (float)(*eIt)->getY();
            float rSum = 30.0f + 30.0f; 
            bool isHit = ((dx * dx) + (dy * dy)) <= (rSum * rSum);
            
            // 如果撞到主角，而且主角沒有在無敵狀態，才扣血
            if(isHit && myPlayer->getHealth() > 0 && playerInvincibleTimer <= 0.0f&&!inInvincible){
                myPlayer->takeDamage(1); 
                playerInvincibleTimer = 1.0f; // 給予主角 1 秒無敵時間
            }
        	}//end for(auto eIt = enemies.begin(); eIt != enemies.end(); eIt++)
			// 5. 💎 吃寶石邏輯：主角走過去撞到寶石，Score 就加分！
			for(auto gIt=gems.begin();gIt!=gems.end();){
			float dx = (float)myPlayer->getX() - (float)gIt->getX();
            float dy = (float)myPlayer->getY() - (float)gIt->getY();
            float rSum = 20.0f + 20.0f;
            bool isHit = ((dx * dx) + (dy * dy)) <= (rSum * rSum);
				if(isHit && myPlayer->getHealth()>0){//吃到寶石
					score+=10;//加十分
					gIt=gems.erase(gIt);//寶石消失
				}
				else{
					gIt++;
				}

			}//end for(auto gIt=gems.begin();gIt!=gems.end();)
			//吃金幣邏輯
			if(myCoin!=nullptr){
				float dx=(float)myPlayer->getX()-300.0f;
				float dy=(float)myPlayer->getY()-200.0f;
				if((dx*dx)+(dy*dy)<=(30.0f*30.0f)){
					score+=100;
					delete myCoin;
					myCoin=nullptr;
				}
			}//end if(myCoin!=nullptr)
			if(myPlayer->getHealth()<=0){
				currentScreen=ENDING;
			}
			//防護罩出現機制
			shieldSpawnTimer+=GetFrameTime();
			if(shieldSpawnTimer>=30.0f){
				isShieldOnGround=true;
				shieldSpawnTimer=0.0f;//丟盾牌時間歸零，再重新算一次三十秒，三十掉了又會進入這個迴圈
				groundShieldPos=Position(GetRandomValue(100,700),GetRandomValue(100,250));
			}
			//如果盾牌出現超過十秒就會消失
			if(isShieldOnGround&&shieldSpawnTimer>=10.0f){
				isShieldOnGround=false;//盾牌消失，只能等下一個三十秒
			}
			if(isShieldOnGround){
				float dx=(float)myPlayer->getX()-(float)groundShieldPos.getX();
				float dy=(float)myPlayer->getY()-(float)groundShieldPos.getY();
				float rSum=30.0f+30.0f;//判斷的半徑
				if((dx*dx)+(dy*dy)<=(rSum*rSum)){
					isShieldOnGround=false;//吃到盾牌
					inInvincible=true;//啟動保護模式
					invincibleTimer=5.0f;
				}
			}
			break;
		case ENDING:
			if(IsKeyPressed(KEY_R)){
				score=0;
				survivalTime=0.0f;
				spawnTimer=0.0f;
				shootTimer=0.0f;
				playerInvincibleTimer=0.0f;
				killCount=0;
				//讓主角回到正中央
				myPlayer->setPosition(Position(400,225));
				myPlayer->setHealth(100);
				myPlayer->setPosition(Position(400,225));
				//清除怪物、子彈、寶石
				for(Enemy* enemy:enemies){delete enemy;}
				enemies.clear();
				for(Bullet* bullet:bullets){delete bullet;}
				bullets.clear();
				gems.clear();
				currentScreen=GAMEPLAY;
			}//end if(IsKeyPressed(KEY_R))
			break;
		}//end switch(currentScreen)
		// ==========================================
        // Render 繪圖區塊
        // ==========================================
		BeginDrawing();
		ClearBackground(GetColor(0x113311FF));
		switch(currentScreen){
			case TITLE:{
				//Raylib置中語法MeasureText、(螢幕總寬度 - 這串文字的總寬度) / 2 = 完美的起點 X 座標
				int titleWidth=MeasureText("Alien Survival",60);
				DrawText("Alien Survival",(800-titleWidth)/2,150,60,GOLD);
				DrawText("Press ENTER to Start",250,300,20,LIGHTGRAY);
				break;
			}
			case GAMEPLAY:
				for(int x=0;x<800;x+=40){
					for(int y=0;y<450;y+=40){
						if((x/40+y/40)%2==0){
							DrawRectangle(x,y,40,40,GetColor(0x153915FF));
						}
					}//end for(int y=0;y<450;y+=40)
				}//end for(int x=0;x<800;x+=40)
				if (myPlayer->getHealth() > 0) {
            		myPlayer->render();
					if(myCoin!=nullptr){
						myCoin->render();
					}
        		}//end if (myPlayer->getHealth() > 0)

				for(Position gem:gems){
					DrawTextureEx(gemTexture,(Vector2){(float)gem.getX()-40,(float)gem.getY()-40},0.0f,0.023f,WHITE);
				}//end for(Position gem:gems)
				for(Bullet* bullet:bullets){
					bullet->render();
				}//end for(Position bullet:bullets)
				for(Enemy* enemy:enemies){
					enemy->render();
				}//end for(Enemy* enemy:enemies)

				DrawRectangle(10, 10, 240, 170, Fade(BLACK, 0.6f));
       			DrawText(TextFormat("SCORE: %d", score), 20, 20, 25, GOLD);
        		DrawText(TextFormat("SURVIVAL: %.1fs", survivalTime), 20, 55, 20, WHITE);
        		DrawText(TextFormat("KILLS: %d", killCount), 20, 85, 20, LIGHTGRAY);
				if (myPlayer->getHealth() > 0) {
            		DrawText(TextFormat("PLAYER HP: %d", myPlayer->getHealth()), 20, 115, 20, RED);
        		} 
				//狀態一：盾牌還沒出現，下一次出現倒數計時
				if(!isShieldOnGround){
					float nextTime=30.0f-shieldSpawnTimer;
					DrawText(TextFormat("NEXT SHIELD: %.1fs",nextTime),20,145,20,SKYBLUE);
				}
				//狀態二：已經掉下來了，顯示倒數計時，提醒玩家趕快去吃
				else{
					float disapperTime=10.0f-shieldSpawnTimer;
					DrawText(TextFormat("SHIELD VANISH:%.1fs",disapperTime),20,145,20,ORANGE);
				}
				if(isShieldOnGround){
				DrawTextureEx(shieldTexture,(Vector2){(float)groundShieldPos.getX()-30,(float)groundShieldPos.getY()-30},
				0.0f,0.01f,WHITE);
				}
				//吃到盾牌後的防護罩
				if(inInvincible){
					DrawCircle(myPlayer->getX(),myPlayer->getY(),40,Fade(SKYBLUE,0.4f));
					DrawCircleLines(myPlayer->getX(),myPlayer->getY(),40,BLUE);
				}
				break;
			case ENDING:
				DrawText("GAME OVER",250,150,50,RED);
				DrawText(TextFormat("FINAL SCORE: %d",score),270,230,30,GOLD);
				DrawText(TextFormat("SURVIVAL: %.1fs", survivalTime),290,280,20,WHITE);
				DrawText("Press R to Restart",290,330,20,LIGHTGRAY);
				break;
		}//end switch(currentScreen)
		EndDrawing();
	}//end while(!WindowShouldClose())
//關閉遊戲前的最後清理
	for(Enemy* enemy : enemies) { delete enemy; }
	for(Bullet* bullet:bullets){ delete bullet;}
    delete myPlayer;
    CloseWindow();
	UnloadTexture(gemTexture);
	UnloadTexture(shieldTexture);
    return 0;
}

