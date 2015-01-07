#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

PlayState PlayState::m_PlayState;

void PlayState::init(){

    //Create Player
    int width = 24, height = 24, movementSpeed = 2, sheetColums = 4, sheetRows = 3, animationSpeed = 60/movementSpeed/2;
    int health = 10, maxHealth = 10, money = 10, experience = 0, gunId = 0, fireTime = 20, attackDamage = 1, maxMagazine = 6, reloadTime = 120, ammo = 50, bulletSpeed = 8, meleeDamage = 1, maxStamina = 1000;
    bool autoFire = false;
    playerList[0] = new Player();
    playerList[0]->setPos(mapDisplayWidth/2-width/2, mapDisplayHeight/2-height/2);
    playerList[0]->setDimensions(width, height);
    playerList[0]->setMovementSpeed(movementSpeed);
    playerList[0]->setSheetDimensions(sheetColums, sheetRows, width, height);
    playerList[0]->setAnimationSpeed(animationSpeed);
    playerList[0]->setBitmap(playerImage);
    playerList[0]->setHealth(health, maxHealth);
    playerList[0]->addMoney(money);
    playerList[0]->addExperience(experience);
    playerList[0]->setGun(gunId, fireTime, attackDamage, maxMagazine, reloadTime, bulletSpeed, autoFire, attackDamage);
    playerList[0]->addAmmo(ammo);
    playerList[0]->setMeleeDamage(meleeDamage);
    playerList[0]->setStamina(maxStamina);

    boughtWeapon = true, leveledSkill = true;
    tierWeapon = 0, tierSkill = 0;

    enemySpawnRateHelper = 0, enemySpawnRate = 60, pauseBetweenWavesActive = true, pauseBetweenWavesHelper = 900, enemySpawnAll = false, amountEnemiesSpawned = 0, amountEnemiesSpawn = 10, enemySpeed = 1, enemyMoney = 1, enemyExperience = 1, enemyDamage = 1, enemyHealth = 1;
    winStatement = false, loseStatement = false, paused = false;
    wave = 0;

    //Pointer Lists +
    for(int i = 0; i < MAX_LIVING; i++){
        livingList[i] = NULL;
    }
    for(int i = 0; i < MAX_MISSILES; i++){
        missileList[i] = NULL;
    }
    for(int i = 0; i < MAX_DEBUG; i++){
        debugList[i] = NULL;
    }
    for(int i = 0; i < MAX_BUTTONS; i++){
        buttonList[i] = NULL;
    }
    for(int i = 0; i < MAX_PARTICLES; i++){
        particleList[i] = NULL;
    }

    for(int i = 0; i < 3; i++){
        float x = 878, y = mapDisplayHeight+i*24, width = 90, height = 30;
        x -= width/2;

        int buttonId = 40+i;

        Button *newButton = new Button();
        newButton->setPos(x, y);
        newButton->setDimensions(width, height);
        newButton->setTypeId(buttonId);

        addButtonToList(newButton);
    }
    //Pointer Lists -

    loadMapArray();

    al_stop_samples();
}

void PlayState::cleanup(){
}

void PlayState::pause(){
    inGame = true;
}

void PlayState::resume(){
}

