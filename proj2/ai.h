#ifndef AI_H
#define AI_H

#include <QTimer>
#include "e_elf.h"
#include "e_horse.h"
#include "e_tank.h"
class AI:public QObject{
        Q_OBJECT
public:
    AI(int sec);
    QTimer *timer;
    E_Elf *elf;
    E_Horse *horse;
    E_Tank *tank;
public slots:
    void spawn();
};

#endif // AI_H
