#include "Actor.h"
#include "StudentWorld.h"

#include <iostream>
using namespace std;


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//

//ACTOR
Actor::Actor(int imageID, double startX, double startY, StudentWorld* world, int startDirection, int depth): GraphObject(imageID, startX, startY, startDirection, depth){
    m_dead = false;
    m_world = world;

}
bool Actor::isDead(){
    return m_dead;
}
void Actor::setDead(){
    m_dead = true;
}
StudentWorld* Actor::getWorld(){
    return m_world;
}
bool Actor::overlap(double x, double y){
    if(dist(x,y) <= 2*SPRITE_RADIUS){
        return true;
    }
    return false;
}
double Actor::dist(double x, double y){
    return sqrt(pow(getY()-y, 2)+pow(getX()-x, 2));
}


Actor::~Actor(){}

//SOCRATES
Socrates::Socrates(StudentWorld* world): Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, world, 0, 0){
    posAngle = 180;
    sprayCharges = 20;
    flameCharges = 5;
    hp = 100;
    
    
}

void Socrates::doSomething(){
    
    if(isDead()){ return; }
    int ch;
    if(getWorld()->getKey(ch)){
        double pi = 3.1415926535897;
        double dtor =  pi / 180;
        switch(ch){
            case KEY_PRESS_RIGHT:
                posAngle -= 5;
                setDirection(posAngle+180);
                moveTo(VIEW_WIDTH/2 + VIEW_RADIUS * cos(dtor * posAngle), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(dtor * posAngle));
                
                break;
            case KEY_PRESS_LEFT:
                posAngle += 5;
                setDirection(posAngle+180);
                moveTo(VIEW_WIDTH/2 + VIEW_RADIUS * cos(dtor * posAngle), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(dtor * posAngle));
                
                break;
            case KEY_PRESS_SPACE:
                if(sprayCharges > 0){
                    
                    double x = VIEW_WIDTH/2 + VIEW_RADIUS*cos(dtor*posAngle);
                    double y = VIEW_HEIGHT/2 + VIEW_RADIUS*sin(dtor*posAngle);
                                                        
                    Spray* spray = new Spray(x, y, getWorld(), posAngle);
                    getWorld()->addActor(spray);
                    sprayCharges--;
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                
                break;
            case KEY_PRESS_ENTER:
                // add check for flameCharges
                if(flameCharges > 0){
                    double angle = 0;
                    for(int i = 0; i < 16; i++){
                        angle = getDirection() + 22*i;
                        double x = getX() + 2*SPRITE_RADIUS * cos(dtor * (angle+180));
                        double y = getY() + 2*SPRITE_RADIUS *sin(dtor * (angle+180));
                        Flame* flame = new Flame(x, y, getWorld(), angle);
                        getWorld()->addActor(flame);
                        
                    }
                    flameCharges--;
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                
                break;
        };
  
    }
    else{
        if(sprayCharges < 20){
            sprayCharges++;
        }
    }

}
bool Socrates::damageable(){ return true; }
bool Socrates::doesBlock(){ return false;}
void Socrates::damage(int amt){
    hp -= amt;
    if(hp > 0){
        getWorld()->playSound(SOUND_PLAYER_HURT);
    }
    else{
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
    //update the status line?
}

Socrates::~Socrates(){}

//DIRTPILE
DirtPile::DirtPile(double startX, double startY, StudentWorld* world): Actor(IID_DIRT, startX, startY, world, 0, 1){
    
  
}
void DirtPile::doSomething(){
    
}
bool DirtPile::damageable(){ return true;}
bool DirtPile::doesBlock(){return true; }
void DirtPile::damage(int amt){
    setDead();
}


DirtPile::~DirtPile(){
    
}

Projectile::Projectile(int imageID, double startX, double startY, StudentWorld *world, double maxTravelDistance, int dir):Actor(imageID, startX, startY, world, dir, 1){
    
    travelDist = maxTravelDistance;
    
}

Spray::Spray(double startX, double startY, StudentWorld *world, int dir): Projectile(IID_SPRAY, startX, startY, world, 112, dir){}
void Spray::doSomething(){
    if(isDead()){
        return;
    }
    //spraying happens in student world because only studentworld is supposed to have knowledge of all actors in the game
    if(getWorld()->damageFirstActor(getX(), getY(), 2)){
        setDead();
        return;
    }
    moveAngle(getDirection()+180, 2*SPRITE_RADIUS);
    
    decTravelDist(2*SPRITE_RADIUS);
    
    if(getTravelDist() <= 0){
        setDead();
    }
    
}

Flame::Flame(double startX, double startY, StudentWorld *world, int dir):Projectile(IID_FLAME, startX, startY, world, 40, dir){}

void Flame::doSomething(){
    if(isDead()){
        return;
    }
    if(getWorld()->damageFirstActor(getX(), getY(), 5)){
        setDead();
        return;
    }
    moveAngle(getDirection()+180, 2*SPRITE_RADIUS);
    decTravelDist(2*SPRITE_RADIUS);
    if(getTravelDist() <= 0){
           setDead();
    }

}

//Bacterium
Bacterium::Bacterium(int imageID, double startX, double startY, StudentWorld* world, int health):Actor(imageID, startX, startY, world, 90, 0){
    foodEaten = 0;
    movePlanDist = 0;
    hp = health;
}
void Bacterium::incFoodEaten(){foodEaten++;}
int Bacterium::getFoodEaten(){return foodEaten;}
void Bacterium::resetFoodEaten(){foodEaten = 0;}

int Bacterium::getMovePlanDist(){return movePlanDist;}
void Bacterium::setMovePlanDist(int newPlan){movePlanDist = newPlan;}

int Bacterium::getHP(){return hp;}
void Bacterium::setHP(int health){hp = health;}

RegularSalmonella::RegularSalmonella(double startX, double startY, StudentWorld* world):Bacterium(IID_SALMONELLA, startX, startY, world, 4){}

void RegularSalmonella::damage(int amt){
    setHP(getHP()-amt);
    if(getHP() > 0){
        getWorld()->playSound(SOUND_SALMONELLA_HURT);
    }
    else{
        setDead();
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
        getWorld()->increaseScore(100);
        int rand = randInt(1, 2);
        if(rand == 1){
            Food* food = new Food(getX(), getY(), getWorld());
            getWorld()->addActor(food);
        }
        
    }
}

void RegularSalmonella::doSomething(){
    if(isDead()){
        return;
    }
    
    if(getWorld()->damageSocrates(getX(), getY(), 1)){
        
    }
    else{
        //food stuff
        
        if(getFoodEaten() == 3){
            double newX = getX();
            double newY = getY();
            
            genNewXandY(newX, newY);
            
            RegularSalmonella* rs = new RegularSalmonella(newX, newY, getWorld());
            getWorld()->addActor(rs);
            resetFoodEaten();
        }
        else if(getWorld()->eatFood(getX(), getY())){
            incFoodEaten();
        }
        
    }
    
    double pi = 3.1415926535897;
    double degreeToRad = pi/180;
    
    if(getMovePlanDist() > 0){

        setMovePlanDist(getMovePlanDist()-1);
        moveIfCanMove();
        return;
    }
    else{

        int closeFood = getWorld()->dirClosestFood(getX(), getY(), 128);

        if(closeFood == -1){ //food not found
            setDirection(randInt(0, 359));
            setMovePlanDist(10);
            return;
        }
        else{
            moveIfCanMoveDirFood(closeFood);
            return;
        }
    }
}


AgressiveSalmonella::AgressiveSalmonella(double startX, double startY, StudentWorld* world):Bacterium(IID_SALMONELLA, startX, startY, world, 10){

}

void AgressiveSalmonella::doSomething(){
    double pi = 3.1415926535897;
    double degreeToRad = pi/180;
    
    if(isDead()){
        return;
    }
    if(getWorld()->distToSocrates(getX(), getY()) <= 72){

        int dir = getWorld()->dirSocrates(getX(), getY());
        double dx = 3 * cos(degreeToRad * dir);
        double dy = 3 * sin(degreeToRad * dir);
        
        if(getWorld()->movingOverlap(getX()+dx, getY()+dy)){ //is this moving overlap? //step 2
            //no movement if stuck
        }
        else{
            if(getWorld()->distToSocrates(getX(), getY()) > 3){ //added additional check to prevent shaking
                setDirection(dir); //otherwise, move in direction of socrates
                moveAngle(dir, 3);
            }
        }
        
        if(getWorld()->damageSocrates(getX(), getY(), 2)){ //step 3
            return;
        }
        
        if(getFoodEaten() == 3){ //step 4
            double newX = getX();
            double newY = getY();
            genNewXandY(newX, newY);
            
            AgressiveSalmonella* as = new AgressiveSalmonella(newX, newY, getWorld());
            getWorld()->addActor(as);
            resetFoodEaten();
            return;
        }
        
        if(getWorld()->eatFood(getX(), getY())){ //step 5
            incFoodEaten();
        }
        
        return; //returning after steps 3-5 in case 2c
    }
    else{ //socrates not within 72 pixels. step 2 false

        if(getWorld()->damageSocrates(getX(), getY(), 2)){
            if(getMovePlanDist() > 0){ //step 6
                setMovePlanDist(getMovePlanDist()-1);
                moveIfCanMove();
                return;
            }
            else{ //step 7
                int closeFood = getWorld()->dirClosestFood(getX(), getY(), 128);
                if(closeFood == -1){ //food not found
                    setDirection(randInt(0, 359));
                    setMovePlanDist(10);
                    return;
                }
                else{
                    moveIfCanMoveDirFood(closeFood);
                    return;
                }
            }
        }
        else{ //step 3 false, step 4
            if(getFoodEaten() == 3){
                double newX = getX();
                double newY = getY();
                
                genNewXandY(newX, newY);
                
                AgressiveSalmonella* as = new AgressiveSalmonella(newX, newY, getWorld());
                getWorld()->addActor(as);
                resetFoodEaten();
            
                if(getMovePlanDist() > 0){ //step 6
                    setMovePlanDist(getMovePlanDist()-1);
                    moveIfCanMove();
                    return;
                }
                else{ //step 7
                    int closeFood = getWorld()->dirClosestFood(getX(), getY(), 128);
                    if(closeFood == -1){ //food not found
                        setDirection(randInt(0, 359));
                        setMovePlanDist(10);
                        return;
                    }
                    else{
                        moveIfCanMoveDirFood(closeFood);
                        return;
                    }
                }
                
            }
            else{//step 4 false, step 5
                if(getWorld()->eatFood(getX(), getY())){
                    incFoodEaten();
                }
                
                //step 6
                if(getMovePlanDist() > 0){ //step 6
                    setMovePlanDist(getMovePlanDist()-1);
                    moveIfCanMove();
                    return;
                }
                else{ //step 7
                    int closeFood = getWorld()->dirClosestFood(getX(), getY(), 128);
                    if(closeFood == -1){ //food not found
                        setDirection(randInt(0, 359));
                        setMovePlanDist(10);
                        return;
                    }
                    else{
                        moveIfCanMoveDirFood(closeFood);
                        return;
                    }
                }
            }
        }
    }
}

void AgressiveSalmonella::damage(int amt ){
    setHP(getHP()-amt);
    if(getHP() > 0){
        getWorld()->playSound(SOUND_SALMONELLA_HURT);
    }
    else{
        setDead();
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
        getWorld()->increaseScore(100);
        int rand = randInt(1, 2);
        if(rand == 1){
            Food* food = new Food(getX(), getY(), getWorld());
            getWorld()->addActor(food);
        }
    }
}


Food::Food(double startX, double startY, StudentWorld* world):Actor(IID_FOOD, startX, startY, world, 90, 1){
    
}

EColi::EColi(double startX, double startY, StudentWorld* world): Bacterium(IID_ECOLI, startX, startY, world, 5){}

void EColi::damage(int amt){
    setHP(getHP() - amt);
    if(getHP() <= 0){
        setDead();
        getWorld()->playSound(SOUND_ECOLI_DIE);
        getWorld()->increaseScore(100);
        
        int rand = randInt(1, 2);
        if(rand == 1){
            Food* food = new Food(getX(), getY(), getWorld());
            getWorld()->addActor(food);
        }
    }
    else{
        getWorld()->playSound(SOUND_ECOLI_HURT);
    }
}

void EColi::doSomething(){
    double pi = 3.1415926535897;
    double degreeToRad = pi/180;
    
    if(isDead()){
        return;
    }

    //step 2
    if(getWorld()->damageSocrates(getX(), getY(), 4)){
        //step 5
          if(getWorld()->distToSocrates(getX(), getY()) < 256){
                  int dir = getWorld()->dirSocrates(getX(), getY());
                  for(int i = 0; i < 10; i++){
                      int newDir = dir + 10*i;
                      if(newDir > 360){
                          newDir -= 360;
                      }
                      
                      double dy = 2 * sin(degreeToRad * newDir);
                      double dx = 2 * cos(degreeToRad * newDir );
                      
                      if(!getWorld()->movingOverlap(getX() + dx, getY() + dy)){
                          if(getWorld()->distToSocrates(getX(), getY()) > 2){

                              moveAngle(newDir, 2);
                          }
                          return;
                      }
                  }
              }
    }
    
    //step 3
    if(getFoodEaten() >= 3){
        double newX = getX();
        double newY = getY();
        
        genNewXandY(newX, newY);
        
        EColi* ecoli = new EColi(newX, newY, getWorld());
        getWorld()->addActor(ecoli);
        resetFoodEaten();
        //step 5
          if(getWorld()->distToSocrates(getX(), getY()) < 256){
                  int dir = getWorld()->dirSocrates(getX(), getY());
                  for(int i = 0; i < 10; i++){
                      int newDir = dir + 10*i;
                      if(newDir > 360){
                          newDir -= 360;
                      }
                      
                      double dy = 2 * sin(degreeToRad * newDir);
                      double dx = 2 * cos(degreeToRad * newDir );
                      
                      if(!getWorld()->movingOverlap(getX() + dx, getY() + dy)){
                          if(getWorld()->distToSocrates(getX(), getY()) > 2){

                              moveAngle(newDir, 2);
                          }
                          return;
                      }
                  }
              }
    }
    
    //step 4
    if(getWorld()->eatFood(getX(), getY())){
        incFoodEaten();
    }
    
    //step 5
   if(getWorld()->distToSocrates(getX(), getY()) < 256){
          int dir = getWorld()->dirSocrates(getX(), getY());
          for(int i = 0; i < 10; i++){
              int newDir = dir + 10*i;
              if(newDir > 360){
                  newDir -= 360;
              }
              
              double dy = 2 * sin(degreeToRad * newDir);
              double dx = 2 * cos(degreeToRad * newDir );
              
              if(!getWorld()->movingOverlap(getX() + dx, getY() + dy)){
                  if(getWorld()->distToSocrates(getX(), getY()) > 2){
                      moveAngle(newDir, 2);
                  }
                  return;
              }
          }
      }
    
    
}

Pit::Pit(double startX, double startY, StudentWorld* world):Actor(IID_PIT, startX, startY, world, 0, 1){
    numS = 5;
    numAS = 3;
    numEC = 2;
}

void Pit::doSomething(){
    if(numS == 0 && numAS == 0 && numEC == 0){
        //Inform the StudentWorld object that it has emitted all of its bacteria
        setDead();
        return;
    }
    int rand = randInt(1, 50);
    if(rand == 1){
        bool spawned = false;
        while(!spawned){
            int a = randInt(1, 3);
            if(a == 1 && numS > 0){
                getWorld()->addActor(new RegularSalmonella(getX(), getY(), getWorld()));
                numS--;
                spawned = true;
                getWorld()->playSound(SOUND_BACTERIUM_BORN);
            }
            if(a == 2 && numAS > 0){
                getWorld()->addActor(new AgressiveSalmonella(getX(), getY(), getWorld()));
                numAS--;
                spawned = true;
                getWorld()->playSound(SOUND_BACTERIUM_BORN);
            }
            if(a == 3 && numEC > 0){
                getWorld()->addActor(new EColi(getX(), getY(), getWorld()));
                numEC--;
                spawned = true;
                getWorld()->playSound(SOUND_BACTERIUM_BORN);    
            }
        }
    }
}

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* world, int life):Actor(imageID, startX, startY, world, 0, 1){
    lifetime = life;
}

void Goodie::setLifetime(int amt){
    lifetime = amt;
}

void Goodie::doSomething(){
    if(getLifetime() <= 0){
        setDead();
        return;
    }
    setLifetime(getLifetime()-1);
}

RestoreGoodie::RestoreGoodie(double startX, double startY, StudentWorld* world, int level): Goodie(IID_RESTORE_HEALTH_GOODIE, startX, startY, world){ //change made here
    setLifetime(max(randInt(0, 300 - 10 * level - 1), 50));
}

void RestoreGoodie::doSomething(){
    if(isDead()){
        return;
    }

    if(getWorld()->damageSocrates(getX(), getY(), 0)){
        
        getWorld()->increaseScore(250);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->restoreSocrates();
        return;
    }
    
    Goodie::doSomething();
}


void Socrates::restoreHealth(){
    hp = 100;
}

ExtraLifeGoodie::ExtraLifeGoodie(double startX, double startY, StudentWorld* world, int level): Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, world){
    setLifetime(max(randInt(0, 300 - 10 * level - 1), 50));
}

void ExtraLifeGoodie::doSomething(){
    if(isDead()){
        return;
    }

    if(getWorld()->damageSocrates(getX(), getY(), 0)){
        
        getWorld()->increaseScore(500);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->incLives();
        return;
    }
    
    Goodie::doSomething();
}

Fungus::Fungus(double startX, double startY, StudentWorld* world, int level): Goodie(IID_FUNGUS, startX, startY, world){
    setLifetime(max(randInt(0, 300 - 10 * level - 1), 50));
}

void Fungus::doSomething(){
    if(isDead()){
        return;
    }
    
    if(getWorld()->damageSocrates(getX(), getY(), 20)){
        getWorld()->increaseScore(-50);
        setDead();
        return;
    }
    
    Goodie::doSomething();
    
}

FlameGoodie::FlameGoodie(double startX, double startY, StudentWorld* world, int level):Goodie(IID_FLAME_THROWER_GOODIE, startX, startY, world){
    setLifetime(max(randInt(0, 300 - 10 * level - 1), 50));
}

void FlameGoodie::doSomething(){
    if(isDead()){
        return;
    }

    if(getWorld()->damageSocrates(getX(), getY(), 0)){
        getWorld()->increaseScore(300);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->incSocratesFlames(5);
        return;
    }
    
    Goodie::doSomething();
    
    
}

void Socrates::incFlameCharges(int amt){
    flameCharges += amt;
}

void Bacterium::genNewXandY(double &x, double &y){
    if(getX() < VIEW_WIDTH/2){
        x += SPRITE_RADIUS;
    }
    else{
        x -= SPRITE_RADIUS;
    }

    if(getY() < VIEW_HEIGHT/2){
        y += SPRITE_RADIUS;
    }
    else{
        y += SPRITE_RADIUS;
    }
}

void Bacterium::moveIfCanMove(){
    double pi = 3.1415926535897;
    double degreeToRad = pi/180;
    
    double dx = 3 * cos(degreeToRad * getDirection());
    double dy = 3 * sin(degreeToRad * getDirection());

    bool inBounds = sqrt(pow(getY()+dy-VIEW_HEIGHT/2, 2)+pow(getX()+dx-VIEW_WIDTH/2, 2)) <= VIEW_RADIUS;

    if(!getWorld()->movingOverlap(getX() + dx, getY() + dy) && inBounds){
        moveAngle(getDirection(), 3);
    }
    else{
        setDirection(randInt(0, 359));
        setMovePlanDist(10);
    }
}

void Bacterium::moveIfCanMoveDirFood(int closeFood){
    double pi = 3.1415926535897;
    double degreeToRad = pi/180;
    
    double dx = 3 * cos(degreeToRad * closeFood);
    double dy = 3 * sin(degreeToRad * closeFood);

    if(getWorld()->movingOverlap(getX()+dx, getY()+dy)){
        setDirection(randInt(0, 359));
        setMovePlanDist(10);
        return;
    }
    else{
        setDirection(closeFood);
        setMovePlanDist(10);
        return;
    }
    
}
