#ifndef E_ELF_H
#define E_ELF_H

#include"health.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
class E_Elf:public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    E_Elf(int x, int y);//give pos
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

#endif // E_ELF_H
