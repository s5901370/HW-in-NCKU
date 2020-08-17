#ifndef NURSE_H
#define NURSE_H

#include"health.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
class Nurse:public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Nurse(int x, int y);//give pos
    double attack_damage;
    Health *health;

    bool c_Tower;//colliding things
    bool c_Elf;
    bool c_area;
    bool c_Tank;
    bool c_Horse;
    double breast;//heal

    QTimer *timer;

    int step;
    static int mortal;
private:
    double hp;
public slots:
    void move();
};

#endif // NURSE_H
