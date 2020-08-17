#ifndef E_TOWER_H
#define E_TOWER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsItem>
#include <QPointF>
#include <QObject>
#include "health.h"
#include <QTimer>
class E_Tower:public QObject, public QGraphicsPixmapItem{
Q_OBJECT
public:
    E_Tower(QGraphicsItem * parent=0);
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

#endif // E_TOWER_H
