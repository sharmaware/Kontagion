#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}


void StudentWorld::placePits(){
    for(int j = 0; j < getLevel(); j++){
        int x = 0;
        int y = 0;
        bool overlap = false;
        do{
            overlap = false;
            x = randInt(VIEW_WIDTH/2-120, VIEW_WIDTH/2+120);
            y = randInt(VIEW_HEIGHT/2-120, VIEW_HEIGHT/2+120);
            
            for(int i = 0; i < activeActors.size(); i++){
                if(activeActors[i]->overlap(x, y)){
                    overlap = true;
                }
            }
            
        }while(sqrt(pow(x - VIEW_WIDTH/2, 2) + pow(y - VIEW_HEIGHT/2, 2)) > 120 || overlap);
        
        activeActors.push_back(new Pit(x, y, this));
    }
}

void StudentWorld::placeFood(int num){
    for(int j = 0; j < num; j++){
        int x = 0;
        int y = 0;
        bool overlap = false;
        do{
            overlap = false;
            x = randInt(VIEW_WIDTH/2-120, VIEW_WIDTH/2+120);
            y = randInt(VIEW_HEIGHT/2-120, VIEW_HEIGHT/2+120);
            
            for(int i = 0; i < activeActors.size(); i++){
                if(activeActors[i]->overlap(x, y)){
                    overlap = true;
                }
            }
            
        }while(sqrt(pow(x - VIEW_WIDTH/2, 2) + pow(y - VIEW_HEIGHT/2, 2)) > 120 || overlap);
        
        activeActors.push_back(new Food(x, y, this));
    }
}

void StudentWorld::placeDirt(int num){
    for(int j = 0; j < num; j++){
           int x = 0;
           int y = 0;
           bool overlap = false;
           do{
               overlap = false;
               x = randInt(VIEW_WIDTH/2-120, VIEW_WIDTH/2+120);
               y = randInt(VIEW_HEIGHT/2-120, VIEW_HEIGHT/2+120);
               
               for(int i = 0; i < activeActors.size(); i++){
                   if(activeActors[i]->overlap(x, y) && !activeActors[i]->doesBlock()){ //is not dirt
                       overlap = true;
                   }
               }
               
           }while(sqrt(pow(x - VIEW_WIDTH/2, 2) + pow(y - VIEW_HEIGHT/2, 2)) > 120 || overlap);
           
           activeActors.push_back(new DirtPile(x, y, this));
       }
}



int StudentWorld::init()
{
    socrates = new Socrates(this);

    placePits();
    placeFood(min(5 * getLevel(), 25));
    placeDirt(max(180-20*getLevel(), 20));
    
    return GWSTATUS_CONTINUE_GAME;
    //
    
}

int StudentWorld::move()
{
    socrates->doSomething();
    
    //calling doSomething for every alive activeActor
    for(int i = 0; i < activeActors.size(); i++){
        if(!activeActors[i]->isDead()){
            activeActors[i]->doSomething();
        }
        
        if(socrates->isDead()){
            decLives();
//            delete socrates;
            return GWSTATUS_PLAYER_DIED;
        }
    }
    
    //removing dead players
    for(int i = 0; i < activeActors.size(); i++){
        if(activeActors[i]->isDead()){
            delete activeActors[i];
            activeActors.erase(activeActors.begin()+i);
            i--;
        }
    }
    if(socrates->isDead()){
//        delete socrates;
        return GWSTATUS_PLAYER_DIED;
    }
    
    createGoodies();
    printScore();
    

    //checking for level ended
    for(int i = 0; i < activeActors.size(); i++){
        if(activeActors[i]->isPit() || activeActors[i]->isBacterium()){
            return GWSTATUS_CONTINUE_GAME;
        }
    }
    
//    delete socrates;
    return GWSTATUS_FINISHED_LEVEL;

}

void StudentWorld::printScore(){
    ostringstream oss;
    oss.fill('0');
    oss << "Score: " <<setw(6) << getScore();
    oss.fill(' ');
    oss << "  Level: " <<setw(2) << getLevel() << " Lives:" << setw(2) << getLives() << " Health:" << setw(4) << socrates->getHealth() << "  Sprays: " << setw(2) << socrates->getSprays() << " Flames: " << setw(2) << socrates->getFlames() << endl;
   
    setGameStatText(oss.str());
}

