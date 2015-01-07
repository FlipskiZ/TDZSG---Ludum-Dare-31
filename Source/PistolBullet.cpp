#include "Engine.h"
#include "PistolBullet.h"

PistolBullet::PistolBullet()
{
    this->animationValue = 0;
    this->attackDamage = 0;
    this->maxAttackDamage = 0;
    this->rocket = false;
    this->radius = 0;
    this->explosionLingerHelper = 0;
    this->explosionLingerActive = false;
    this->quakor = false;
    this->dynamite = false;
}

void PistolBullet::setRocket(bool rocket, int radius){
    this->rocket = rocket;
    this->radius = radius;
}

void PistolBullet::setQuakor(bool quakor){
    this->quakor = quakor;
}

void PistolBullet::setDynamite(bool dynamite){
    if(dynamite){
        this->rocket = true;
        this->radius = 128;

        for(int x = -this->radius/tileSize+this->posX/tileSize; x <= this->radius/tileSize+this->posX/tileSize; x++){
            for(int y = -this->radius/tileSize+this->posY/tileSize; y <= this->radius/tileSize+this->posY/tileSize; y++){
                if(x >= 0 && x < mapArrayWidth && y >= 0 && y < mapArrayHeight){
                    mapArray[x][y] = rand()%2;
                    mapArrayRotation[x][y] = rand()%4;
                }
            }
        }
        this->dynamite = dynamite;
    }
}

void PistolBullet::setStats(int attackDamage, int maxAttackDamage){
    this->attackDamage = attackDamage;
    this->maxAttackDamage = maxAttackDamage;
}

void PistolBullet::update(){
    this->colX = false, this->colY = false;

    if(this->quakor || this->dynamite){
        this->colX = true;
    }

    this->deltaX = sin(this->angle) * this->movementSpeed;
    this->deltaY = -cos(this->angle) * this->movementSpeed;

    float loopI = ceil(this->movementSpeed/this->width);

    for(float i = 0; i < loopI && !colX && !this->explosionLingerActive; i++){
        if(!isPassable(this->posX + this->width*i*(deltaX/loopI/this->width), this->posY + this->height*i*(deltaY/loopI/this->height), this->width, this->height) && !this->colX){
            this->colX = true;
        }
        for(int lI = 0; lI < MAX_LIVING && !colX; lI++){
            if(livingList[lI] != NULL && livingList[lI]->checkActive()){
                if(checkCollision(this->posX + this->width*i*(deltaX/loopI/this->width), this->posY + this->height*i*(deltaY/loopI/this->height), livingList[lI]->posX, livingList[lI]->posY,
                    this->width, this->height, livingList[lI]->width, livingList[lI]->height)){

                    this->colX = true;

                    if(!this->rocket){
                        this->random = rand();
                        int takeDamage = this->attackDamage + random%(maxAttackDamage-attackDamage+1);

                        livingList[lI]->takeDamage(takeDamage);

                        for(int i = 0; i < 5+this->random%6; i++){
                            this->random = rand();
                            ParticleEntity *newParticle = new ParticleEntity();
                            newParticle->setPos(this->centerX, this->centerY);
                            newParticle->setDimensions(2+this->random%7, 2+this->random%7);
                            newParticle->setMovementSpeed(this->movementSpeed/2-this->movementSpeed/6+this->random%(int)round(this->movementSpeed/3+1));
                            newParticle->setDrag(0.85 + this->random%11/(float)100);
                            newParticle->setDuration(30+this->random%91);
                            newParticle->setAngle(this->angle-22.5*toRadians+(random%46)*toRadians);
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
                        newParticle->setValue(takeDamage);
                        addParticleToList(newParticle);
                    }
                }
            }
        }
        /*if(colX){
            DebugClass *purplePixel = new DebugClass();
            purplePixel->setPos(this->posX + this->width*i*(deltaX/loopI/this->width), this->posY + this->height*i*(deltaY/loopI/this->height));
            purplePixel->setDimensions(1, 1);
            purplePixel->setSheetDimensions(1, 1, 1, 1);
            purplePixel->setAnimationSpeed(0);
            purplePixel->setBitmap(pixelImage);
            addDebugToList(purplePixel);
        }*/
    }
    if(!this->colX && !this->explosionLingerActive){
        this->posX += this->deltaX;
        this->posY += this->deltaY;
    }else if(this->colX){
        if(!this->rocket){
            this->active = false;
        }else if(!this->explosionLingerActive){
            this->explosionLingerActive = true;
            int takeDamage = 0;
            for(int lI = 0; lI < MAX_LIVING; lI++){
                if(livingList[lI] != NULL && livingList[lI]->checkActive()){
                    if(checkCollision(this->posX - this->radius, this->posY - this->radius, livingList[lI]->posX, livingList[lI]->posY,
                    this->radius*2, this->radius*2, livingList[lI]->width, livingList[lI]->height)){

                        takeDamage = this->attackDamage + random%(maxAttackDamage-attackDamage+1);

                        livingList[lI]->takeDamage(takeDamage);

                        for(int i = 0; i < 15+random%16; i++){
                            this->random = rand();
                            ParticleEntity *newParticle = new ParticleEntity();
                            newParticle->setPos(this->centerX, this->centerY);
                            newParticle->setDimensions(2+random%7, 2+random%7);
                            newParticle->setMovementSpeed(5+random%11);
                            newParticle->setDrag(0.9 + random%6/(float)100);
                            newParticle->setDuration(60+random%61);
                            newParticle->setAngle(rand()%361*toRadians);
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
                        newParticle->setValue(takeDamage);
                        addParticleToList(newParticle);
                    }
                }
            }
            if(!this->quakor)
                al_play_sample(explosionSound, volumeLevel, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
    }

    if(this->explosionLingerActive){
        this->explosionLingerHelper++;
        if(this->explosionLingerHelper >= 30){
            this->active = false;
        }
    }

    this->updateCenter();


    this->deltaX = 0;
    this->deltaY = 0;
}

void PistolBullet::draw(){
    if(!this->explosionLingerActive)
        al_draw_rotated_bitmap(bulletImage, this->width/2, this->height/2, this->posX+this->width/2, this->posY+this->height/2, this->angle, NULL);
    else
        al_draw_scaled_bitmap(explosionImage, 0, 0, 32, 32, this->posX-this->radius, this->posY-this->radius, this->radius*2, this->radius*2, NULL);
}
