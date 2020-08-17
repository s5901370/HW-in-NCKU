#include "text.h"
#include <QFont>
#include "game.h"
extern Game *game;
Text::Text(QString name,int x,int y){
    t = new QGraphicsTextItem(name);
    t->setFont(QFont("Timers",16));
    t->setPos(x,y);
    game->scene->addItem(t);
}


