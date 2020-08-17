#include "watermelon.h"
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <game.h>
#include "tower.h"
#include "e_tower.h"
#include "mana.h"
extern Game *game;
int Watermelon :: mortal = 0;
int Watermelon :: count = 0;
Watermelon::Watermelon(int x,int y):step(0),hp(1){
    //set pos
    setPos(x,y);
    //set graph
    setPixmap(QPixmap(":/images/cute/watermelon.png"));

    //create hp
    health = new Health(x,y,step,hp);
    game->scene->addItem(health);
    //move
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(80);//200 ms
    Mana::add+=5;
}

void Watermelon::move(){
    if(health->getHp() == 0){//dead
        mortal++;
        Mana::add-=5;
        delete this;
    }
    if(game->tower->towerhp == 0 || game->e_tower->towerhp == 0 ){
        disconnect(timer,SIGNAL(timeout()),this,SLOT(move()));
    }
}
