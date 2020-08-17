#include "card.h"
#include <QDebug>
#include "elf.h"
#include <game.h>
#include "health.h"
#include "mana.h"
extern Game *game;
Card::Card(int x, int y, QPixmap path, int mana):need(mana),xPos(x),yPos(y){
    setPixmap(path);//64pixel
    setPos(xPos,yPos);
    //allow responding to hover events
    setAcceptHoverEvents(true);
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/images/need_mana.aac"));
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setPixmap(QPixmap(":/images/card/d1.png"));
    if(need > game->mana->getMana()){
        if (music->state() == QMediaPlayer::PlayingState){
                    music->setPosition(0);
                }
                else if (music->state() == QMediaPlayer::StoppedState){
                    music->play();
                }
    }
    else {
        if(!game->elf){
            game->mana->consume(need);
            game->elf = new Elf(900,300);
            game->scene->addItem(game->elf);
            game->elf = nullptr;
        }
    }
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos-5);
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos);
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    setPixmap(QPixmap(":/images/card/1.png"));
}

