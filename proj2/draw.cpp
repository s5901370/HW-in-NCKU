#include "draw.h"
#include <QBrush>
#include <QDebug>
#include "game.h"
extern Game *game;
Draw::Draw(int x,int y,int w,int h,double t,QColor color){
    rect = new QGraphicsRectItem(x,y,w,h);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    rect->setBrush(brush);
    rect->setOpacity(t);

}
