#ifndef PLAYER_H
#define PLAYER_H

#include "LivingEntity.h"

class Player : public LivingEntity{
    public:
        Player();
        void update();
        void draw();
        void fireBullet();
        void setHealth(int health, int maxHealth);
        void takeDamage(int damage);
        void addMoney(int money);
        void addExperience(int experience);
        void levelUp();
        void setGun(int gunId, int fireTime, int attackDamage, int maxMagazine, int reloadTime, int bulletSpeed, bool autoFire, int maxAttackDamage);
        void addAmmo(int ammo);
        void reload();
        void setMeleeDamage(int meleeDamage);
        void setStamina(float maxStamina);
        void setSprint(bool sprinting);
        void useDynamite();

        void buyWeapon(int id1, int id2);
        void levelSkill(int id1, int id2);
        void buyConsumable(int id);

        bool getAutoFire();
        float getMaxMovement();
        float getMovementSpeed();
        int getStats(int id);

    protected:
    private:
        int health;
        int maxHealth;
        int money;
        int experience;
        int abilityPoints;
        int level;
        int experienceToLevel;
        int gunId;
        int ammo;
        int fireTime;
        int attackDamage;
        int maxAttackDamage;
        int magazine;
        int maxMagazine;
        int reloadTime;
        int bulletSpeed;
        int meleeDamage;
        int dynamite;

        bool autoFire;

        int fireTimeHelper;
        bool fireTimeActive;
        int reloadTimeHelper;
        bool reloadTimeActive;
        int staminaRechargeHelper;
        bool staminaRechargeActive;
        int crippledHelper;
        bool crippledActive;
        int damagedHelper;
        bool damaged;

        bool sprinting;
        float stamina;
        float maxStamina;

        ALLEGRO_SAMPLE_ID soundId;
};

#endif // PLAYER_H
