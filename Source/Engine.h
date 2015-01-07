#ifndef ENGINE_H
#define ENGINE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <fstream>

#include "Button.h"
#include "Player.h"
#include "PistolBullet.h"
#include "DebugClass.h"
#include "Zombie.h"
#include "ParticleEntity.h"

#define PI 3.14159265359
#define toRadians 0.0174532925

#define screenWidth 1280
#define screenHeight 720

#define mapDisplayWidth 1280
#define mapDisplayHeight 640

#define mapArrayWidth 40
#define mapArrayHeight 20

#define tileSize 32

#define MAX_BUTTONS 25
#define MAX_LIVING 300
#define MAX_MISSILES 300
#define MAX_PARTICLES 1000
#define MAX_PLAYERS 1
#define MAX_DEBUG 0

#define FPS 60.0

using namespace std;

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
void updateCamera();

extern ALLEGRO_DISPLAY *display;

extern ALLEGRO_FONT *defaultFont;
extern ALLEGRO_FONT *smallFont;
extern ALLEGRO_FONT *bigFont;

extern ALLEGRO_BITMAP *cursorImage;
extern ALLEGRO_BITMAP *playerImage;
extern ALLEGRO_BITMAP *bulletImage;
extern ALLEGRO_BITMAP *pixelImage;
extern ALLEGRO_BITMAP *zombieImage;
extern ALLEGRO_BITMAP *explosionImage;

extern ALLEGRO_BITMAP *groundImage1;
extern ALLEGRO_BITMAP *groundImage2;
extern ALLEGRO_BITMAP *wallImage;
extern ALLEGRO_BITMAP *brokenWallImage;
extern ALLEGRO_BITMAP *portalImage1;
extern ALLEGRO_BITMAP *portalImage2;
extern ALLEGRO_BITMAP *floorImage;
extern ALLEGRO_BITMAP *brokenFloorImage;
extern ALLEGRO_BITMAP *bloodGroundImage1;
extern ALLEGRO_BITMAP *bloodGroundImage2;
extern ALLEGRO_BITMAP *vineWallImage;

extern ALLEGRO_SAMPLE *pistolSound;
extern ALLEGRO_SAMPLE *machineGunSound;
extern ALLEGRO_SAMPLE *shotgunSound;
extern ALLEGRO_SAMPLE *rocketLauncherSound;
extern ALLEGRO_SAMPLE *explosionSound;
extern ALLEGRO_SAMPLE *reloadSound;
extern ALLEGRO_SAMPLE *hurtSound;
extern ALLEGRO_SAMPLE *killSound;
extern ALLEGRO_SAMPLE *levelSound;
extern ALLEGRO_SAMPLE *buttonSound;
extern ALLEGRO_SAMPLE *buttonNoSound;

extern ALLEGRO_KEYBOARD_STATE keyState;
extern ALLEGRO_MOUSE_STATE mouseState;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *timer;

extern Button *buttonList[MAX_BUTTONS];
extern MissileEntity *missileList[MAX_MISSILES];
extern LivingEntity *livingList[MAX_LIVING];
extern DebugClass *debugList[MAX_DEBUG];
extern ParticleEntity *particleList[MAX_PARTICLES];
extern Player *playerList[1];

extern const char* versionNumber;
extern float fpsTimeNew, fpsCounter, fpsTimeOld;

extern bool drawScreen, timerEvent, done, mouseButtonLeft, mouseButtonLeftClick, mouseButtonRight, mouseButtonRightClick, inGame;
extern float mouseX, mouseY, volumeLevel;
extern int lastKeyPress, mouseWheel;

extern int mapArray[mapArrayWidth][mapArrayHeight];
extern int mapArrayRotation[mapArrayWidth][mapArrayHeight];

extern int weaponArray[5][3][9];
extern float skillArray[5][3][4];

extern bool boughtWeapon, leveledSkill;
extern int tierWeapon, tierSkill;

extern int enemySpawnRateHelper, enemySpawnRate, pauseBetweenWavesHelper, amountEnemiesSpawned, amountEnemiesSpawn, enemySpeed, enemyMoney, enemyExperience, enemyDamage, enemyHealth;
extern bool pauseBetweenWavesActive, enemySpawnAll, winStatement, loseStatement, paused;
extern int wave;

class GameState;

class Engine{
    public:
        void init(const char* title, int width, int height, bool fullscreen);
        void cleanup();

        void changeState(GameState* state);
        void pushState(GameState* state);
        void popState();

        void update();
        void draw();

        bool running(){ return m_running; }
        void quit(){m_running = false;}

    private:
        // the stack of states
        vector<GameState*> states;

        bool m_running;
        bool m_fullscreen;
};

#endif // ENGINE_H
