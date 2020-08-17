#include "e_tank.h"
#include "e_elf.h"
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <game.h>
#include "tower.h"
#include "elf.h"
#include "e_elf.h"
#include "watermelon.h"
#include "e_horse.h"
#include "tank.h"
extern Game *game;
int E_Tank :: mortal = 0;
E_Tank::E_Tank(int x,int y):step(7),attack_damage(5),c_Tower(false),hp(70){
    //set pos
    setPos(x,y);
    //set graph
    setPixmap(QPixmap(":/images/cute/e_tank.png"));

    c_Tower = false;
    c_Elf = false;
    c_Water = false;
    c_Nurse = false;
    c_Tank = false;

    //create hp
    health = new Health(x,y,-step,hp);//stepSize
    game->scene->addItem(health);

    //move
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(80);//200 ms

}

void E_Tank::move(){
    if(health->getHp() == 0){//dead
        mortal++;
        delete this;
    }
    if(game->tower->towerhp == 0 || game->e_tower->towerhp == 0){
        disconnect(timer,SIGNAL(timeout()),this,SLOT(move()));
    }

    //true or false
    bool hasrun = false;
    bool hasrun2 = false;
    bool hasrun3 = false;
    bool felf = false;
    bool ftank = false;
    bool fhorse = false;

    //get colliding item
    QList<QGraphicsItem *> colliding_items = this->collidingItems();
    //nothing collided
    if(colliding_items.size() == 0){
        setPos(x()+step,y());
        return;
    }
    for(int i=0,n=colliding_items.size();i<n;i++){
        Tower *tower = dynamic_cast<Tower *>(colliding_items[i]);
        QGraphicsPolygonItem *attack_area = dynamic_cast<QGraphicsPolygonItem *>(colliding_items[i]);
        Elf *elf = dynamic_cast<Elf *>(colliding_items[i]);
        E_Elf *e_elf = dynamic_cast<E_Elf *>(colliding_items[i]);
        Watermelon *water = dynamic_cast<Watermelon *>(colliding_items[i]);
        Nurse *nurse = dynamic_cast<Nurse *>(colliding_items[i]);
        E_Horse *horse = dynamic_cast<E_Horse *>(colliding_items[i]);
        Tank *tank = dynamic_cast<Tank *>(colliding_items[i]);
        E_Tank *e_tank = dynamic_cast<E_Tank *>(colliding_items[i]);
        if(tower){
            c_Tower = true;
            health->colliding_st = true;
            tower->health->hurt(attack_damage-2);//tower be attacked
            tower->tower_be_damaged(attack_damage-2);
            return;//final destination
        }
        else if (attack_area && (!c_Tower)){
            setPos(x()+step,y());
        }
        else if(elf){//enemy
            c_Elf = true;
            health->colliding_st = true;//stop hp
            if(elf->health->getHp()<=attack_damage){
                c_Elf = false;
                health->colliding_st = false;
            }
            elf->health->hurt(attack_damage);
        }
        else if(water){//enemy
            health->colliding_st = true;
            c_Water = true;
            water->health->hurt(attack_damage);
//            health->colliding_st = false;
        }
        else if(nurse){//enemy
            health->colliding_st=true;
            c_Nurse = true;
            if(nurse->health->getHp()<=attack_damage){
                c_Nurse = false;
                health->colliding_st = false;
            }
            nurse->health->hurt(attack_damage);
        }
        else if(tank){
            health->colliding_st=true;
            c_Tank = true;
            if(tank->health->getHp()<=attack_damage){
                c_Tank = false;
                health->colliding_st = false;
            }
            tank->health->hurt(attack_damage);
        }
        else if(e_elf && (!c_Tower) && (!c_Elf) && (!c_Nurse) && (!c_Tank)){//friend
            if((!hasrun)&& (!fhorse) && (!ftank))
                setPos(x()+step,y());
            hasrun = true;
            felf = true;
        }
        else if(horse && (!c_Tower) && (!c_Elf) && (!c_Nurse) && (!c_Tank)){
            if((!hasrun2) && (!felf) && (!ftank))
                setPos(x()+step,y());
            hasrun2 = true;
            fhorse = true;
        }
        else if(e_tank && (!c_Tower) && (!c_Elf) && (!c_Nurse) && (!c_Tank)){
            if((!hasrun3)&& (!fhorse) && (!felf))
                setPos(x()+step,y());
            hasrun3 = true;
            ftank = true ;
        }
    }
    if(c_Water)//let health go with e_elf
        health->colliding_st = false;
}
