#include "nurse.h"
#include "elf.h"
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include "game.h"
#include "tower.h"
#include "e_elf.h"
#include "e_tower.h"
#include "watermelon.h"
#include "tank.h"
#include "horse.h"
#include "e_tank.h"
extern Game *game;
int Nurse :: mortal = 0;
Nurse::Nurse(int x,int y):attack_damage(0),step(10),hp(25),breast(0.2){
    //set pos
    setPos(x,y);
    //set graph
    setPixmap(QPixmap(":/images/cute/nurse.png"));

    //create hp
    health = new Health(x,y,step,hp);
    game->scene->addItem(health);

    c_Tower = false;
    c_area = false;

    //move
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(80);//200 ms


}

void Nurse::move(){
    if(health->getHp() == 0){//dead
        mortal++;
        delete this;
    }
    if(game->tower->towerhp == 0 || game->e_tower->towerhp == 0 ){
        disconnect(timer,SIGNAL(timeout()),this,SLOT(move()));
    }
    //get colliding item
    QList<QGraphicsItem *> colliding_items = this->collidingItems();
    //nothing collided
    if(colliding_items.size() == 0){
        health->colliding_st = false;//let hp go
        setPos(x()-step,y());
        return;
    }
    //true or false good job!
    health->colliding_st = false;
    bool hasrun = false;//meet elf has run 1 time
    bool hasrun1 = false;// meet water
    bool felf = false;//meet friend elf
    bool fhorse = false;
    bool fnurse = false;
    bool ftank = false;
    bool fwater = false;
    bool hasrun2 = false;//meet nurse
    bool hasrun3 = false;
    bool hasrun4 = false;
    c_Elf = false;//
    c_Tank = false;
    c_Horse =false;

    int j = 0;
    for(int i=0,n=colliding_items.size();i<n;i++){
        E_Tower *tower = dynamic_cast<E_Tower *>(colliding_items[i]);//colliding tower
        QGraphicsPolygonItem *attack_area = dynamic_cast<QGraphicsPolygonItem *>(colliding_items[i]);
        E_Elf *e_elf = dynamic_cast<E_Elf *>(colliding_items[i]);
        Elf *elf = dynamic_cast<Elf *>(colliding_items[i]);
        Watermelon *water = dynamic_cast<Watermelon *>(colliding_items[i]);
        Nurse *nurse = dynamic_cast<Nurse *>(colliding_items[i]);
        Horse *horse = dynamic_cast<Horse *>(colliding_items[i]);
        Tank *tank = dynamic_cast<Tank *>(colliding_items[i]);
        E_Tank *e_tank = dynamic_cast<E_Tank *>(colliding_items[i]);
        if(tower){
            c_Tower = true;
            health->colliding_st = true;//stop hp
            tower->health->hurt(attack_damage);//tower be attacked
            tower->tower_be_damaged(attack_damage);
            return;//final destination
        }
        else if (attack_area && (!c_Tower)){
            c_area = true;
            setPos(x()-step,y());
        }
        else if(e_elf){//enemy
            c_Elf = true;
            health->colliding_st = true;//stop hp
            if(e_elf->health->getHp()<=attack_damage){// must have = !!!!!
                c_Elf = false;
                health->colliding_st = false;
            }
            e_elf->health->hurt(attack_damage);
        }
        else if(e_tank){
            c_Tank = true;
            health->colliding_st = true;//stop hp
            if(e_tank->health->getHp()<=attack_damage){// must have = !!!!!
                c_Tank = false;
                health->colliding_st = false;
            }
            e_tank->health->hurt(attack_damage);
        }
        else if(elf){//friend    && (!c_Tower) && (!c_Elf) && (!c_area)
            elf->health->cure(breast);
            //health->colliding_st = true;
            felf = true;
        }
//        else if(water && (!c_Tower) && (!c_Elf) && (!c_area) && (!c_Tank) ){//friend, does not meet enemy
//            if((!felf)&& (!fhorse) && (!fnurse) && (!ftank) && (!hasrun1))// if meet elf --> cannot move
//                setPos(x()-step,y());
//            fwater = true;
//            hasrun1 = true;
//        }
        else if(nurse){//meet nurse, heal but still go
            if((!c_Tower) && (!c_Elf) && (!c_area)&&(!hasrun2) && (!felf)&&(!c_Tank)&& (!fhorse) && (!ftank)){
                //health->colliding_st = true;
                setPos(x()-step,y());
            }
                //don't collidi enemy,keep going;friends,stop
            nurse->health->cure(breast);
            //health->colliding_st = false;
            hasrun2 = true;
            fnurse = true;
        }
        else if(horse){
            if((!hasrun4) && (!felf) && (!c_Tower)&& (!c_area) && (!c_Elf)&&(!c_Tank)&& (!fhorse) && (!fnurse) && (!ftank))
                setPos(x()-step,y());
            hasrun4 = true;
            fhorse = true;
        }
        else if(tank){
            if((!hasrun3) && (!felf) && (!c_Tower)&& (!c_area) && (!c_Elf) &&(!c_Tank)&& (!fhorse) && (!fnurse) && (!ftank))
                setPos(x()-step,y());
            tank->health->cure(breast);
            hasrun3 = true;
            ftank = true;
        }
    }

}
