#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "LivingEntity.h"

class Zombie : public LivingEntity
{
    public:
        Zombie();
        void draw();
        void update();
        void setStats(int health, int attackDamge, int worthMoney, int worthExperience);
        void takeDamage(int damage);
        int getHealth(int id);

    protected:
    private:
        int health;
        int maxHealth;
        int attackDamage;
        int worthMoney;
        int worthExperience;

        int damagedHelper;
        bool damaged;
};

#endif // ZOMBIE_H
