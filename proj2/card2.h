#ifndef CARD2_H
#define CARD2_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QMediaPlayer>
class Card2:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    // constructors
    Card2(int x,int y,QPixmap path,int mana=0);
    QMediaPlayer *music;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    int need;//mana
    int xPos;
    int yPos;

signals:
    void clicked();
};
#endif // CARD2_H
