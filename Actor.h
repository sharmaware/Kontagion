#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <string>


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
public:
    Actor(int imageID, double startX, double startY, StudentWorld* world, int startDirection = 0, int depth = 0);
    bool isDead();
    void setDead();
    bool overlap(double x, double y); //virtual?
    double dist(double x, double y);
    StudentWorld* getWorld();
    virtual void doSomething() = 0;
//iffy
    virtual void damage(int amt){} //Actor* attacker??
    virtual bool isFood(){ return false;}
    virtual bool isProjectile(){ return false;}
    virtual bool damageable() =0;
    virtual bool doesBlock() =0;
    virtual bool isPit(){return false;}
    virtual bool isBacterium(){return false;}
    virtual ~Actor();

private:
    bool m_dead;
    StudentWorld* m_world;
    std::string name;

};

//bool movingOverlap(double x1, double y1, double x2, double y2);

class Socrates: public Actor{
public:
    Socrates(StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual bool doesBlock();
    virtual void damage(int amt); //Actor * attacker?
    int getHealth(){return hp;}
    void restoreHealth();
    int getSprays(){return sprayCharges;}
    int getFlames(){return flameCharges;}
    void incFlameCharges(int amt);
    virtual ~Socrates();

private:
    int posAngle;
    int sprayCharges;
    int flameCharges;
    int hp;
};

class DirtPile: public Actor{
public:
    DirtPile( double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual ~DirtPile();
    virtual void damage(int amt);
    virtual bool doesBlock();
    
};


class Projectile: public Actor{
public:
    Projectile(int imageID, double startX, double startY, StudentWorld *world, double maxTravelDistance, int dir);
    double getTravelDist(){return travelDist;}
    void decTravelDist(double amt){ travelDist -= amt; }

    virtual void doSomething(){}
    virtual bool damageable(){ return false;}
    virtual bool doesBlock(){ return false;}
    virtual bool isProjectile(){return true;}
    virtual ~Projectile(){}
    
private:
    double travelDist;
    
};
class Spray: public Projectile{
public:
    Spray(double startX, double startY, StudentWorld *world, int dir);
    virtual void doSomething();
    
};
class Flame: public Projectile{
public:
    Flame(double startX, double startY, StudentWorld *world, int dir);
    virtual void doSomething();
};

class Bacterium: public Actor{
public:
    Bacterium(int imageID, double startX, double startY, StudentWorld* world, int health);
    virtual void doSomething(){};
    virtual bool damageable(){return true;}
    virtual bool doesBlock(){return false;}
    virtual bool isBacterium(){return true;}
    virtual ~Bacterium(){}
    
    void incFoodEaten();
    int getFoodEaten();
    void resetFoodEaten();
    
    int getMovePlanDist();
    void setMovePlanDist(int newPlan);
    
    int getHP();
    void setHP(int health);
    
    //functions used by doSomethings
    void genNewXandY(double &x, double &y);
    void moveIfCanMove();
    void moveIfCanMoveDirFood(int closestFood);
    
    
private:
    int foodEaten;
    int movePlanDist;
    int hp;
    
};

class RegularSalmonella: public Bacterium{
public:
    RegularSalmonella(double startX, double startY, StudentWorld* world);
    virtual void damage(int amt);
    virtual void doSomething();
};

class AgressiveSalmonella: public Bacterium{
public:
    AgressiveSalmonella(double startX, double startY, StudentWorld* world);
    virtual void damage(int amt);
    virtual void doSomething();
};

class EColi: public Bacterium{
public:
    EColi(double startX, double startY, StudentWorld* world);
    virtual void damage(int amt);
    virtual void doSomething();
};

class Goodie: public Actor{
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* world, int life = 0);
    
    int getLifetime(){ return lifetime;}
    void setLifetime(int amt);
    virtual void doSomething();
    bool damageable(){return true;}
    bool doesBlock(){return false;}
    virtual ~Goodie(){}
    
private:
    int lifetime;
};

class RestoreGoodie: public Goodie{
public:
    RestoreGoodie(double startX, double startY, StudentWorld* world, int level);
    virtual void doSomething();
};

class ExtraLifeGoodie: public Goodie{
public:
    ExtraLifeGoodie(double startX, double startY, StudentWorld* world, int level);
    virtual void doSomething();
};

class FlameGoodie: public Goodie{
public:
    FlameGoodie(double startX, double startY, StudentWorld* world, int level);
    virtual void doSomething();
};

class Fungus: public Goodie{
public:
    Fungus(double startX, double startY, StudentWorld* world, int level);
    virtual void doSomething();
};


class Food: public Actor{
public:
    Food(double startX, double startY, StudentWorld* world);
    virtual void doSomething(){}
    virtual bool damageable(){return false;}
    virtual bool doesBlock(){return false;}
    virtual bool isFood(){return true;}
    
};

class Pit: public Actor{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual void damage(int amt){}
    virtual void doSomething();
    virtual bool isPit(){return true;}
    virtual bool damageable(){return false;};
    virtual bool doesBlock(){return false;}
    
private:
    int numS;
    int numAS;
    int numEC;
};


#endif // ACTOR_H_