void PlayState::update(Engine* engine){

    al_get_mouse_state(&mouseState);

    al_get_keyboard_state(&keyState);

    //Player Input +
    float movementSpeedP = playerList[0]->movementSpeed;

    playerList[0]->animationValue = 0;

    int horizontal = 0, vertical = 0;

    if(al_key_down(&keyState, ALLEGRO_KEY_W)){
        playerList[0]->deltaY = -movementSpeedP;
        playerList[0]->setAnimationValue(1, false, playerList[0]->getMovementSpeed()/2);
        vertical = 1;
    }else if(al_key_down(&keyState, ALLEGRO_KEY_S)){
        playerList[0]->deltaY = movementSpeedP;
        playerList[0]->setAnimationValue(1, false, playerList[0]->getMovementSpeed()/2);
        vertical = 2;
    }
    if(al_key_down(&keyState, ALLEGRO_KEY_A)){
        playerList[0]->deltaX = -movementSpeedP;
        playerList[0]->setAnimationValue(1, false, playerList[0]->getMovementSpeed()/2);
        horizontal = 1;
    }else if(al_key_down(&keyState, ALLEGRO_KEY_D)){
        playerList[0]->deltaX = movementSpeedP;
        playerList[0]->setAnimationValue(1, false, playerList[0]->getMovementSpeed()/2);
        horizontal = 2;
    }

    if(vertical != 0 && horizontal != 0){
        playerList[0]->deltaX /= sqrt(2);
        playerList[0]->deltaY /= sqrt(2);
    }

    if(al_key_down(&keyState, ALLEGRO_KEY_LSHIFT) && playerList[0]->getStats(16) > 0){
        playerList[0]->setSprint(true);
    }else{
        playerList[0]->setSprint(false);
    }

    if(!mouseButtonLeft && !mouseButtonLeftClick && !paused){
        if(vertical > 0){
            if(horizontal == 0){
                playerList[0]->angle = (vertical-1)*180*toRadians;
            }else{
                if(vertical == 1){
                    playerList[0]->angle = (horizontal == 1) ? 315*toRadians : 45*toRadians;
                }else if(vertical == 2){
                    playerList[0]->angle = (horizontal == 1) ? 225*toRadians : 135*toRadians;
                }
            }
        }else if(horizontal > 0){
            playerList[0]->angle = 270*toRadians+(horizontal-1)*180*toRadians;
        }
    }else if(mouseY <= mapDisplayHeight && !paused){
        playerList[0]->angle = -atan2(playerList[0]->centerX - mouseX, playerList[0]->centerY - mouseY);
        playerList[0]->setAnimationValue(2, false, 1);
        if(mouseButtonLeftClick || (playerList[0]->getAutoFire() && mouseButtonLeft))
            playerList[0]->fireBullet();
    }

    if(al_key_down(&keyState, ALLEGRO_KEY_F5)){
        if(lastKeyPress != ALLEGRO_KEY_F5){
            loadMapArray();
            lastKeyPress = ALLEGRO_KEY_F5;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
        if(lastKeyPress != ALLEGRO_KEY_ESCAPE){
            //engine->pushState(MenuState::instance());
            engine->changeState(PlayState::instance());
            lastKeyPress = ALLEGRO_KEY_ESCAPE;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_F)){
        if(lastKeyPress != ALLEGRO_KEY_F){

            playerList[0]->setAnimationValue(3, true, 1);

            lastKeyPress = ALLEGRO_KEY_F;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_R)){
        if(lastKeyPress != ALLEGRO_KEY_R){

            playerList[0]->reload();

            lastKeyPress = ALLEGRO_KEY_R;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_G)){
        if(lastKeyPress != ALLEGRO_KEY_G){
            if(playerList[0]->getStats(20) > 0){
                playerList[0]->useDynamite();

                PistolBullet *newMissile = new PistolBullet();
                newMissile->setPos(playerList[0]->posX+playerList[0]->width/2, playerList[0]->posY+playerList[0]->height/2);
                newMissile->setDimensions(6, 6);
                newMissile->setMovementSpeed(0);
                newMissile->setSheetDimensions(3, 1, 6, 6);
                newMissile->setAnimationSpeed(1);
                newMissile->setBitmap(bulletImage);
                newMissile->setStats(1000, 1000);
                newMissile->setDynamite(true);
                addMissileToList(newMissile);
            }

            lastKeyPress = ALLEGRO_KEY_G;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_P)){
        if(lastKeyPress != ALLEGRO_KEY_P){
            paused = !paused;

            lastKeyPress = ALLEGRO_KEY_P;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_N)){
        if(lastKeyPress != ALLEGRO_KEY_N && pauseBetweenWavesActive){
            pauseBetweenWavesHelper = 1200;

            lastKeyPress = ALLEGRO_KEY_N;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_COMMA)){
        if(lastKeyPress != ALLEGRO_KEY_COMMA){
            if(volumeLevel > 0)
                volumeLevel -= 0.1;

            lastKeyPress = ALLEGRO_KEY_COMMA;
        }
    }else if(al_key_down(&keyState, ALLEGRO_KEY_FULLSTOP)){
        if(lastKeyPress != ALLEGRO_KEY_FULLSTOP){
            if(volumeLevel < 1)
                volumeLevel += 0.1;

            lastKeyPress = ALLEGRO_KEY_FULLSTOP;
        }
    }
    //Player Input -
    if(enemySpawnRateHelper >= enemySpawnRate && !pauseBetweenWavesActive && amountEnemiesSpawned < amountEnemiesSpawn && !paused){

        float x = 0, y = 0, width = 0, height = 0;
        int randomSpawnpointHelper = 0, random = rand();
        bool active = true;

        int amountSpawnPoints = 0;
        for(int lx = 0; lx < mapArrayWidth; lx++){
            for(int ly = 0; ly < mapArrayHeight; ly++){
                if(mapArray[lx][ly] == 4 || mapArray[lx][ly] == 5){
                    amountSpawnPoints++;
                }
            }
        }

        int loopValue = (enemySpawnAll) ? amountSpawnPoints : 1;
        for(int i = 0; i < loopValue; i++){
            width = 24, height = 24, active = true;
            randomSpawnpointHelper = 0, random = rand();

            for(int tx = 0; tx < mapArrayWidth; tx++){
                for(int ty = 0; ty < mapArrayHeight; ty++){
                    if(mapArray[tx][ty] == 4 || mapArray[tx][ty] == 5){
                        if(random % amountSpawnPoints == randomSpawnpointHelper){
                            do{
                                x = (tx + (1 - rand() % 3))*tileSize;

                                y = (ty + (1 - rand() % 3))*tileSize;
                            }while(!isPassable(x, y, width, height));
                        }
                        randomSpawnpointHelper++;
                    }
                }
            }

            for(int i = 0; i < MAX_LIVING; i++){
                if(livingList[i] != NULL && livingList[i]->checkActive()){
                    if(checkCollision(x, y, livingList[i]->posX, livingList[i]->posY, width, height, livingList[i]->width, livingList[i]->height)){
                        active = false;
                    }
                }
            }

            if(checkCollision(x, y, playerList[0]->posX, playerList[0]->posY, width, height, playerList[0]->width, playerList[0]->height)){
                active = false;
            }
            if(active){
                float sheetColums = 4, sheetRows = 3, animationSpeed = 60/enemySpeed;
                Zombie *newZombie = new Zombie();
                newZombie->setPos(x, y);
                newZombie->setDimensions(width, height);
                newZombie->setMovementSpeed(enemySpeed);
                newZombie->setSheetDimensions(sheetColums, sheetRows, width, height);
                newZombie->setAnimationSpeed(animationSpeed);
                newZombie->setBitmap(zombieImage);
                newZombie->setStats(enemyHealth + rand()%(1+(int)ceil((float)enemyHealth/3)), enemyDamage, enemyMoney + rand()%(1+(int)ceil((float)enemyMoney/5)), enemyExperience + rand()%(1+(int)ceil((float)enemyExperience/5)));
                addLivingToList(newZombie);
                enemySpawnRateHelper = 0;
            }
        }
        if(active){
            amountEnemiesSpawned += 1;
        }
    }else if(amountEnemiesSpawned >= amountEnemiesSpawn && !pauseBetweenWavesActive){
        pauseBetweenWavesActive = true;
        if(wave % 10 == 0){
            enemySpeed = ceil(enemySpeed*1.5);
        }
        if(wave % 5 == 0){
            enemyMoney *= 2;
            enemyExperience *= 2;
        }

        if(wave % 5 == 0){
            enemyDamage += 1;
        }
        if(wave != 1 && wave != 2 && wave != 3){
            enemyHealth = wave/4+pow(1.1, wave);
        }
        if(wave % 2 == 0 && enemySpawnRate > 2){
            enemySpawnRate = floor(enemySpawnRate/1.3);
        }else if(enemySpawnRate <= 2 && !enemySpawnAll){
            enemySpawnAll = true;
            int amountSpawnPoints = 0;
            for(int lx = 0; lx < mapArrayWidth; lx++){
                for(int ly = 0; ly < mapArrayHeight; ly++){
                    if(mapArray[lx][ly] == 4 || mapArray[lx][ly] == 5){
                        amountSpawnPoints++;
                    }
                }
            }
            enemySpawnRate /= (amountSpawnPoints*2);
            amountEnemiesSpawn /= amountSpawnPoints;
        }else if(enemySpawnAll && enemySpawnRate > 1 && wave % 2 == 0){
            enemySpawnRate = floor(enemySpawnRate/1.25);
        }
        amountEnemiesSpawn = ceil(amountEnemiesSpawn*1.05);
        amountEnemiesSpawned = 0;
    }else if(pauseBetweenWavesActive){
        pauseBetweenWavesHelper++;
        if(pauseBetweenWavesHelper >= 1200){
            pauseBetweenWavesActive = false;
            pauseBetweenWavesHelper = 0;
            wave += 1;
        }
    }


    //Update Entities +
    if(!paused){
        for(int i = 0; i < MAX_PARTICLES; i++){
            if(particleList[i] != NULL && particleList[i]->checkActive()){
                particleList[i]->update();
            }
        }
        for(int i = 0; i < MAX_PLAYERS; i++){
            if(playerList[i] != NULL && playerList[i]->checkActive()){
                playerList[i]->update();
            }
        }
        for(int i = 0; i < MAX_LIVING; i++){
            if(livingList[i] != NULL && livingList[i]->checkActive()){
                livingList[i]->update();
            }
        }
        for(int i = 0; i < MAX_MISSILES; i++){
            if(missileList[i] != NULL && missileList[i]->checkActive()){
                missileList[i]->update();
            }
        }
        int amountSpawnPoints = 0;
        for(int lx = 0; lx < mapArrayWidth; lx++){
            for(int ly = 0; ly < mapArrayHeight; ly++){
                if(mapArray[lx][ly] == 4){
                    amountSpawnPoints++;
                }
            }
        }
        if(amountSpawnPoints == 0){
            winStatement = true;
            enemySpawnRate = 9999999;
        }
        /*for(int i = 0; i < MAX_DEBUG; i++){
            if(debugList[i] != NULL && debugList[i]->checkActive()){
                debugList[i]->update();
            }
        }*/
    }
    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] != NULL && buttonList[i]->checkActive()){
            buttonList[i]->update();
            if(buttonList[i]->clicked){
                switch(buttonList[i]->typeId){
                    case 3:
                        playerList[0]->buyWeapon(0,0);
                        break;
                    case 4:
                        playerList[0]->buyWeapon(0,1);
                        break;
                    case 5:
                        playerList[0]->buyWeapon(0,2);
                        break;
                    case 6:
                        playerList[0]->buyWeapon(1,0);
                        break;
                    case 7:
                        playerList[0]->buyWeapon(1,1);
                        break;
                    case 8:
                        playerList[0]->buyWeapon(1,2);
                        break;
                    case 9:
                        playerList[0]->buyWeapon(2,0);
                        break;
                    case 10:
                        playerList[0]->buyWeapon(2,1);
                        break;
                    case 11:
                        playerList[0]->buyWeapon(2,2);
                        break;
                    case 12:
                        playerList[0]->buyWeapon(3,0);
                        break;
                    case 13:
                        playerList[0]->buyWeapon(3,1);
                        break;
                    case 14:
                        playerList[0]->buyWeapon(3,2);
                        break;
                    case 15:
                        playerList[0]->buyWeapon(4,0);
                        break;
                    case 16:
                        playerList[0]->buyWeapon(4,1);
                        break;
                    case 17:
                        playerList[0]->buyWeapon(4,2);
                        break;
                    case 18:
                        playerList[0]->levelSkill(0,0);
                        break;
                    case 19:
                        playerList[0]->levelSkill(0,1);
                        break;
                    case 20:
                        playerList[0]->levelSkill(0,2);
                        break;
                    case 21:
                        playerList[0]->levelSkill(1,0);
                        break;
                    case 22:
                        playerList[0]->levelSkill(1,1);
                        break;
                    case 23:
                        playerList[0]->levelSkill(1,2);
                        break;
                    case 24:
                        playerList[0]->levelSkill(2,0);
                        break;
                    case 25:
                        playerList[0]->levelSkill(2,1);
                        break;
                    case 26:
                        playerList[0]->levelSkill(2,2);
                        break;
                    case 27:
                        playerList[0]->levelSkill(3,0);
                        break;
                    case 28:
                        playerList[0]->levelSkill(3,1);
                        break;
                    case 29:
                        playerList[0]->levelSkill(3,2);
                        break;
                    case 30:
                        playerList[0]->levelSkill(4,0);
                        break;
                    case 31:
                        playerList[0]->levelSkill(4,1);
                        break;
                    case 32:
                        playerList[0]->levelSkill(4,2);
                        break;
                    case 40:
                        playerList[0]->buyConsumable(0);
                        break;
                    case 41:
                        playerList[0]->buyConsumable(1);
                        break;
                    case 42:
                        playerList[0]->buyConsumable(2);
                        break;
                }
            }
        }
    }
    if((leveledSkill && tierSkill < 6) || (boughtWeapon && tierWeapon < 6)){
        if(boughtWeapon){
            tierWeapon++;
            boughtWeapon = false;
            for(int i = 0; i < MAX_BUTTONS; i++){
                if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                    if(buttonList[i]->typeId > 2 && buttonList[i]->typeId < 18){
                        buttonList[i] = NULL;
                    }
                }
            }
            for(int i = 0; i < 3; i++){
                float x = 585+i*100, y = mapDisplayHeight+25, width = 100, height = 48;
                x -= width/2;

                int buttonId = 3*tierWeapon+i;

                Button *newButton = new Button();
                newButton->setPos(x, y);
                newButton->setDimensions(width, height);
                newButton->setTypeId(buttonId);

                addButtonToList(newButton);
            }
        }else if(leveledSkill){
            tierSkill++;
            leveledSkill = false;
            for(int i = 0; i < MAX_BUTTONS; i++){
                if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                    if(buttonList[i]->typeId > 17 && buttonList[i]->typeId < 40){
                        buttonList[i] = NULL;
                    }
                }
            }
            for(int i = 0; i < 3; i++){
                float x = 970+i*100, y = mapDisplayHeight+25, width = 100, height = 48;
                x -= width/2;

                int buttonId = 3*tierSkill+i+15;

                Button *newButton = new Button();
                newButton->setPos(x, y);
                newButton->setDimensions(width, height);
                newButton->setTypeId(buttonId);

                addButtonToList(newButton);
            }
        }
    }else if(tierWeapon == 6){
        tierWeapon++;
        boughtWeapon = false;
        for(int i = 0; i < MAX_BUTTONS; i++){
            if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                if(buttonList[i]->typeId > 2 && buttonList[i]->typeId < 21 && buttonList[i]->posX < 800){
                    buttonList[i] = NULL;
                }
            }
        }
    }else if(tierSkill == 6){
        tierSkill++;
        leveledSkill = false;
        for(int i = 0; i < MAX_BUTTONS; i++){
            if(buttonList[i] != NULL && buttonList[i]->checkActive()){
                if(buttonList[i]->typeId > 17 && buttonList[i]->typeId < 40){
                    buttonList[i] = NULL;
                }
            }
        }
    }
    //Update Entities -

    //Rest +
    if(!pauseBetweenWavesActive && !paused)
        enemySpawnRateHelper++;
    //Rest -

}

