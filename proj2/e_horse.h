#ifndef E_HORSE_H
#define E_HORSE_H

#include"health.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
class E_Horse:public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    E_Horse(int x, int y);//give pos
    double attack_damage;
    Health *health;
    bool c_Tower;
    bool c_Elf;
    bool c_Water;
    bool c_Nurse;
    bool c_Tank;
    QTimer *timer;
    int step;
    static int mortal;
private:
    double hp;
public slots:
    void move();
};
#endif // E_HORSE_H
