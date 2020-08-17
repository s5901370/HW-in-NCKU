#include "tower.h"
#include "e_elf.h"
#include <QPixmap>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPolygonF>
#include <QDebug>
#include "game.h"
#include "e_tank.h"
#include "e_horse.h"
extern Game *game;

Tower::Tower(QGraphicsItem *parent):QObject(), QGraphicsPixmapItem(parent),attack_damage(0.4),towerhp(100),hp(100){
    setPixmap(QPixmap(":/images/cute/castle (2).png"));


    //create attack area (1,0) (2,0)(3,1)(3,2)(2,3)(1,3)(1,0)
    QVector<QPointF> points;
    points << QPoint(3,0) << QPoint(2,0) << QPoint(1,1) << QPoint(1,2) << QPoint(2,3) << QPoint(3,3) ;
    //QPoint(1,0) << QPoint(2,0) << QPoint(3,1) << QPoint(3,2) << QPoint(2,3) << QPoint(1,3) << QPoint(1,0);
    //increase points
    int SCALE_FACTOR = 100;
    for (size_t i = 0, n = points.size(); i < n; i++){
        points[i] *= SCALE_FACTOR;
    }
    // create the QGraphicsPolygonItem
    attack_area = new QGraphicsPolygonItem(QPolygonF(points),this);

    //move area
    QPointF area_center (1.5,1.5);
    area_center *= SCALE_FACTOR;
    area_center = mapToScene(area_center);
    QPointF tower_center(x(),y()+65);
    QLineF ln(area_center,tower_center);
    attack_area->setPos(x()+ln.dx(),y()+ln.dy());

    //add hp
    health = new Health(1100,225,0,hp);//perfect
    game->scene->addItem(health);

    //attack enemy
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(detect_enemy()));
    timer->start(20);
}

void Tower::tower_be_damaged(double damage){
    if(towerhp < damage)
        towerhp = 0;
    else
        towerhp -= damage;
}

void Tower::detect_enemy(){
    if(towerhp == 0){
        disconnect(timer,SIGNAL(timeout()),this,SLOT(detect_enemy()));
        game->gameover();        
    }
    //get colliding item
    QList<QGraphicsItem *> colliding_items = attack_area->collidingItems();
    if(colliding_items.size() == 1)
        return;
    for(int i=0,n=colliding_items.size();i<n;i++){
        E_Elf *elf = dynamic_cast<E_Elf *>(colliding_items[i]);
        E_Horse *horse = dynamic_cast<E_Horse *>(colliding_items[i]);
        E_Tank *tank = dynamic_cast<E_Tank *>(colliding_items[i]);
        if(elf){
            elf->health->hurt(attack_damage);//elf be attacked
        }
        else if(horse){
            horse->health->hurt(attack_damage);
        }
        else if(tank){
            tank->health->hurt(attack_damage);
        }
    }


}

