#include "horse.h"
#include "elf.h"
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <game.h>
#include "tower.h"
#include "e_elf.h"
#include "e_tower.h"
#include "watermelon.h"
#include "e_horse.h"
#include "tank.h"
#include "e_tank.h"
extern Game *game;
int Horse :: mortal = 0;
Horse::Horse(int x,int y):attack_damage(90),step(15),hp(10){
    //set pos
    setPos(x,y);
    //set graph
    setPixmap(QPixmap(":/images/cute/horse.png"));

    //create hp
    health = new Health(x,y,step,hp);
    game->scene->addItem(health);

    c_Tower = false;
    c_Elf = false;
    c_area = false;
    c_Tank = false;
    //move
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(80);//200 ms


}


void Horse::move(){
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
        setPos(x()-step,y());
        return;
    }
    //true or false good job!
    bool hasrun = false;//meet elf has run 1 time
    bool hasrun1 = false;// meet water
    bool felf = false;//meet friend elf
    bool fhorse = false;
    bool fnurse = false;
    bool ftank = false;
    bool fwater = false;
    bool hasrun2 = false;
    bool hasrun3 = false;
    bool killed = false;//if horse has killed enemy ,it will die
    bool hasrun4 = false;

    int i = 0;
    for(int i=0,n=colliding_items.size();i<n;i++){
        E_Tower *tower = dynamic_cast<E_Tower *>(colliding_items[i]);//colliding tower
        QGraphicsPolygonItem *attack_area = dynamic_cast<QGraphicsPolygonItem *>(colliding_items[i]);
        E_Elf *e_elf = dynamic_cast<E_Elf *>(colliding_items[i]);
        Elf *elf = dynamic_cast<Elf *>(colliding_items[i]);
        Watermelon *water = dynamic_cast<Watermelon *>(colliding_items[i]);
        Nurse *nurse = dynamic_cast<Nurse *>(colliding_items[i]);
        E_Horse *e_horse = dynamic_cast<E_Horse *>(colliding_items[i]);
        Horse *horse = dynamic_cast<Horse *>(colliding_items[i]);
        Tank *tank = dynamic_cast<Tank *>(colliding_items[i]);
        E_Tank *e_tank = dynamic_cast<E_Tank *>(colliding_items[i]);
        if(tower){
            c_Tower = true;
            health->colliding_st = true;//stop hp
            tower->health->hurt(1);//tower be attacked
            tower->tower_be_damaged(1);// a horse cannot hurt tower
            return;//final destination
        }
        else if (attack_area && (!c_Tower)){
            c_area = true;
            setPos(x()-step,y());
        }
        else if(e_elf){//enemy
            killed = true;
            c_Elf = true;
            health->colliding_st = true;//stop hp
//            if(e_elf->health->getHp()<=attack_damage){// must have = !!!!!
//                c_Elf = false;
//                health->colliding_st = false;
//            }
            e_elf->health->hurt(attack_damage);
        }
        else if(e_tank){
            killed = true;
            health->colliding_st = true;//stop hp
//            if(e_tank->health->getHp()<=attack_damage){// must have = !!!!!
//                c_Tank = false;
//                health->colliding_st = false;
//            }
            e_tank->health->hurt(attack_damage);
        }
        else if(e_horse){
            health->colliding_st = true;
            killed = true;
        }
        else if(elf && (!c_Tower) && (!c_Elf) && (!c_area)&& (!c_Tank)){//friend
                if((!hasrun) && (!fhorse) && (!fnurse) && (!ftank) && (!fwater))
                    setPos(x()-step,y());
                hasrun = true;
                felf = true;
        }
        else if(horse && (!c_Tower) && (!c_Elf) && (!c_area) && (!c_Tank)){
            if((!hasrun4) && (!felf) && (!ftank) && (!fnurse))
                setPos(x()-step,y());
            hasrun4 = true;
            fhorse = true;
        }
        else if(water && (!c_Tower) && (!c_Elf) && (!c_area) && (!c_Tank) ){//friend, does not meet enemy
            if((!felf)&& (!fhorse) && (!fnurse) && (!ftank) && (!hasrun1))// if meet elf --> cannot move
                setPos(x()-step,y());
            fwater = true;
            hasrun1 = true;
        }
        else if(nurse && (!c_Tower) && (!c_Elf) && (!c_area) && (!c_Tank)){//friend
            if((!hasrun2) && (!felf) && (!fhorse) && (!ftank) && (!fwater))
                setPos(x()-step,y());
            hasrun2 = true;
            fnurse = true;
        }
        else if(tank && (!c_Tower) && (!c_Elf) && (!c_area) && (!c_Tank)){
            if((!hasrun3) && (!felf) && (!fhorse) && (!fnurse) && (!fwater))
                setPos(x()-step,y());
            hasrun3 = true;
        }
    }
    if(killed){
        health->hurt(hp);
    }

}
