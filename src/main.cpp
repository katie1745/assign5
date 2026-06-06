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
	SetTargetFPS(60);// 【作用】將遊戲的最高更新率鎖定在每秒 60 幀，避免電腦把效能浪費在畫多餘的影格上（防止筆電風扇狂轉、發燙）。
	// 【原理】如果沒有這行，迴圈會毫無保留地狂飆。設定後，Raylib 只要提早畫完一張圖，就會自動「休眠」等待，大幅降低 CPU 負擔，「效能控制」
	Position startPos(400,225);
	Player* myPlayer=GameObjectFactory::createPlayer(startPos);
	Position coinPos(300,200);
	GameObject* myCoin=GameObjectFactory::createCoin(coinPos);
	int score=0;
	float survivalTime=0.0f;//記錄存活秒數
	float spawnTimer=0.0f;// 用來算每 2 秒生一隻怪的計時器
	int killCount=0;//擊殺數
	float spawnInterval=2.0f;//	控制生怪速度的變數每 2 秒生一隻怪

	std::vector<Enemy*> enemies;// 用來存活著的怪物們的容器
	std::vector<Bullet*> bullets;//子彈們的容器
	std::vector<Gem*> gems;//寶石們的容器
	std::vector<Shield*> shields;


	float shootTimer=0.0f;//射擊計時器，限制射擊頻率
	float shootInterval=0.4f;
	float playerInvincibleTimer = 0.0f;
	Texture2D gemTexture=LoadTexture("../resources/gem.png");
	
	enum GameScreen{TITLE,GAMEPLAY,ENDING};
	GameScreen currentScreen=TITLE;

	Texture2D shieldTexture=LoadTexture("../resources/shield.png");
	float shieldSpawnTimer=0.0f;
	
	
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
		
		}//end if(myPlayer->getHealth>0)
		// 每 2 秒生一隻怪
		if(spawnTimer>=spawnInterval&& myPlayer->getHealth()>0){
			int edge=GetRandomValue(0,3);
			int spawnX=0;
			int spawnY=0;
			if(edge==0){
				spawnX=GetRandomValue(-50,850);
				spawnY=-50;
			}
			else if(edge==1){
				spawnX=GetRandomValue(-50,850);
				spawnY=500;
			}
			else if(edge==2){
				spawnX=-50;
				spawnY=500;
			}
			else if(edge==3){
				spawnX=850;
				spawnY=GetRandomValue(-50,500);
			}
			Position spawnPos(spawnX,spawnY);
			//生怪的血量、速度、體型會隨著存活時間增加而變強，讓遊戲越玩越難
			int enemyHp=3;
			float enemySpeed=1.5f;
			int enemySize=3;
			// 3. 難度動態升級：存活時間越久，怪物越強
            if(survivalTime>10.0f){
                enemyHp=15;
                enemySpeed=3.2f;
                enemySize=6;
                shootInterval=0.10f; // 變身機關槍
                spawnInterval=0.2f;  // 怪物海
                myPlayer->setSpeed(5.0f); 
            }
            else if(survivalTime>5.0f){
                enemyHp=7;
                enemySpeed=2.3f;
                enemySize=4;
                shootInterval=0.25f; // 變身散彈槍
                spawnInterval=0.6f;  // 生怪加快
                myPlayer->setSpeed(4.0f); 
            }

            // 4. 真正把怪物造出來、裝進陣列、碼表歸零（就是妳剛剛不小心刪掉的這三句！）
            Enemy* newEnemy = GameObjectFactory::createEnemy(spawnPos, enemyHp, enemySpeed, enemySize);
            enemies.push_back(newEnemy);
            spawnTimer = 0.0f;
			
		}//end if(spawnTimer>=2.0f)
		if(shootTimer>=shootInterval && !enemies.empty()&&myPlayer->getHealth()>0){
			//尋找最近的怪物
			int maxTargets=1;//預設一次只打一隻
			if(survivalTime>5.0f){
				maxTargets=5;//存活超過10秒後一次可以打三隻
			}
			if(survivalTime>10.0f){
				maxTargets=10;//存活超過10秒後一次可以打六隻
			}
			int targetsFired=0;//記錄這回合已瞄準過幾隻怪

			for(Enemy*enemy:enemies){
				float dx=(float)myPlayer->getX()-(float)enemy->getX();
				float dy=(float)myPlayer->getY()-(float)enemy->getY();
				float distSq=(dx*dx)+(dy*dy);
				if(distSq<(450.0f*450.0f)){
					bullets.push_back(GameObjectFactory::createBullet(myPlayer->getX()+20,myPlayer->getY()+20,enemy->getX(),enemy->getY()));
					targetsFired++;
				}
				if(targetsFired>=maxTargets){
					break;
				}
			}//end for(Enemy*enemy:enemies)
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
						gems.push_back(GameObjectFactory::createGem(Position((*eIt)->getX(),(*eIt)->getY()),gemTexture));
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
            if(isHit && myPlayer->getHealth() > 0 && playerInvincibleTimer <= 0.0f&& !myPlayer->checkInvincible()){
                myPlayer->takeDamage(1); 
                playerInvincibleTimer = 1.0f; // 給予主角 1 秒無敵時間
            }
        	}//end for(auto eIt = enemies.begin(); eIt != enemies.end(); eIt++)
			
			// 5. 💎 吃寶石邏輯：主角走過去撞到寶石，Score 就加分！
			for(auto gIt=gems.begin();gIt!=gems.end();){
				float dx=(float)myPlayer->getX()-(float)(*gIt)->getX();
				float dy=(float)myPlayer->getY()-(float)(*gIt)->getY();
				if(((dx*dx)+(dy*dy))<=(40.0f*40.0f)&& myPlayer->getHealth()>0){
					score+=10;
					delete *gIt;
					gIt=gems.erase(gIt);
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
				Position pos(GetRandomValue(100,700),GetRandomValue(100,250));
				shields.push_back(GameObjectFactory::createShield(pos,shieldTexture));
				shieldSpawnTimer=0.0f;
			}
			//如果盾牌出現超過十秒就會消失
			for(auto sIt=shields.begin();sIt!=shields.end();){
				(*sIt)->decreaseTimer(GetFrameTime());
				float dx=(float)myPlayer->getX()-(float)(*sIt)->getX();
				float dy=(float)myPlayer->getY()-(float)(*sIt)->getY();
				if(((dx*dx)+(dy*dy))<=(60.0f*60.0f)){
					myPlayer->activateShield(5.0f);
					delete *sIt;
					sIt=shields.erase(sIt);
				}
				else if((*sIt)->isExpired()){
					delete *sIt;
					sIt=shields.erase(sIt);
				}
				else{
					sIt++;
				}
			}
			myPlayer->updateInvincible(GetFrameTime());
			
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
					myPlayer->renderShield();
					if(myCoin!=nullptr){
						myCoin->render();
					}
        		}//end if (myPlayer->getHealth() > 0)

				for(Gem* gem:gems){
					gem->render();
				}//end for(Gem* gem:gems)
				for(Bullet* bullet:bullets){
					bullet->render();
				}//end for(Bullet* bullet:bullets)
				for(Enemy* enemy:enemies){
					enemy->render();
				}//end for(Enemy* enemy:enemies)
				for(Shield* shield:shields){
					shield->render();
				}

				DrawRectangle(10, 10, 240, 170, Fade(BLACK, 0.6f));
       			DrawText(TextFormat("SCORE: %d", score), 20, 20, 25, GOLD);
        		DrawText(TextFormat("SURVIVAL: %.1fs", survivalTime), 20, 55, 20, WHITE);
        		DrawText(TextFormat("KILLS: %d", killCount), 20, 85, 20, LIGHTGRAY);
				if (myPlayer->getHealth() > 0) {
            		DrawText(TextFormat("PLAYER HP: %d", myPlayer->getHealth()), 20, 115, 20, RED);
        		} 
				//狀態一：盾牌還沒出現，下一次出現倒數計時
				if(shields.empty()){
					float nextTime=30.0f-shieldSpawnTimer;
					DrawText(TextFormat("NEXT SHIELD: %.1fs",nextTime),20,145,20,SKYBLUE);
				}
				//狀態二：已經掉下來了，顯示倒數計時，提醒玩家趕快去吃
				else{
					float disapperTime=10.0f-shieldSpawnTimer;
					DrawText(TextFormat("SHIELD VANISH:%.1fs",disapperTime),20,145,20,ORANGE);
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
	for(Gem* gem:gems){delete gem;}
	for(Shield*shield:shields){delete shield;}
    delete myPlayer;
    CloseWindow();
	UnloadTexture(gemTexture);
	UnloadTexture(shieldTexture);
    return 0;
}

