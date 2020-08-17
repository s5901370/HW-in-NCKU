#ifndef HORSE_H
#define HORSE_H

#include"health.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Horse:public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Horse(int x, int y);//give pos
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

#endif // HORSE_H
