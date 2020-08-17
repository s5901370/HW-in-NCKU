#ifndef MANA_H
#define MANA_H

#include <QGraphicsTextItem>
#include <QTimer>
class Mana:public QGraphicsTextItem{
    Q_OBJECT
public:
    Mana(QGraphicsItem *parnet =0);
    int getMana();
    QTimer *timer;
    static int add;
    void consume(int need);

public slots:
    void increase();
private:
    int mana;
};

#endif // MANA_H
