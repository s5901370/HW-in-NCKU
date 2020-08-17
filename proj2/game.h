#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QObject>
#include <mana.h>
#include <elf.h>
#include <tower.h>
#include "e_elf.h"
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include "e_tower.h"
#include "watermelon.h"
#include "nurse.h"
#include "horse.h"
#include "tank.h"
#include "e_tank.h"
#include "horse.h"
class Game:public QGraphicsView{
    Q_OBJECT
public:

    void displayMainMenu();
    void gameover();
    void start();

    Game(QWidget *parent = NULL);
    QGraphicsScene *scene;
    Mana *mana;
    Elf *elf;
    E_Elf *elf2;
    Tower *tower;
    E_Tower *e_tower;
    Watermelon *water;
    Nurse *nurse;
    Horse *horse;
    Tank *tank;

    bool easy;
    bool medium;
    bool hard;
    bool impossible;

public slots:
    void Easy();
    void Medium();
    void Hard();
    void Impossible();
    void restart();
    };

#endif // GAME_H