void StudentWorld::cleanUp()
{
    if(socrates != nullptr){
        delete socrates;
        socrates = nullptr;
    }
    
    for(int i = 0; i < activeActors.size();){
        delete activeActors[i];
        activeActors.erase(activeActors.begin() + i);
    }
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

bool StudentWorld::damageFirstActor(double x, double y, int amt){
    for(vector<Actor*>::iterator it = activeActors.begin(); it != activeActors.end(); it++){
        if(!(*it)->isDead() && (*it)->damageable() && (*it)->overlap(x, y)){ //need to check for !isDead?
            (*it)->damage(amt);
            return true;
        }
    }
    return false;
}

bool StudentWorld::addActor(Actor* actor){
    activeActors.push_back(actor);
    return true;
}

bool StudentWorld::damageSocrates( double x,  double y, int amt){
    if(socrates->overlap(x, y)){
        socrates->damage(amt);
        return true;
    }
    return false;
}

void StudentWorld::direction(double dx, double dy, double &dir){
    double pi = 3.1415926535897;
    double radToDegree = 180/pi;
    
    if(dy > 0){
        if(dx > 0){
            dir = radToDegree * (atan(dy/dx));
        }
        else if(dx < 0){
            dir = radToDegree * (pi - atan(-1 * dy/dx));
        }
        else{
            dir = 90;
        }
    }
    else if(dy < 0){
        if(dx < 0){
            dir = radToDegree * (pi + atan(dy/dx));
        }
        else if(dx > 0){
            dir = radToDegree * (2*pi - atan(-1 * dy/dx));
        }
    }
    else{
        if(dx > 0){
            dir = 0;
        }
        else{
            dir = 180;
        }
    }
    
}

int StudentWorld::dirClosestFood( double x,  double y, double dist){
    double minDistance = dist;
    double minDir = -1;
    double pi = 3.1415926535897;
    double radToDegree = 180/pi;
    for(int i = 0; i < activeActors.size(); i++){
        if(!activeActors[i]->isDead() && activeActors[i]->isFood()){
            int dist = sqrt(pow(activeActors[i]->getX() - x, 2) + pow(activeActors[i]->getY()-y, 2));
            if(dist < minDistance){
                minDir = activeActors[i]->getDirection();
                double dy = activeActors[i]->getY() - y;
                double dx = activeActors[i]->getX() - x;
                
                direction(dx, dy, minDir);
                minDistance = dist;
            }
            
        }
    }
    
    return minDir;
}

bool StudentWorld::eatFood( double x,  double y){
    for(int i = 0; i < activeActors.size(); i++){
        if(!activeActors[i]->isDead() && activeActors[i]->overlap(x, y) && activeActors[i]->isFood()){
            activeActors[i]->setDead();
            return true;
        }
    }
    return false;
}

bool StudentWorld::movingOverlap( double x,  double y){
    for(int i = 0; i < activeActors.size(); i++){
        if(!activeActors[i]->isDead() && activeActors[i]->doesBlock()){
            double dist = sqrt(pow(activeActors[i]->getX() - x, 2) + pow(activeActors[i]->getY()-y, 2));
            if(dist <= SPRITE_RADIUS){
//                cout << "moving overlap" << endl;
//                cout << "returned true!"
                return true;
                
            }
        }
    }
    return false;
}

int StudentWorld::distToSocrates(double x, double y){
    return socrates->dist(x, y);
}

int StudentWorld::dirSocrates( double x, double y){
    double dx = socrates->getX() - x;
    double dy = socrates->getY() - y;
    
    double dir = -1;
    direction(dx, dy, dir);
    return dir;
}

void StudentWorld::restoreSocrates(){
    socrates->restoreHealth();
}

void StudentWorld::createGoodies(){
    int chanceFungus = max(510 - getLevel() * 10, 200);
    int makeFungus = randInt(0, chanceFungus-1);
    
    if(makeFungus == 0){
        int angle = randInt(0, 360);
        Fungus* fg = new Fungus(VIEW_WIDTH/2 + VIEW_RADIUS * cos(angle), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(angle), this, getLevel());
        addActor(fg);
    }
    
    
    int chanceGoodie = max(510 - getLevel() * 10, 250);
    int makeGoodie = randInt(0, chanceGoodie-1);
    if(makeGoodie == 0){
        int whichGoodie = randInt(1, 10);
        if(whichGoodie >= 1 && whichGoodie <= 6){
            int angle = randInt(0, 360);
            RestoreGoodie* rg = new RestoreGoodie(VIEW_WIDTH/2 + VIEW_RADIUS * cos(angle), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(angle), this, getLevel());
            addActor(rg);
        }
        else if(whichGoodie >= 7 && whichGoodie <= 9){
            int angle = randInt(0, 360);
            FlameGoodie* fg = new FlameGoodie(VIEW_WIDTH/2 + VIEW_RADIUS * cos(angle), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(angle), this, getLevel());
            addActor(fg);
        }
        else{
            int angle = randInt(0, 360);
            ExtraLifeGoodie* elg = new ExtraLifeGoodie(VIEW_WIDTH/2 + VIEW_RADIUS * cos(angle), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(angle), this, getLevel());
            addActor(elg);
            
        }



    }

//    int angle = randInt(0, 360);
//    RestoreGoodie* rg = new RestoreGoodie(VIEW_WIDTH/2 + VIEW_RADIUS * cos(angle), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(angle), this, 1);
//    addActor(rg);

}

void StudentWorld::incSocratesFlames(int amt){
    socrates->incFlameCharges(amt);
}
