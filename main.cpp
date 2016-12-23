#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

//Initalization +
struct TILE_TYPE{
	bool isPassable;
};

bool isPassable(float x, float y, float width, float height);
bool checkCollision(float x, float y, float ex, float ey, float width, float height, float ewidth, float eheight);
bool insideMap(float x, float y, float width, float height);
void addButtonToList(Button *newButton);
void addMissileToList(MissileEntity *newMissile);
void addLivingToList(LivingEntity *newLiving);
void addParticleToList(ParticleEntity *newParticle);
void addDebugToList(DebugClass *newDebug);
void loadMapArray();
void saveMapArray();
void drawMap();
void drawTile(float x, float y, int tileId);

ALLEGRO_DISPLAY *display;

ALLEGRO_FONT *defaultFont;
ALLEGRO_FONT *smallFont;
ALLEGRO_FONT *bigFont;

ALLEGRO_BITMAP *cursorImage;
ALLEGRO_BITMAP *playerImage;
ALLEGRO_BITMAP *bulletImage;
ALLEGRO_BITMAP *pixelImage;
ALLEGRO_BITMAP *zombieImage;
ALLEGRO_BITMAP *explosionImage;

ALLEGRO_BITMAP *groundImage1;
ALLEGRO_BITMAP *groundImage2;
ALLEGRO_BITMAP *wallImage;
ALLEGRO_BITMAP *brokenWallImage;
ALLEGRO_BITMAP *portalImage1;
ALLEGRO_BITMAP *portalImage2;
ALLEGRO_BITMAP *floorImage;
ALLEGRO_BITMAP *brokenFloorImage;
ALLEGRO_BITMAP *bloodGroundImage1;
ALLEGRO_BITMAP *bloodGroundImage2;
ALLEGRO_BITMAP *vineWallImage;

ALLEGRO_SAMPLE *pistolSound;
ALLEGRO_SAMPLE *machineGunSound;
ALLEGRO_SAMPLE *shotgunSound;
ALLEGRO_SAMPLE *rocketLauncherSound;
ALLEGRO_SAMPLE *explosionSound;
ALLEGRO_SAMPLE *reloadSound;
ALLEGRO_SAMPLE *hurtSound;
ALLEGRO_SAMPLE *killSound;
ALLEGRO_SAMPLE *levelSound;
ALLEGRO_SAMPLE *buttonSound;
ALLEGRO_SAMPLE *buttonNoSound;

ALLEGRO_KEYBOARD_STATE keyState;
ALLEGRO_MOUSE_STATE mouseState;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;

Button *buttonList[MAX_BUTTONS];
MissileEntity *missileList[MAX_MISSILES];
LivingEntity *livingList[MAX_LIVING];
DebugClass *debugList[MAX_DEBUG];
ParticleEntity *particleList[MAX_PARTICLES];
Player *playerList[1];

Engine engine;

int mapArray[mapArrayWidth][mapArrayHeight];
int mapArrayRotation[mapArrayWidth][mapArrayHeight];

///gunId;fireTime;attackDamage;maxMagazine;reloadTime;bulletSpeed;autoFire;cost;maxAttackDamage;
int weaponArray[5][3][9] =  {{{0, 20, 1, 10, 60, 8, 0, 10, 1}, {0, 10, 1, 6, 90, 16, 0, 20, 2}, {0, 30, 2, 20, 120, 16, 0, 30, 2}},
                            {{0, 20, 4, 6, 60, 16, 0, 45, 5}, {2, 20, 2, 25, 90, 24, 1, 60, 2}, {3, 60, 2, 2, 60, 24, 0, 60, 2}},
                            {{0, 15, 5, 12, 90, 24, 0, 80, 6}, {2, 7, 2, 20, 60, 24, 1, 115, 4}, {1, 30, 2, 4, 75, 24, 0, 130, 3}},
                            {{2, 5, 2, 50, 120, 32, 1, 200, 3}, {1, 20, 1, 12, 45, 32, 1, 300, 3}, {4, 30, 3, 3, 120, 8, 0, 400, 10}},
                            {{5, 2, 4, 1000, 180, 32, 1, 800, 8}, {4, 2, 3, 200, 120, 8, 1, 1250, 7}, {6, 60, 5, 1, 60, 0, 0, 1500, 15}}};

///maxHealth;meleeDamage;maxStamina;maxMovementSpeed;
float skillArray[5][3][4] = {{{1.5, 1, 1, 1}, {1, 2, 1, 1}, {1, 1, 1.5, 1}},
                            {{2, 1, 1, 1}, {1, 2, 1, 1}, {1, 1, 1, 1.5}},
                            {{2, 1, 1, 1}, {1, 1.5, 1, 1}, {1, 1, 1.5, 1.5}},
                            {{1.5, 1.5, 1.5, 1.5}, {2, 1, 2, 1}, {1, 1, 2, 2}},
                            {{2, 2, 2, 2}, {3, 1, 3, 1}, {1, 1, 4, 4}}};

