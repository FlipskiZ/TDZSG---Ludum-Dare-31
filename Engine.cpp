#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

void Engine::init(const char* title, int width, int height, bool fullscreen){

	// initialize ALLEGRO
	al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_acodec_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();

    if(fullscreen){
		al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	}else{
        al_set_new_display_flags(ALLEGRO_WINDOWED);
	}
    display = al_create_display(screenWidth, screenHeight);
    al_set_window_title(display, title);

    bigFont = al_load_font("fonts/pixelFont.ttf", 48, 0);
    defaultFont = al_load_font("fonts/pixelFont.ttf", 24, 0);
    smallFont = al_load_font("fonts/pixelFont.ttf", 16, 0);

    cursorImage = al_load_bitmap("graphics/cursorImage.png");
    playerImage = al_load_bitmap("graphics/playerImage.png");
    bulletImage = al_load_bitmap("graphics/bulletImage.png");
    pixelImage = al_load_bitmap("graphics/purplePixel.png");
    zombieImage = al_load_bitmap("graphics/zombieImage.png");
    explosionImage = al_load_bitmap("graphics/explosionImage.png");

    groundImage1 = al_load_bitmap("graphics/groundImage1.png");
    groundImage2 = al_load_bitmap("graphics/groundImage2.png");
    wallImage = al_load_bitmap("graphics/wallImage.png");
    brokenWallImage = al_load_bitmap("graphics/brokenWallImage.png");
    portalImage1 = al_load_bitmap("graphics/portalImage1.png");
    portalImage2 = al_load_bitmap("graphics/portalImage2.png");
    floorImage = al_load_bitmap("graphics/floorImage.png");
    brokenFloorImage = al_load_bitmap("graphics/brokenFloorImage.png");
    bloodGroundImage1 = al_load_bitmap("graphics/bloodGroundImage1.png");
    bloodGroundImage2 = al_load_bitmap("graphics/bloodGroundImage2.png");
    vineWallImage = al_load_bitmap("graphics/vineWallImage.png");

    pistolSound = al_load_sample("sfx/pistolSound.wav");
    machineGunSound = al_load_sample("sfx/machineGunSound.wav");
    shotgunSound = al_load_sample("sfx/shotgunSound.wav");
    rocketLauncherSound = al_load_sample("sfx/rocketLauncherSound.wav");
    explosionSound = al_load_sample("sfx/explosionSound.wav");
    reloadSound = al_load_sample("sfx/reloadSound.wav");
    hurtSound = al_load_sample("sfx/hurtSound.wav");
    killSound = al_load_sample("sfx/killSound.wav");
    levelSound = al_load_sample("sfx/levelSound.wav");
    buttonSound = al_load_sample("sfx/buttonSound.wav");
    buttonNoSound = al_load_sample("sfx/buttonNoSound.wav");

    al_reserve_samples(11);

    event_queue = al_create_event_queue();
    timer = al_create_timer(1/FPS);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_hide_mouse_cursor(display);

	m_fullscreen = fullscreen;
    m_running = true;

    //Pointer Lists +
    for(int i = 0; i < MAX_BUTTONS; i++){
        buttonList[i] = NULL;
    }
    //Pointer Lists -

    //Variables +
    versionNumber = "1.2.5";

    fpsTimeNew = 0, fpsCounter = 0, fpsTimeOld = 0;

    drawScreen = false, timerEvent = false, done = false, mouseButtonLeft = false, mouseButtonLeftClick = false, mouseButtonRight = false, mouseButtonRightClick = false, inGame = false;
    mouseX = 0, mouseY = 0, volumeLevel = 1;
    lastKeyPress = 0, mouseWheel = 0;

    boughtWeapon = false, leveledSkill = false;
    tierWeapon = 0, tierSkill = 0;

    enemySpawnRateHelper = 0, enemySpawnRate = 0, pauseBetweenWavesActive = false, enemySpawnAll = false, pauseBetweenWavesHelper = 0, amountEnemiesSpawned = 0, amountEnemiesSpawn = 0, enemySpeed = 0, enemyMoney = 0, enemyExperience = 0, enemyDamage = 0, enemyHealth = 0;
    winStatement =  false, loseStatement = false, paused = false;
    wave = 0;

    loadMapArray();

    al_start_timer(timer);
}

void Engine::cleanup(){
	// cleanup the all states

	while(!states.empty()){
		states.back()->cleanup();
		states.pop_back();
	}

	// switch back to windowed mode so other
	// programs won't get accidentally resized
	if(m_fullscreen){
		al_set_new_display_flags(ALLEGRO_WINDOWED);
	}

    al_destroy_font(defaultFont);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
}

void Engine::changeState(GameState* state) {
	// cleanup the current state
	if(!states.empty()){
		states.back()->cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->cleanup();
	states.back()->init();
}

void Engine::pushState(GameState* state){
	// pause current state
	if (!states.empty()){
		states.back()->pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->init();
}

void Engine::popState(){
	// cleanup the current state
	if (!states.empty()){
		states.back()->cleanup();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty()){
		states.back()->resume();
	}
}

void Engine::update(){
	// let the state update the game
	states.back()->update(this);
}

void Engine::draw(){
	// let the state update the game
	drawScreen = true;
	states.back()->draw(this);
}
