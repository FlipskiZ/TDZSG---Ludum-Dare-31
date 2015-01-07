#ifndef DEBUGCLASS_H
#define DEBUGCLASS_H

#include "Entity.h"


class DebugClass : public Entity{
    public:
        DebugClass();

        void draw();
        void update();
    protected:
    private:
    float timeAlive;
};

#endif // DEBUGCLASS_H
