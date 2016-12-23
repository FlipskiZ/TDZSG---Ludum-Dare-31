#ifndef PISTOLBULLET_H
#define PISTOLBULLET_H

#include "MissileEntity.h"

class PistolBullet : public MissileEntity{
    public:
        PistolBullet();
        void draw();
        void update();
        void setRocket(bool rocket, int radius);
        void setStats(int attackDamage, int maxAttackDamage);
        void setQuakor(bool quakor);
        void setDynamite(bool dynamite);

    protected:
    private:
        int attackDamage;
        int maxAttackDamage;

        bool rocket;
        int radius;

        int explosionLingerHelper;
        bool explosionLingerActive;

        bool quakor;

        bool dynamite;
};

#endif // PISTOLBULLET_H
