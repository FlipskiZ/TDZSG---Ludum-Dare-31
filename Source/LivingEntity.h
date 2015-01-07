#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H

#include "Entity.h"

class LivingEntity : public Entity{
    public:
        LivingEntity();
        void setMovementSpeed(float movementSpeed);
        float getMovementSpeed();
        virtual void takeDamage(int damage) = 0;

        float movementSpeed, maxMovementSpeed;
        float deltaX, deltaY;

        bool colX, colY;

    protected:

    private:
};

#endif // LIVINGENTITY_H
