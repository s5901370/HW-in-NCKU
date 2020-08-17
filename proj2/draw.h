#ifndef DRAW_H
#define DRAW_H

#include <QGraphicsRectItem>
class Draw:public QGraphicsRectItem{
public:
    Draw(int x, int y, int w, int h, double t, QColor color);
    QGraphicsRectItem *rect;
};

#endif // DRAW_H
