#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

Player::Player(){
    this->health = 0;
    this->maxHealth = 0;
    this->money = 0;
    this->experience = 0;
    this->abilityPoints = 0;
    this->level = 1;
    this->experienceToLevel = 10;
    this->gunId = 0;
    this->ammo = 0;
    this->fireTime = 0;
    this->attackDamage = 0;
    this->magazine = 0;
    this->maxMagazine = 0;
    this->reloadTime = 0;
    this->bulletSpeed = 0;
    this->meleeDamage = 0;
    this->dynamite = 0;
    this->autoFire = false;
    this->fireTimeHelper = 0;
    this->reloadTimeHelper = 0;
    this->fireTimeActive = false;
    this->reloadTimeActive = false;
    this->sprinting = false;
    this->stamina = 0;
    this->maxStamina = 0;
    this->maxMovementSpeed = 0;
    this->staminaRechargeActive = false;
    this->crippledHelper = 0;
    this->crippledActive = false;
    this->damagedHelper = 0;
    this->damaged = false;
    this->maxAttackDamage = 0;
    //this->soundId = NULL;
}

void Player::setHealth(int health, int maxHealth){
    this->health = health;
    this->maxHealth = maxHealth;
}

void Player::takeDamage(int damage){
    this->health -= damage;
    if(health <= 0){
        loseStatement = true;
        this->active = false;
        this->health = 0;
    }else{
        this->crippledActive = true;
        this->crippledHelper = 0;
        this->damaged = true;
    }
    al_play_sample(hurtSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
}

void Player::addMoney(int money){
    this->money += money;
}
void Player::addExperience(int experience){
    this->experience += experience;
    if(this->experience >= this->experienceToLevel){
        levelUp();
    }
}
void Player::levelUp(){
    this->abilityPoints++;
    this->experience -= this->experienceToLevel;
    this->experienceToLevel *= 4;
    this->level++;

    ParticleEntity *newParticle = new ParticleEntity();
    newParticle->setPos(this->centerX, this->posY);
    newParticle->setMovementSpeed(5);
    newParticle->setDrag(0.95);
    newParticle->setDuration(60);
    newParticle->setAngle(0);
    newParticle->setTypeId(4);
    addParticleToList(newParticle);

    al_play_sample(levelSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
}

void Player::setGun(int gunId, int fireTime, int attackDamage, int maxMagazine, int reloadTime, int bulletSpeed, bool autoFire, int maxAttackDamage){
    this->gunId = gunId;
    this->fireTime = fireTime;
    this->attackDamage = attackDamage;
    this->addAmmo(this->magazine);
    this->magazine = maxMagazine;
    this->maxMagazine = maxMagazine;
    this->reloadTime = reloadTime;
    this->bulletSpeed = bulletSpeed;
    this->autoFire = autoFire;
    this->maxAttackDamage = maxAttackDamage;
}

void Player::addAmmo(int ammo){
    this->ammo += ammo;
}

void Player::reload(){
    if(ammo > 0 && !this->reloadTimeActive){
        this->reloadTimeActive = true;
        al_play_sample(reloadSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_LOOP, &this->soundId);
    }
}

void Player::setMeleeDamage(int meleeDamage){
    this->meleeDamage = meleeDamage;
}

void Player::setStamina(float maxStamina){
    this->stamina = maxStamina;
    this->maxStamina = maxStamina;
}

void Player::setSprint(bool sprinting){
    if(this->sprinting && !sprinting){
        this->staminaRechargeActive = true;
        this->staminaRechargeHelper = 0;
    }
    this->sprinting = sprinting;
}

void Player::useDynamite(){
    this->dynamite--;
}

void Player::buyWeapon(int id1, int id2){
    if(this->money >= weaponArray[id1][id2][7]){
        this->money -= weaponArray[id1][id2][7];
        this->setGun(weaponArray[id1][id2][0], weaponArray[id1][id2][1], weaponArray[id1][id2][2], weaponArray[id1][id2][3], weaponArray[id1][id2][4], weaponArray[id1][id2][5], (weaponArray[id1][id2][6] == 1) ? true : false, weaponArray[id1][id2][8]);
        boughtWeapon = true;
        al_play_sample(buttonSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }else{
        al_play_sample(buttonNoSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }
}
void Player::levelSkill(int id1, int id2){
    if(this->abilityPoints > 0){
        this->abilityPoints--;
        this->setHealth(this->maxHealth*skillArray[id1][id2][0], this->maxHealth*skillArray[id1][id2][0]);
        this->setMeleeDamage(this->meleeDamage*skillArray[id1][id2][1]);
        this->setStamina(this->maxStamina*skillArray[id1][id2][2]);
        this->setMovementSpeed(this->maxMovementSpeed*skillArray[id1][id2][3]);
        leveledSkill = true;
        al_play_sample(buttonSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }else{
        al_play_sample(buttonNoSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }
}
void Player::buyConsumable(int id){
    if(id == 0){
        if(this->money >= 1){
            this->money--;
            this->addAmmo(10);
            al_play_sample(buttonSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }else{
            al_play_sample(buttonNoSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
    }else if(id == 1){
        if(this->money >= this->maxHealth-this->health){
            this->money -= this->maxHealth - this->health;
            this->health = this->maxHealth;
            al_play_sample(buttonSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }else{
            al_play_sample(buttonNoSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
    }else{
        if(this->money >= 5000){
            this->money -= 5000;
            this->dynamite++;
            al_play_sample(buttonSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }else{
            al_play_sample(buttonNoSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
    }
}

float Player::getMaxMovement(){
    return this->maxMovementSpeed;
}

bool Player::getAutoFire(){
    return this->autoFire;
}

float Player::getMovementSpeed(){
    return this->movementSpeed;
}

int Player::getStats(int id){
    switch(id){
        case 0:
            return this->health;
            break;

        case 1:
            return this->maxHealth;
            break;

        case 2:
            return this->money;
            break;

        case 3:
            return this->experience;
            break;

        case 4:
            return this->abilityPoints;
            break;

        case 5:
            return this->level;
            break;

        case 6:
            return this->experienceToLevel;
            break;

        case 7:
            return this->gunId;
            break;

        case 8:
            return this->ammo;
            break;

        case 9:
            return this->fireTime;
            break;

        case 10:
            return this->attackDamage;
            break;

        case 11:
            return this->magazine;
            break;

        case 12:
            return this->maxMagazine;
            break;

        case 13:
            return this->reloadTime;
            break;

        case 14:
            return this->bulletSpeed;
            break;

        case 15:
            return this->meleeDamage;
            break;

        case 16:
            return this->stamina;
            break;

        case 17:
            return this->maxStamina;
            break;

        case 18:
            return this->sprinting;
            break;

        case 19:
            return round(this->movementSpeed);
            break;

        case 20:
            return this->dynamite;
            break;

        case 21:
            return this->maxAttackDamage;
            break;

    }
    ///0 = health | 1 = maxHealth | 2  = money | 3 = experience | 4 = abilityPoints | 5 = level | 6 = experienceToLevel | 7 = gunId | 8 = ammo | 9 = fireTime | 10 = attackDamage | 11 = magazine | 12 = maxMagazine | 13 = reloadTime | 14 = meleeDamage
    return 0;
}

void Player::fireBullet(){
    if(!this->fireTimeActive && !this->reloadTimeActive && this->magazine > 0){
        float width = 6, height = 8;

        if(this->gunId == 0){

            PistolBullet *newMissile = new PistolBullet();
            newMissile->setPos(this->centerX, this->centerY);
            newMissile->setDimensions(width, height);
            newMissile->setAngle(-atan2(this->centerX - mouseX + newMissile->width/2, this->centerY - mouseY + newMissile->height/2));
            newMissile->setMovementSpeed(this->bulletSpeed);
            newMissile->setStats(this->attackDamage, this->maxAttackDamage);
            addMissileToList(newMissile);

            al_play_sample(pistolSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }else if(this->gunId == 1 || this->gunId == 3){

            float amountBullets = (this->gunId == 1) ? 5 : 3, bulletSpread = toRadians*15, angle;
            int spread;

            for(int i = 0; i < amountBullets; i++){
                //Shots Angle Calculation -- START
                angle = (-atan2(this->centerX - mouseX + width/2, this->centerY - mouseY + height/2)) + (-bulletSpread/2+(bulletSpread/(amountBullets-1))*i);

                spread = round(bulletSpread*100*2);

                angle += (-spread/2+rand()%spread)/200.0;

                //Shots Angle Calculation -- END

                PistolBullet *newMissile = new PistolBullet();
                newMissile->setPos(this->centerX, this->centerY);
                newMissile->setDimensions(width, height);
                newMissile->setAngle(angle);
                newMissile->setMovementSpeed(this->bulletSpeed);
                newMissile->setStats(this->attackDamage, this->maxAttackDamage);
                addMissileToList(newMissile);
            }
            al_play_sample(shotgunSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }else if(this->gunId == 2 || this->gunId == 5){
            float angle = -atan2(this->centerX - mouseX + width/2, this->centerY - mouseY + height/2);

            int spreadValue = (this->gunId == 2) ? 10 : 90;
            int spread = round(toRadians*spreadValue*100);

            angle += ((float)-spread/2+rand()%spread)/200.0;

            //Shots Angle Calculation -- END

            PistolBullet *newMissile = new PistolBullet();
            newMissile->setPos(this->centerX, this->centerY);
            newMissile->setDimensions(width, height);
            newMissile->setAngle(angle);
            newMissile->setMovementSpeed(this->bulletSpeed);
            newMissile->setStats(this->attackDamage, this->maxAttackDamage);
            addMissileToList(newMissile);

            al_play_sample(machineGunSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }else if(this->gunId == 4){
            PistolBullet *newMissile = new PistolBullet();
            newMissile->setPos(this->centerX, this->centerY);
            newMissile->setDimensions(width, height);
            newMissile->setAngle(-atan2(this->centerX - mouseX + newMissile->width/2, this->centerY - mouseY + newMissile->height/2));
            newMissile->setMovementSpeed(this->bulletSpeed);
            newMissile->setStats(this->attackDamage, this->maxAttackDamage);
            newMissile->setRocket(true, 48);
            addMissileToList(newMissile);

            al_play_sample(rocketLauncherSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }else if(this->gunId == 6){
            for(int lI = 0; lI < MAX_LIVING; lI++){
                if(livingList[lI] != NULL && livingList[lI]->checkActive()){
                    PistolBullet *newMissile = new PistolBullet();
                    newMissile->setPos(livingList[lI]->centerX, livingList[lI]->centerY);
                    newMissile->setDimensions(width, height);
                    newMissile->setMovementSpeed(0);
                    newMissile->setStats(this->attackDamage, this->maxAttackDamage);
                    newMissile->setRocket(true, 64);
                    newMissile->setQuakor(true);
                    addMissileToList(newMissile);
                }
            }
            al_play_sample(explosionSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }

        this->magazine--;
        this->fireTimeActive = true;
    }
}

void Player::update(){

    this->colX = false, this->colY = false;

    float loopI = ceil(this->movementSpeed/this->width);

    for(float i = 0; i < loopI && (!colX || !colY); i++){
        for(int lI = 0; lI < MAX_LIVING && (!colX || !colY); lI++){
            if(livingList[lI] != NULL && livingList[lI]->checkActive()){
                if(checkCollision(this->posX + this->deltaX/loopI, this->posY, livingList[lI]->posX, livingList[lI]->posY,
                    this->width, this->height, livingList[lI]->width, livingList[lI]->height)){
                    colX = true;
                }
                if(checkCollision(this->posX, this->posY + this->deltaY/loopI, livingList[lI]->posX, livingList[lI]->posY,
                    this->width, this->height, livingList[lI]->width, livingList[lI]->height)){
                    colY = true;
                }
            }
        }

        if(!isPassable(this->posX + this->deltaX/loopI, this->posY, this->width, this->height) && !this->colX){
            this->colX = true;
        }else if(!this->colX){
            this->posX += deltaX/loopI;
        }

        if(!isPassable(this->posX, this->posY + this->deltaY/loopI , this->width, this->height) && !this->colY){
            this->colY = true;
        }else if(!this->colY){
            this->posY += deltaY/loopI;
        }
    }

    if(this->rememberAnimationValue == 3 && this->untilFinished){
        this->angle = -atan2(this->centerX - mouseX, this->centerY - mouseY);
        this->deltaX = sin(this->angle)*16;
        this->deltaY = -cos(this->angle)*16;

        if(this->nextFrame && this->frameX == 2){
            for(int lI = 0; lI < MAX_LIVING && !colX; lI++){
                if(livingList[lI] != NULL && livingList[lI]->checkActive()){
                    if(checkCollision(this->posX+deltaX, this->posY+deltaY, livingList[lI]->posX, livingList[lI]->posY,
                        this->width, this->height, livingList[lI]->width, livingList[lI]->height)){

                        livingList[lI]->takeDamage(this->meleeDamage);

                        random = rand();

                        for(int i = 0; i < 3+random%3; i++){
                            random = rand();
                            ParticleEntity *newParticle = new ParticleEntity();
                            newParticle->setPos(livingList[lI]->centerX, livingList[lI]->centerY);
                            newParticle->setDimensions(2+random%7, 2+random%7);
                            newParticle->setMovementSpeed(2+random%4);
                            newParticle->setDrag(0.85 + random%10/(float)100);
                            newParticle->setDuration(30+random%91);
                            newParticle->setAngle(this->angle+(random%45)*toRadians);
                            newParticle->setTypeId(0);
                            addParticleToList(newParticle);
                        }

                        ParticleEntity *newParticle = new ParticleEntity();
                        newParticle->setPos(this->centerX, this->posY);
                        newParticle->setDimensions(0, 0);
                        newParticle->setMovementSpeed(2 + random%2);
                        newParticle->setDrag(0.9 + random%6/(float)100);
                        newParticle->setDuration(30 + random%16);
                        newParticle->setAngle((-22.5+random%46)*toRadians);
                        newParticle->setTypeId(5);
                        newParticle->setValue(this->meleeDamage);
                        addParticleToList(newParticle);
                    }
                }
            }
        }
    }

    if(this->reloadTimeHelper >= this->reloadTime){
        this->ammo += this->magazine;
        if(this->ammo > maxMagazine){
            this->magazine = this->maxMagazine;
            this->ammo -= this->maxMagazine;
        }
        else if(this->ammo > 0){
            this->magazine = this->ammo;
            this->ammo = 0;
        }
        this->reloadTimeHelper = 0;
        this->reloadTimeActive = false;

        al_stop_sample(&this->soundId);
    }

    if(this->fireTimeHelper >= this->fireTime){
        this->fireTimeActive = false;
        this->fireTimeHelper = 0;
    }

    if(this->reloadTimeActive){
        this->reloadTimeHelper++;
    }
    if(this->fireTimeActive){
        this->fireTimeHelper++;
    }

    if(this->sprinting){
        this->stamina -= (((this->deltaX < 0) ? -this->deltaX : this->deltaX) + ((this->deltaY < 0) ? -this->deltaY : this->deltaY));
        if(this->crippledActive){
            this->movementSpeed = this->maxMovementSpeed/2 + this->maxMovementSpeed/this->maxStamina*this->stamina + this->maxMovementSpeed/120*this->crippledHelper;
        }else{
            this->movementSpeed = this->maxMovementSpeed/2 + this->maxMovementSpeed/this->maxStamina*this->stamina*2;
        }
    }else if(this->stamina < 0){
        this->stamina = 0;
    }else if(this->stamina < this->maxStamina){
        if(this->staminaRechargeActive){
            this->staminaRechargeHelper++;
            if(!this->crippledActive){
                this->movementSpeed = this->maxMovementSpeed/4 + this->maxMovementSpeed/4/120*this->staminaRechargeHelper + this->maxMovementSpeed/2/this->maxStamina*this->stamina;
            }else if(this->crippledActive){
                this->movementSpeed = this->maxMovementSpeed/6 + this->maxMovementSpeed/6/120*this->crippledHelper + this->maxMovementSpeed/6/120*this->staminaRechargeHelper + this->maxMovementSpeed/2/this->maxStamina*this->stamina;
            }

            if(this->staminaRechargeHelper >= 120){
                this->staminaRechargeActive = false;
                this->staminaRechargeHelper = 0;
            }
        }else{
            this->stamina += round(this->maxStamina/10/60);
            if(this->crippledActive){
                this->movementSpeed = this->maxMovementSpeed/4 + this->maxMovementSpeed/4/120*this->crippledHelper + this->maxMovementSpeed/2/this->maxStamina*this->stamina;
            }else{
                this->movementSpeed = this->maxMovementSpeed/2 + this->maxMovementSpeed/2/this->maxStamina*this->stamina;
            }
        }
    }else if(this->crippledActive){
        this->movementSpeed = this->maxMovementSpeed/2 + this->maxMovementSpeed/2/120*this->crippledHelper;
    }

    printf("\n%f", this->maxMovementSpeed-this->movementSpeed);

    if(this->crippledActive){
        this->crippledHelper++;
        if(this->crippledHelper >= 120){
            this->crippledActive = false;
            this->crippledHelper = 0;
        }
    }
    if(this->damaged){
        this->damagedHelper++;
        if(this->damagedHelper >= 15){
            this->damaged = false;
            this->damagedHelper = 0;
        }
    }

    this->updateCenter();

    this->updateAnimation();

    this->deltaX = 0;
    this->deltaY = 0;
}

void Player::draw(){
    if(!this->damaged)
        al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width/2, this->posY+this->height/2, this->angle, NULL);
    else
        al_draw_tinted_rotated_bitmap(this->frameImage, al_map_rgba_f(1, 0, 0, 1), this->width/2, this->height/2, this->posX+this->width/2, this->posY+this->height/2, this->angle, NULL);

    if(this->reloadTimeActive){
        al_draw_rectangle(this->posX-1, this->posY-11, this->posX+this->width+1, this->posY-4, al_map_rgb(200,200,200), 2);
        al_draw_filled_rectangle(this->posX, this->posY-10, 2+(this->posX-2)+this->width/this->reloadTime*this->reloadTimeHelper, this->posY-5, al_map_rgb(80,30,0));
    }
}
