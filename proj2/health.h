#ifndef HEALTH_H
#define HEALTH_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Health:public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Health(int xPos, int yPos, int stepSize, double h);//h most is 100
    void hurt(double damage);
    void cure(double good);//increase hp
    double getHp();

    bool colliding_Tower;
    bool colliding_st;//colliding others, not area

    QTimer *timer;

public slots:
    void move();

private:
    double hp;
    double hpmax;
    int stepSize;

};

#endif // HEALTH_H
