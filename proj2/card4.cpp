#include "card4.h"
#include <QDebug>
#include "horse.h"
#include <game.h>
#include "health.h"
#include "mana.h"
extern Game *game;
Card4::Card4(int x, int y, QPixmap path, int mana):need(mana),xPos(x),yPos(y){
    setPixmap(path);//64pixel
    setPos(xPos,yPos);
    //allow responding to hover events
    setAcceptHoverEvents(true);
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/images/need_mana.aac"));
}

void Card4::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setPixmap(QPixmap(":/images/card/d4.png"));
    if(need > game->mana->getMana()){
        if (music->state() == QMediaPlayer::PlayingState){
                    music->setPosition(0);
                }
                else if (music->state() == QMediaPlayer::StoppedState){
                    music->play();
                }
    }
    else {
        if(!game->horse){
            game->mana->consume(need);
            game->horse = new Horse(900,300);
            game->scene->addItem(game->horse);
            game->horse = nullptr;
        }
    }
}

void Card4::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos-5);
}

void Card4::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos);
}

void Card4::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
   setPixmap(QPixmap(":/images/card/4.png"));
}

