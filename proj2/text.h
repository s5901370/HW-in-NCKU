#ifndef TEXT_H
#define TEXT_H

#include <QGraphicsTextItem>"
#include <QString>"
class Text:public QGraphicsTextItem{
public:
    Text(QString name,int x,int y);
    QGraphicsTextItem *t;
};

#endif // TEXT_H
