#include "Engine.h"
#include "MissileEntity.h"

MissileEntity::MissileEntity(){
    this->angle = 0;
    this->movementSpeed = 0;
    this->deltaX = 0;
    this->deltaY = 0;
    this->colX = false;
    this->colY = false;
}

void MissileEntity::setAngle(float angle){
    this->angle = angle;
}
void MissileEntity::setMovementSpeed(float movementSpeed){
    this->movementSpeed = movementSpeed;
}
