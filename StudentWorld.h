#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
class Actor;
class Socrates;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld();
    
    //added public member functions
    bool damageFirstActor( double x,  double y, int amt); //used by projectiles
    bool damageSocrates( double x,  double y, int amt); //used by bacteria
    bool addActor(Actor* actor);
    bool eatFood( double x,  double y);
    bool movingOverlap( double x,  double y); //used by bacteria
    int dirClosestFood( double x,  double y, double dist); //returns -1 if no food found within 128 pixels
    int distToSocrates(double x, double y);
    int dirSocrates( double x,  double y);
    void placePits();
    void placeFood(int num);
    void placeDirt(int num);
    void restoreSocrates();
    void createGoodies();
    void incSocratesFlames(int amt);
    void printScore();
    void direction(double dx, double dy, double &dir);
    

private:
    Socrates* socrates;
    Actor* dp;
    Actor* spray;
    std::vector<Actor*> activeActors;
    int numBacteria;
};

#endif // STUDENTWORLD_H_
