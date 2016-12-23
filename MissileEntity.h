#ifndef MISSILEENTITY_H
#define MISSILEENTITY_H

#include "Entity.h"

class MissileEntity : public Entity{
    public:
        MissileEntity();
        void setAngle(float angle);
        void setMovementSpeed(float movementSpeed);

        float angle, movementSpeed;
        float deltaX, deltaY;

        bool colX, colY;

    protected:

    private:
};

#endif // MISSILEENTITY_H

