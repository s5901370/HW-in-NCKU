#include "card2.h"
#include "card.h"
#include <QDebug>
#include "elf.h"
#include <game.h>
#include "health.h"
#include "mana.h"
extern Game *game;
Card2::Card2(int x, int y, QPixmap path, int mana):need(mana),xPos(x),yPos(y){
    setPixmap(path);//64pixel
    setPos(xPos,yPos);
    //allow responding to hover events
    setAcceptHoverEvents(true);
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/images/need_mana.aac"));
}

void Card2::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setPixmap(QPixmap(":/images/card/d2.png"));
    if(need > game->mana->getMana()){
        if (music->state() == QMediaPlayer::PlayingState){
                    music->setPosition(0);
                }
                else if (music->state() == QMediaPlayer::StoppedState){
                    music->play();
                }
    }
    else {
        if(!game->water){
            game->mana->consume(need);
            if(game->water->count<=2){
                int x = qrand()%1200;
                game->water = new Watermelon(x,100);
                game->water->count++;
            }
            else{
                int x = qrand()%700;
                game->water = new Watermelon(x+200,300);
            }
            game->scene->addItem(game->water);
            game->water = nullptr;
        }
    }
}

void Card2::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos-5);
}

void Card2::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    setPos(xPos,yPos);
}

void Card2::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    setPixmap(QPixmap(":/images/card/2.png"));
}