bool boughtWeapon, leveledSkill;
int tierWeapon, tierSkill;

const char* versionNumber;
float fpsTimeNew, fpsCounter, fpsTimeOld;

bool drawScreen, timerEvent, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame;
float mouseX, mouseY, volumeLevel;
int lastKeyPress, mouseWheel = 0;

int enemySpawnRateHelper, enemySpawnRate, pauseBetweenWavesHelper, amountEnemiesSpawned, amountEnemiesSpawn, enemySpeed, enemyMoney, enemyExperience, enemyDamage, enemyHealth;
bool pauseBetweenWavesActive, enemySpawnAll, winStatement, loseStatement, paused;
int wave;

TILE_TYPE tileIndex[] = {
	{true}, // (0) TILE_GROUND1
	{true}, // (1) TILE_GROUND2
	{false}, // (2) TILE_WALL
	{false}, // (3) TILE_BROKENWALL
	{false}, // (4) TILE_PORTAL1
	{false}, // (5) TILE_PORTAL2
	{true}, // (6) TILE_FLOOR
	{true}, // (7) TILE_BROKENFLOOR
	{true}, // (8) TILE_BLOODGROUND1
	{true}, // (9) TILE_BLOODGROUND2
	{false}, // (10) TILE_VINEWALL
};
//Initalization -

int main(){
    srand(time(0));

	Engine engine;

	//Initialize the Engine
	engine.init("Ludum Dare 31 - Whole Game on One Screen", screenWidth, screenHeight, false);

	//Load the Menu
	engine.changeState(PlayState::instance());

    //Timestep Variables
    double t = 0.0;
    double dt = 1/LogicFPS;

    double currentTime = 0.0;
    double newTime = 0.0;
    double frameTime = 0.0;

    double accumulator = 0.0;

	//Main Loop
    while(engine.running()){
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        timerEvent = false;

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            engine.quit();
        }

        if(events.type == ALLEGRO_EVENT_TIMER){
            timerEvent = true;
        }
        //Main Timer Event +
        if(timerEvent){
            al_get_mouse_state(&mouseState);

            al_get_keyboard_state(&keyState);

            //Update Mouse Variables +
            mouseX = al_get_mouse_state_axis(&mouseState,0);
            mouseY = al_get_mouse_state_axis(&mouseState,1);
            mouseWheel = al_get_mouse_state_axis(&mouseState, 2);

            if(al_mouse_button_down(&mouseState, 1)){
                mouseButtonLeft = true;
            }else if(mouseButtonLeft){
                mouseButtonLeftClick = true;
                mouseButtonLeft = false;
            }else{
                mouseButtonLeftClick = false;
            }

            if(al_mouse_button_down(&mouseState, 2)){
                mouseButtonRight = true;
            }else if(mouseButtonRight){
                mouseButtonRightClick = true;
                mouseButtonRight = false;
            }else{
                mouseButtonRightClick = false;
            }
            //Update Mouse Variables -

            //Rest +
            if(!al_key_down(&keyState, lastKeyPress)){
                lastKeyPress = 0;
            }
            //Rest -

            if(events.timer.source == timer){
                newTime = al_get_time();
                frameTime = newTime - currentTime;
                if(frameTime > 0.25)
                    frameTime = 0.25;	  // note: max frame time to avoid spiral of death
                currentTime = newTime;

                accumulator += frameTime;

                while(accumulator >= dt){
                    engine.update(); //Call the gameState specfic update
                    t += dt;
                    accumulator -= dt;
                }

                engine.draw();

                al_draw_bitmap(cursorImage, mouseX-4, mouseY-4, 0);

                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }
        }
        //Main Timer Event -
    }
	// cleanup the engine
	engine.cleanup();

}

bool isPassable(float x, float y, float width, float height){
    if(!insideMap(x, y, width, height)){
        return false;
    }else{
        int tX = floor(x/tileSize), tY = floor(y/tileSize), tWX = floor((x+width-1)/tileSize), tHY = floor((y+height-1)/tileSize);

        if(!tileIndex[mapArray[tX][tY]].isPassable || !tileIndex[mapArray[tWX][tY]].isPassable || !tileIndex[mapArray[tX][tHY]].isPassable || !tileIndex[mapArray[tWX][tHY]].isPassable){
            return false;
        }
    }
    return true;
}

bool checkCollision(float x, float y, float ex, float ey, float width, float height, float ewidth, float eheight){
    if(x + width - 1 < ex || x > ewidth + ex - 1 || y + height - 1 < ey || y > eheight + ey - 1){
        return false;
    }else{
        return true;
    }
}

bool insideMap(float x, float y, float width, float height){
    if(x < 0 || x + width-1 >= mapArrayWidth*tileSize || y < 0 || y + height-1 >= mapArrayHeight*tileSize){
        return false;
    }
    return true;
}

