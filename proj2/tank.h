#ifndef TANK_H
#define TANK_H

#include"health.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
class Tank:public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Tank(int x, int y);//give pos
    double attack_damage;
    Health *health;

    bool c_Tower;//colliding things
    bool c_Elf;
    bool c_area;
    bool c_Tank;
    QTimer *timer;

    int step;
    static int mortal;
private:
    double hp;
public slots:
    void move();
};

#endif // TANK_H
