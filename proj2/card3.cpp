#include "card3.h"
#include <QDebug>
#include "nurse.h"
#include <game.h>
#include "health.h"
#include "mana.h"
extern Game *game;
Card3::Card3(int x, int y, QPixmap path, int mana):need(mana),xPos(x),yPos(y){
    setPixmap(path);//64pixel
    setPos(xPos,yPos);
    //allow responding to hover events
    setAcceptHoverEvents(true);
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/images/need_mana.aac"));
}

void Card3::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setPixmap(QPixmap(":/images/card/d3.png"));
    if(need > game->mana->getMana()){
        if (music->state() == QMediaPlayer::PlayingState){
                    music->setPosition(0);
                }
                else if (music->state() == QMediaPlayer::StoppedState){
                    music->play();
                }
    }
    else {
        if(!game->nurse){
            game->mana->consume(need);
            game->nurse = new Nurse(900,300);
            game->scene->addItem(game->nurse);
            game->nurse = nullptr;
        }
    }
}

void Card3::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos-5);
}

void Card3::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos);
}

void Card3::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    setPixmap(QPixmap(":/images/card/3.png"));
}

