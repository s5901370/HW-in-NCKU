#include "mana.h"
#include <QTimer>
#include <game.h>
extern Game *game;
Mana::Mana(QGraphicsItem *parnet):mana(0){
    setPlainText(QString("Mana : ")+QString::number(mana));
    setDefaultTextColor(Qt::blue);
    setFont(QFont("Timers",16));
    //increase mana per sec
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(increase()));
    timer->start(500);
}

int Mana::getMana(){
return mana;
}

void Mana::consume(int need){
    if(need <= mana)
        mana-=need;
}
int Mana::add = 5;
void Mana::increase(){
    mana+=add;
    setPlainText(QString("Mana : ")+QString::number(mana));
    if(game->tower->towerhp == 0)
        disconnect(timer,SIGNAL(timeout()),this,SLOT(increase()));
}
