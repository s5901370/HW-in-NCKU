#include "ai.h"
#include <QObject>
#include "game.h"
#include <stdlib.h>
#include <time.h>
#include "QDebug"
extern Game *game;
AI::AI(int sec){
    srand(time(NULL));
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(spawn()));
    timer->start(sec);
}

void AI::spawn(){
    if(game->tower->towerhp == 0||game->e_tower->towerhp == 0){
        disconnect(timer,SIGNAL(timeout()),this,SLOT(spawn()));
    }
    int x = rand();
    switch(x%10){
        case 0:case 1:case 3:case 5:case 7:
            elf = new E_Elf(200,300);
            game->scene->addItem(elf);
            elf = nullptr;
            break;
        case 6:case 2:case 9:
            horse = new E_Horse(200,300);
            game->scene->addItem(horse);
            horse = nullptr;
            break;
        case 4:case 8:
            tank = new E_Tank(200,300);
            game->scene->addItem(tank);
            tank = nullptr;
            break;
    }

}
