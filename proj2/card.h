#ifndef CARD_H
#define CARD_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QMediaPlayer>
class Card:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    // constructors
    Card(int x,int y,QPixmap path,int mana=0);
    QMediaPlayer *music;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
public slots:
    //void
private:
    int need;//mana
    int xPos;
    int yPos;

signals:
    void clicked();
};
#endif // CARD_H
