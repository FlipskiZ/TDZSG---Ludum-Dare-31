#include "Engine.h"
#include "DebugClass.h"

DebugClass::DebugClass(){
    this->timeAlive = al_get_time();
}

void DebugClass::draw(){
    al_draw_rotated_bitmap(this->frameImage, this->width/2, this->height/2, this->posX+this->width, this->posY+this->height, this->angle, NULL);
}
void DebugClass::update(){
    if(al_get_time() - this->timeAlive >= 20){
        this->active = false;
    }
}
