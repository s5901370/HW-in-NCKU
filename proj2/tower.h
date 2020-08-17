#ifndef TOWER_H
#define TOWER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsItem>
#include <QPointF>
#include <QObject>
#include "health.h"
#include <QTimer>
class Tower:public QObject, public QGraphicsPixmapItem{
Q_OBJECT
public:
    Tower(QGraphicsItem * parent=0);
    //double distance_to(QGraphicsItem *item);//distance to this item
    QGraphicsPolygonItem *attack_area;
    double attack_damage;
    Health *health;
    QTimer *timer;
    double towerhp;// for end game

    void attack();
    void tower_be_damaged(double damage);

private:
    double hp;
public slots:
    void detect_enemy();

};


#endif // TOWER_H
