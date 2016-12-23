#include "Engine.h"
#include "Zombie.h"

Zombie::Zombie()
{
    this->animationValue = 0;
    this->health = 0;
    this->attackDamage = 0;
    this->worthMoney = 0;
    this->worthExperience = 0;
    this->damagedHelper = 0;
    this->damaged = false;
}

void Zombie::setStats(int health, int attackDamage, int worthMoney, int worthExperience){
    this->health = health;
    this->maxHealth = health;
    this->attackDamage = attackDamage;
    this->worthMoney = worthMoney;
    this->worthExperience = worthExperience;
}

void Zombie::takeDamage(int damage){
    this->health -= damage;
    if(this->health <= 0){
        this->active = false; //Death animation maybe?
        playerList[0]->addMoney(this->worthMoney);
        playerList[0]->addExperience(this->worthExperience);

        for(int i = 0; i < 2; i++){
            this->random = rand();
            ParticleEntity *newParticle = new ParticleEntity();
            newParticle->setPos(playerList[0]->centerX, playerList[0]->posY-20+20*i);
            newParticle->setDimensions(0, 0);
            newParticle->setMovementSpeed(2 + random%2);
            newParticle->setDrag(0.9 + random%6/(float)100);
            newParticle->setDuration(30 + random%16);
            newParticle->setAngle((-22.5+random%46)*toRadians);
            newParticle->setTypeId(2+i);
            newParticle->setValue((i == 0) ? this->worthMoney : this->worthExperience);
            addParticleToList(newParticle);
        }

        al_play_sample(killSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }else
        al_play_sample(hurtSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    this->damaged = true;
}

int Zombie::getHealth(int id){
    if(id == 0){
        return this->health;
    }else if(id == 1){
        return this->maxHealth;
    }
    return 0;
}

void Zombie::update(){
    this->colX = false, this->colY = false;

    if(!this->untilFinished){
        this->setAnimationValue(1, false, this->movementSpeed*2);
    }

    this->angle = -atan2(this->centerX - playerList[0]->centerX, this->centerY - playerList[0]->centerY);

    this->deltaX = sin(this->angle) * this->movementSpeed;
    this->deltaY = -cos(this->angle) * this->movementSpeed;

    float loopI = ceil(this->movementSpeed/this->width);

    for(float i = 0; i < loopI && (!this->colX || !this->colY); i++){

        if(checkCollision(this->posX + this->deltaX/loopI, this->posY, playerList[0]->posX, playerList[0]->posY,
            this->width, this->height, playerList[0]->width, playerList[0]->height)){
            this->colX = true;

            this->setAnimationValue(3, true, 2);
            //printf("\nHit Player");
        }
        if(checkCollision(this->posX, this->posY + this->deltaY/loopI, playerList[0]->posX, playerList[0]->posY,
            this->width, this->height, playerList[0]->width, playerList[0]->height)){
            this->colY = true;

            this->setAnimationValue(3, true, 2);
            //printf("\nHit Player");
        }

        for(int lI = 0; lI < MAX_LIVING && (!this->colX || !this->colY); lI++){
            if(livingList[lI] != NULL && livingList[lI]->checkActive() && livingList[lI]->entityId != this->entityId){
                if(checkCollision(this->posX + this->deltaX/loopI, this->posY, livingList[lI]->posX, livingList[lI]->posY,
                    this->width, this->height, livingList[lI]->width, livingList[lI]->height)){
                    this->colX = true;
                }
                if(checkCollision(this->posX, this->posY + this->deltaY/loopI, livingList[lI]->posX, livingList[lI]->posY,
                    this->width, this->height, livingList[lI]->width, livingList[lI]->height)){
                    this->colY = true;
                }
            }
        }

        if(!isPassable(this->posX + this->deltaX/loopI, this->posY, this->width, this->height) && !this->colX){
            this->colX = true;
        }else if(!this->colX){
            this->posX += this->deltaX/loopI;
        }

        if(!isPassable(this->posX, this->posY + this->deltaY/loopI , this->width, this->height) && !this->colY){
            this->colY = true;
        }else if(!this->colY){
            this->posY += this->deltaY/loopI;
        }
    }

    if(this->rememberAnimationValue == 3 && this->untilFinished){
        if(this->nextFrame && this->frameX == 2){
            this->deltaX = sin(this->angle)*8;
            this->deltaY = -cos(this->angle)*8;

            if(checkCollision(this->posX + this->deltaX, this->posY + this->deltaY, playerList[0]->posX, playerList[0]->posY,
                this->width, this->height, playerList[0]->width, playerList[0]->height)){

                playerList[0]->takeDamage(this->attackDamage);

                for(int i = 0; i < 5+random%6; i++){
                    this->random = rand();
                    ParticleEntity *newParticle = new ParticleEntity();
                    newParticle->setPos(playerList[0]->centerX, playerList[0]->centerY);
                    newParticle->setDimensions(2+random%7, 2+random%7);
                    newParticle->setMovementSpeed(3+random%8);
                    newParticle->setDrag(0.85 + random%10/(float)100);
                    newParticle->setDuration(2500+random%91);
                    newParticle->setAngle(this->angle-22.5*toRadians+(random%45)*toRadians);
                    newParticle->setTypeId(1);
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
                newParticle->setValue(this->attackDamage);
                addParticleToList(newParticle);
            }
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

void Zombie::draw(){
    if(!this->damaged)
        al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width/2, this->posY+this->height/2, this->angle, NULL);
    else
        al_draw_tinted_rotated_bitmap(this->frameImage, al_map_rgba_f(1, 0, 0, 1), this->width/2, this->height/2, this->posX+this->width/2, this->posY+this->height/2, this->angle, NULL);

    if(this->health < this->maxHealth){
        al_draw_rectangle(this->posX-1, this->posY-11, this->posX+this->width+1, this->posY-4, al_map_rgb(200,200,200), 2);
        al_draw_filled_rectangle(this->posX, this->posY-10, 2+(this->posX-2)+this->width/this->maxHealth*this->health, this->posY-5, al_map_rgb(200,0,0));
    }
}
