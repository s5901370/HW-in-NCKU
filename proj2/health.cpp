#include "health.h"
#include <QPixmap>
#include <QTimer>
#include "tower.h"
#include "game.h"
#include <QDebug>
extern Game *game;
Health::Health(int xPos, int yPos, int step, double h):stepSize(step),hp(h),colliding_Tower(false){
    hpmax = h;
    //set pos at elf
    setPos(xPos,yPos-10);
    setPixmap(QPixmap(":/images/hp/hp10.png"));


    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(80);//500
}

void Health::hurt(double damage){
    if(hp<damage){
        hp = 0;
    }//dead
    else{
        hp-=damage;
    }
}

void Health::cure(double good){
    if((hp+good)>hpmax)
        hp = hpmax;
    else
        hp += good;
}

double Health::getHp(){
    return hp;
}

void Health::move()
{
    if(game->tower->towerhp == 0 || game->e_tower->towerhp == 0){
        disconnect(timer,SIGNAL(timeout()),this,SLOT(move()));
    }
    if(hp == 0){//dead
        scene()->removeItem(this);
        delete this;
    }
    else if(hp>=80&&hp<90){
        setPixmap(QPixmap(":/images/hp/hp9.PNG"));
    }
    else if(hp>=70&&hp<80){
        setPixmap(QPixmap(":/images/hp/hp8.PNG"));
    }
    else if(hp>=60&&hp<70){
        setPixmap(QPixmap(":/images/hp/hp7.PNG"));
    }
    else if(hp>=50&&hp<60){
        setPixmap(QPixmap(":/images/hp/hp6.PNG"));
    }
    else if(hp>=40&&hp<50){
        setPixmap(QPixmap(":/images/hp/hp5.png"));
    }
    else if(hp>=30&&hp<40){
        setPixmap(QPixmap(":/images/hp/hp4.png"));
    }
    else if(hp>=20&&hp<30){
        setPixmap(QPixmap(":/images/hp/hp3.png"));
    }
    else if(hp>=10&&hp<20){
        setPixmap(QPixmap(":/images/hp/hp2.png"));
    }
    else if(hp>0&&hp<10){
        setPixmap(QPixmap(":/images/hp/hp1.png"));
    }

    if(!colliding_st){// does not colliding
        setPos(x()-stepSize,y());
    }
}

