#include "card5.h"
#include <QDebug>
#include "tank.h"
#include <game.h>
#include "health.h"
#include "mana.h"
extern Game *game;
Card5::Card5(int x, int y, QPixmap path, int mana):need(mana),xPos(x),yPos(y){
    setPixmap(path);//64pixel
    setPos(xPos,yPos);
    //allow responding to hover events
    setAcceptHoverEvents(true);
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/images/need_mana.aac"));
}

void Card5::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setPixmap(QPixmap(":/images/card/d5.png"));
    if(need > game->mana->getMana()){
        if (music->state() == QMediaPlayer::PlayingState){
                    music->setPosition(0);
                }
                else if (music->state() == QMediaPlayer::StoppedState){
                    music->play();
                }
    }
    else {
        if(!game->tank){
            game->mana->consume(need);
            game->tank = new Tank(900,300);
            game->scene->addItem(game->tank);
            game->tank = nullptr;
        }
    }
}

void Card5::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos-5);
}

void Card5::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos);
}

void Card5::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    setPixmap(QPixmap(":/images/card/5.png"));
}

