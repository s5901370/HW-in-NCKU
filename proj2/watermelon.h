#ifndef WATERMELON_H
#define WATERMELON_H

#include"health.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
class Watermelon:public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Watermelon(int x, int y);//give pos
    Health *health;
    QTimer *timer;
    int step;
    static int mortal;
    static int count;
private:
    double hp;
public slots:
    void move();
};
#endif // WATERMELON_H
