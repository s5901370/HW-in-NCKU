#include "e_tower.h"
#include "elf.h"
#include <QPixmap>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPolygonF>
#include <QDebug>
#include "game.h"
#include "horse.h"
#include "tank.h"
extern Game *game;

E_Tower::E_Tower(QGraphicsItem *parent):QObject(), QGraphicsPixmapItem(parent),attack_damage(0.3),towerhp(100),hp(100){
    setPixmap(QPixmap(":/images/cute/e_castle.png"));


    //create attack area (1,0) (2,0)(3,1)(3,2)(2,3)(1,3)(1,0)
    QVector<QPointF> points;
    points << QPoint(1,0) << QPoint(2,0) << QPoint(3,1) << QPoint(3,2) << QPoint(2,3) << QPoint(1,3) << QPoint(1,0);

    //increase points
    int SCALE_FACTOR = 150;
    for (size_t i = 0, n = points.size(); i < n; i++){
        points[i] *= SCALE_FACTOR;
    }
    // create the QGraphicsPolygonItem
    attack_area = new QGraphicsPolygonItem(QPolygonF(points),this);

    //move area
    QPointF area_center (1.5,1.5);
    area_center *= SCALE_FACTOR;
    area_center = mapToScene(area_center);
    QPointF tower_center(x()+30,y()+80);
    QLineF ln(area_center,tower_center);
    attack_area->setPos(x()+ln.dx(),y()+ln.dy());

    //add hp
    health = new Health(30,225,0,hp);
    game->scene->addItem(health);

    //attack enemy
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(detect_enemy()));
    timer->start(20);
}

void E_Tower::tower_be_damaged(double damage){
    if(towerhp <= damage)
        towerhp = 0;
    else
        towerhp -= damage;
}

void E_Tower::detect_enemy(){
    if(towerhp == 0|| game->tower->towerhp == 0){
        disconnect(timer,SIGNAL(timeout()),this,SLOT(detect_enemy()));
        game->gameover();
    }
    //get colliding item
    QList<QGraphicsItem *> colliding_items = attack_area->collidingItems();
    if(colliding_items.size() == 1)
        return;
    for(int i=0,n=colliding_items.size();i<n;i++){
        Elf *elf = dynamic_cast<Elf *>(colliding_items[i]);
        Nurse *nurse = dynamic_cast<Nurse *>(colliding_items[i]);
        Horse *horse = dynamic_cast<Horse *>(colliding_items[i]);
        Tank *tank = dynamic_cast<Tank *>(colliding_items[i]);
        if(elf){
            elf->health->hurt(attack_damage);//elf be attacked
        }
        else if(nurse){
            nurse->health->hurt(attack_damage);
        }
        else if(horse){
            horse->health->hurt(attack_damage);
        }
        else if(tank){
            tank->health->hurt(attack_damage);
        }
    }


}