void addButtonToList(Button *newButton){
    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] == NULL || !buttonList[i]->checkActive()){
            newButton->setEntityId(i);
            buttonList[i] = newButton;
            return;
        }
    }
}

void addMissileToList(MissileEntity *newMissile){
    for(int i = 0; i < MAX_MISSILES; i++){
        if(missileList[i] == NULL || !missileList[i]->checkActive()){
            newMissile->setEntityId(i);
            missileList[i] = newMissile;
            return;
        }
    }
}

void addLivingToList(LivingEntity *newLiving){
    for(int i = 0; i < MAX_LIVING; i++){
        if(livingList[i] == NULL || !livingList[i]->checkActive()){
            newLiving->setEntityId(i);
            livingList[i] = newLiving;
            return;
        }
    }
}

void addParticleToList(ParticleEntity *newParticle){
    for(int i = 0; i < MAX_PARTICLES; i++){
        if(particleList[i] == NULL || !particleList[i]->checkActive()){
            newParticle->setEntityId(i);
            particleList[i] = newParticle;
            return;
        }
    }
}


void addDebugToList(DebugClass *newDebug){
    for(int i = 0; i < MAX_DEBUG; i++){
        if(debugList[i] == NULL || !debugList[i]->checkActive()){
            newDebug->setEntityId(i);
            debugList[i] = newDebug;
            return;
        }
    }
}

void loadMapArray(){
    ifstream mapArrayFile;
    mapArrayFile.open("config/MapArray.txt");

    for(int y = 0; y < mapArrayHeight; y++){
        for(int x = 0; x < mapArrayWidth; x++){
            mapArrayFile >> mapArray[x][y];

            if(mapArray[x][y] != 2 && mapArray[x][y] != 6){
                if(mapArray[x][y] == 0){
                    mapArray[x][y] = ((rand()%200 == 0) ? 8 : 0) + rand()%2;
                    mapArrayRotation[x][y] = rand()%4;
                }else if(mapArray[x][y] == 4){
                    mapArray[x][y] = 4+rand()%2;
                    mapArrayRotation[x][y] = rand()%2*2;

                }else if(mapArray[x][y] == 3 || mapArray[x][y] == 7){
                    if(mapArray[x][y] == 3 && rand()%5 == 0)
                        mapArray[x][y] = 10;
                    mapArrayRotation[x][y] = rand()%4;
                }
            }
        }
    }

    mapArrayFile.close();
}

void saveMapArray(){
    ofstream mapArrayFile;
    mapArrayFile.open("config/MapArray.txt");

    for(int y = 0; y < mapArrayHeight; y++){
        for(int x = 0; x <= mapArrayWidth; x++){
            if(x < mapArrayWidth){
                mapArrayFile << mapArray[x][y] << " ";
            }else if(x == mapArrayWidth){
                mapArrayFile << endl;
            }
        }
    }

    mapArrayFile.close();
}

void drawMap(){
    for(int x = 0; x < mapDisplayWidth/tileSize+1; x++){
        for(int y = 0; y < mapDisplayHeight/tileSize+1; y++){
            drawTile(x, y, mapArray[x][y]);
        }
    }
}

void drawTile(float x, float y, int tileId){
    switch(tileId){
        case 0:
            al_draw_rotated_bitmap(groundImage1, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[(int)x][(int)y]*90*toRadians, NULL);
            break;

        case 1:
            al_draw_rotated_bitmap(groundImage2, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[(int)x][(int)y]*90*toRadians, NULL);
            break;

        case 2:
            al_draw_bitmap(wallImage, x*tileSize, y*tileSize, NULL);
            break;

        case 3:
            al_draw_rotated_bitmap(brokenWallImage, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[(int)x][(int)y]*90*toRadians, NULL);
            break;

        case 4:
            al_draw_rotated_bitmap(portalImage1, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[(int)x][(int)y]*90*toRadians, NULL);
            break;

        case 5:
            al_draw_rotated_bitmap(portalImage2, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[(int)x][(int)y]*90*toRadians, NULL);
            break;

        case 6:
            al_draw_bitmap(floorImage, x*tileSize, y*tileSize, NULL);
            break;

        case 7:
            al_draw_rotated_bitmap(brokenFloorImage, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[(int)x][(int)y]*90*toRadians, NULL);
            break;

        case 8:
            al_draw_rotated_bitmap(bloodGroundImage1, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[(int)x][(int)y]*90*toRadians, NULL);
            break;

        case 9:
            al_draw_rotated_bitmap(bloodGroundImage2, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[(int)x][(int)y]*90*toRadians, NULL);
            break;

        case 10:
            al_draw_rotated_bitmap(vineWallImage, tileSize/2, tileSize/2, x*tileSize+tileSize/2, y*tileSize+tileSize/2, mapArrayRotation[(int)x][(int)y]*90*toRadians, NULL);
            break;
    }
}
