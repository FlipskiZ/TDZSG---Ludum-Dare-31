#include "Engine.h"
#include "LivingEntity.h"

LivingEntity::LivingEntity(){
    this->movementSpeed = 0;
    this->maxMovementSpeed = 0;
    this->deltaX = 0;
    this->deltaY = 0;
    this->colX = false;
    this->colY = false;
}

void LivingEntity::setMovementSpeed(float movementSpeed){
    this->movementSpeed = movementSpeed;
    this->maxMovementSpeed = movementSpeed;
}

float LivingEntity::getMovementSpeed(){
    return this->movementSpeed;
}
