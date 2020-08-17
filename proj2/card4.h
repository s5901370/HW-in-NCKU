#ifndef CARD4_H
#define CARD4_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QMediaPlayer>
class Card4:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    // constructors
    Card4(int x,int y,QPixmap path,int mana=0);
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
#endif // CARD4_H