void PlayState::draw(Engine* engine){
    //Draw Map+
    drawMap();
    //Draw Map-

    //Draw Entities +
    for(int i = 0; i < MAX_PARTICLES; i++){
        if(particleList[i] != NULL && particleList[i]->checkActive()){
            if(particleList[i]->typeId < 2)
                particleList[i]->draw();
        }
    }
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(playerList[i] != NULL && playerList[i]->checkActive()){
            playerList[i]->draw();
        }
    }
    for(int i = 0; i < MAX_LIVING; i++){
        if(livingList[i] != NULL && livingList[i]->checkActive()){
            livingList[i]->draw();
        }
    }
    for(int i = 0; i < MAX_MISSILES; i++){
        if(missileList[i] != NULL && missileList[i]->checkActive()){
            missileList[i]->draw();
        }
    }
    for(int i = 0; i < MAX_PARTICLES; i++){
        if(particleList[i] != NULL && particleList[i]->checkActive()){
            if(particleList[i]->typeId >= 2)
                particleList[i]->draw();
        }
    }
    for(int i = 0; i < MAX_DEBUG; i++){
        if(debugList[i] != NULL && debugList[i]->checkActive()){
            debugList[i]->draw();
        }
    }
    //Draw Entities -

    //Draw GUI +
    al_draw_filled_rectangle(0, mapDisplayHeight, screenWidth, screenHeight, al_map_rgb(0,0,0));


    al_draw_rectangle(1, mapDisplayHeight+1, 151, mapDisplayHeight-1+(screenHeight-mapDisplayHeight)/2, al_map_rgb(200,200,200), 2);
    al_draw_rectangle(1, mapDisplayHeight+1+(screenHeight-mapDisplayHeight)/2, 151, mapDisplayHeight-1+(screenHeight-mapDisplayHeight), al_map_rgb(200,200,200), 2);

    al_draw_filled_rectangle(2, mapDisplayHeight+2, 2+148.0/playerList[0]->getStats(1)*playerList[0]->getStats(0), mapDisplayHeight-2+(screenHeight-mapDisplayHeight)/2, al_map_rgb(200,0,0));
    al_draw_filled_rectangle(2, mapDisplayHeight+2+(screenHeight-mapDisplayHeight)/2, 2+148.0/(float)playerList[0]->getStats(17)*playerList[0]->getStats(16), mapDisplayHeight-2+(screenHeight-mapDisplayHeight), al_map_rgb(0,200,0));

    al_draw_textf(defaultFont, al_map_rgb(50, 50, 50), 75, mapDisplayHeight+(screenHeight-mapDisplayHeight)/8, ALLEGRO_ALIGN_CENTER, "%d/%d", (int)round(playerList[0]->getStats(0)), (int)round(playerList[0]->getStats(1)));
    al_draw_textf(defaultFont, al_map_rgb(50, 50, 50), 75, screenHeight-(screenHeight-mapDisplayHeight)/2.5, ALLEGRO_ALIGN_CENTER, "%d/%d", (int)round(playerList[0]->getStats(16)/100), (int)round(playerList[0]->getStats(17)/100));

    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 160, mapDisplayHeight, NULL, "%d$", playerList[0]->getStats(2));
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 160, mapDisplayHeight+15, NULL, "%d XP LEFT", playerList[0]->getStats(6) - playerList[0]->getStats(3));
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 160, mapDisplayHeight+30, NULL, "%d/%d MGZ", playerList[0]->getStats(11), playerList[0]->getStats(12)); if(playerList[0]->getStats(11) <= 0) al_draw_textf(smallFont, al_map_rgb(150, 50, 50), 152, mapDisplayHeight+30, NULL, "!!"); else if(playerList[0]->getStats(11) <= playerList[0]->getStats(12)/4) al_draw_textf(smallFont, al_map_rgb(255, 150, 0), 152, mapDisplayHeight+30, NULL, "!!");
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 160, mapDisplayHeight+45, NULL, "%d AMMO", playerList[0]->getStats(8)); if(playerList[0]->getStats(8) <= 0) al_draw_textf(smallFont, al_map_rgb(150, 50, 50), 152, mapDisplayHeight+45, NULL, "!!"); else if(playerList[0]->getStats(8) <= playerList[0]->getStats(12)) al_draw_textf(smallFont, al_map_rgb(255, 150, 0), 152, mapDisplayHeight+45, NULL, "!!");
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 160, mapDisplayHeight+60, NULL, "%d WAVE", wave);

    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 300, mapDisplayHeight, NULL, "%d LVL", playerList[0]->getStats(5));
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 300, mapDisplayHeight+15, NULL, "%d AP", playerList[0]->getStats(4));
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 300, mapDisplayHeight+30, NULL, "%d MVSPD", (int)round(playerList[0]->movementSpeed));
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 300, mapDisplayHeight+45, NULL, "%d MLEE DMG", playerList[0]->getStats(15));
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 300, mapDisplayHeight+60, NULL, "%d DYNMT", playerList[0]->getStats(20));

    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 400, mapDisplayHeight, NULL, "TYPE: %s", (playerList[0]->getStats(7) == 0) ? "Gun" : (playerList[0]->getStats(7) == 1) ? "5-Shotgun" : (playerList[0]->getStats(7) == 2) ? "Machinegun" : (playerList[0]->getStats(7) == 3) ? "3-Shotgun" : (playerList[0]->getStats(7) == 4) ? "Rocket launcher" : (playerList[0]->getStats(7) == 5) ? "Minigun" : (playerList[0]->getStats(7) == 6) ? "Quakor" : "Error");
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 400, mapDisplayHeight+15, NULL, "%dB/s", (int)round((float)60/playerList[0]->getStats(9)));
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 400, mapDisplayHeight+30, NULL, "%d-%d DAM", playerList[0]->getStats(10), playerList[0]->getStats(21));
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 400, mapDisplayHeight+45, NULL, "%fs RLD", (float)playerList[0]->getStats(13)/(float)60);
    al_draw_textf(smallFont, al_map_rgb(50, 150, 50), 400, mapDisplayHeight+60, NULL, "%d BLTSPD", playerList[0]->getStats(14));
    ///0 = health | 1 = maxHealth | 2  = money | 3 = experience | 4 = abilityPoints | 5 = level | 6 = experienceToLevel | 7 = gunId | 8 = ammo | 9 = fireTime | 10 = attackDamage | 11 = magazine | 12 = maxMagazine | 13 = reloadTime | 14 = meleeDamage
    al_draw_textf(defaultFont, al_map_rgb(150, 150, 150), 600, mapDisplayHeight, NULL, "Weapon Shop");
    al_draw_textf(defaultFont, al_map_rgb(150, 150, 150), 1000, mapDisplayHeight, NULL, "Skill Stick");
    al_draw_textf(smallFont, al_map_rgb(150, 150, 150), screenWidth-60, screenHeight-23, NULL, "v. %s", versionNumber);
    al_draw_textf(smallFont, al_map_rgb(150, 150, 150), screenWidth-60, screenHeight-40, NULL, "vol %d", (int)round(volumeLevel*100));
    if(winStatement){
        al_draw_textf(bigFont, al_map_rgb(0, 0, 0), mapDisplayWidth/2, mapDisplayHeight/2-36, ALLEGRO_ALIGN_CENTER, "YOU WON");
        al_draw_textf(defaultFont, al_map_rgb(0, 0, 0), mapDisplayWidth/2, mapDisplayHeight/2+12, ALLEGRO_ALIGN_CENTER, "Press escape to restart");
    }else if(loseStatement){
        al_draw_textf(bigFont, al_map_rgb(0, 0, 0), mapDisplayWidth/2, mapDisplayHeight/2-36, ALLEGRO_ALIGN_CENTER, "YOU LOST");
        al_draw_textf(defaultFont, al_map_rgb(0, 0, 0), mapDisplayWidth/2, mapDisplayHeight/2+12, ALLEGRO_ALIGN_CENTER, "Press escape to restart");
    }else if(paused){
        al_draw_textf(bigFont, al_map_rgb(0, 0, 0), mapDisplayWidth/2, mapDisplayHeight/2-36, ALLEGRO_ALIGN_CENTER, "PAUSED");
        al_draw_textf(defaultFont, al_map_rgb(0, 0, 0), mapDisplayWidth/2, mapDisplayHeight/2+12, ALLEGRO_ALIGN_CENTER, "Press P to unpause");
    }
    fpsTimeNew = al_get_time();
    fpsCounter = 1/(fpsTimeNew - fpsTimeOld);
    fpsTimeOld = fpsTimeNew;
    al_draw_textf(defaultFont, (fpsCounter > 55) ? al_map_rgb(50, 150, 50) : (fpsCounter <= 55 && fpsCounter > 30) ? al_map_rgb(150, 150, 50) : al_map_rgb(150, 50, 50), screenWidth-95, mapDisplayHeight, NULL, "FPS: %d", (int)round(fpsCounter));
    int hover[3] = {0,0,0};
    for(int i = 0; i < MAX_BUTTONS; i++){
        if(buttonList[i] != NULL && buttonList[i]->checkActive()){
            buttonList[i]->draw();
            if(buttonList[i]->selected && buttonList[i]->typeId > 2 && buttonList[i]->typeId < 18){
                hover[0] = i;
                hover[1] = floor((buttonList[i]->typeId-3)/3);
                hover[2] = (buttonList[i]->typeId-3)%3;
            }
        }
    }
    if(hover[0] != 0){
        al_draw_rectangle(mouseX, mouseY-160, mouseX+270, mouseY, al_map_rgb(255,255,255), 2);
        al_draw_filled_rectangle(mouseX, mouseY-159, mouseX+269, mouseY, al_map_rgb(0,0,0));

        al_draw_textf(smallFont, al_map_rgb(150, 150, 150), mouseX+5, mouseY-155, NULL, "Weapon Type: %s", (weaponArray[hover[1]][hover[2]][0] == 0) ? "Gun" : (weaponArray[hover[1]][hover[2]][0] == 1) ? "5-Shotgun" : (weaponArray[hover[1]][hover[2]][0] == 2) ? "Machinegun" : (weaponArray[hover[1]][hover[2]][0] == 3) ? "3-Shotgun" : (weaponArray[hover[1]][hover[2]][0] == 4) ? "Rocket launcher" : (weaponArray[hover[1]][hover[2]][0] == 5) ? "Minigun" : (weaponArray[hover[1]][hover[2]][0] == 6) ? "Quakor" : "Error");
        al_draw_textf(smallFont, al_map_rgb(150, 150, 150), mouseX+5, mouseY-135, NULL, "Weapon Bullets/s: %dB/s", (int)round((float)60/weaponArray[hover[1]][hover[2]][1]));
        al_draw_textf(smallFont, al_map_rgb(150, 150, 150), mouseX+5, mouseY-115, NULL, "Weapon Damage: %d-%d", weaponArray[hover[1]][hover[2]][2], weaponArray[hover[1]][hover[2]][8]);
        al_draw_textf(smallFont, al_map_rgb(150, 150, 150), mouseX+5, mouseY-95, NULL, "Weapon Magazine: %d", weaponArray[hover[1]][hover[2]][3]);
        al_draw_textf(smallFont, al_map_rgb(150, 150, 150), mouseX+5, mouseY-75, NULL, "Weapon Reload Time: %fs", (float)weaponArray[hover[1]][hover[2]][4]/(float)60);
        al_draw_textf(smallFont, al_map_rgb(150, 150, 150), mouseX+5, mouseY-55, NULL, "Weapon Bullet Speed: %d", weaponArray[hover[1]][hover[2]][5]);
        al_draw_textf(smallFont, al_map_rgb(150, 150, 150), mouseX+5, mouseY-35, NULL, "Weapon Auto Fire: %s", (weaponArray[hover[1]][hover[2]][6]) ? "Yes" : "No");
        al_draw_textf(smallFont, al_map_rgb(150, 150, 150), mouseX+5, mouseY-15, NULL, "Weapon Cost: %d$", weaponArray[hover[1]][hover[2]][7]);
    }
    //Draw GUI -
}
