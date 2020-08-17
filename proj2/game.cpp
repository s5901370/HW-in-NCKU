#include "game.h"
#include <QBrush>
#include <QImage>
#include "tower.h"
#include "button.h"
#include "card.h"
#include <QGraphicsTextItem>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QPixmap>
#include "draw.h"
#include "ai.h"
#include "card2.h"
#include "card3.h"
#include "card4.h"
#include "card5.h"
#include "text.h"
Game::Game(QWidget *parent):QGraphicsView(){
    //create scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1200,600);

    //set scene
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1200,600);
    setBackgroundBrush(QBrush(QImage(":/images/cute/background.png")));


    //set cursor
    //cursor = nullptr;
}

void Game::start(){

    scene->clear();

    //set AI easy 4000
    //medium 3000
    //hard 2000
    //impossible 1000
    int sec;

    //easy=true;// delete this                 here    watch out!


    if(easy){
        sec = 4000;}
    else if(medium){
        sec = 2000;}
    else if(hard){
        sec = 1500;}
    else if(impossible){
        sec = 1000;}
    else{
        sec = 4000;}//by default
    AI *ai = new AI(sec);


    //add E_tower
    e_tower = new E_Tower();
    scene->addItem(e_tower);
    e_tower->setPos(0,225);
    //add ourTower
    tower = new Tower();
    scene->addItem(tower);
    tower->setPos(1070,225);

    //set mana
    mana = new Mana();
    scene->addItem(mana);
    mana->add = 10;//5



    //create deck adjust 40%

    Card *card1 = new Card(300,400,QString(":/images/card/1.png"),20);//set mana need
    scene->addItem(card1);

    Card2 *card2 = new Card2(400,400,QString(":/images/card/2.png"),100);//watermelon
    scene->addItem(card2);
    Watermelon :: count = 0;

    Card3 *card3 = new Card3(500,400,QString(":/images/card/3.png"),25);//nurse
    scene->addItem(card3);

    Card4 *card4 = new Card4(600,400,QString(":/images/card/4.png"),50);//horse
    scene->addItem(card4);

    Card5 *card5 = new Card5(700,400,QString(":/images/card/5.png"),40);//tank
    scene->addItem(card5);
    //reset mortality
    Elf::mortal = 0;
    Horse::mortal = 0;
    Nurse::mortal = 0;
    Watermelon::mortal = 0;
    Tank::mortal = 0;
    E_Elf::mortal = 0;
    E_Horse::mortal = 0;
    E_Tank::mortal = 0;

}

void Game::displayMainMenu(){
    //set title
    QGraphicsTextItem *title = new QGraphicsTextItem(QString("Country Battle"));
    QFont titleFont("comic sans",50);
    title->setFont(titleFont);
    int tx = this->width()/2 - title->boundingRect().width()/2;
    int ty = 100;
    title->setPos(tx,ty);
    scene->addItem(title);

    //create play button
//    Button *playButton = new Button(QString("Play"));
//    int bx = this->width()/2 - playButton->boundingRect().width()/2;;
//    int by = 200;
//    playButton->setPos(bx,by);
//    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));
//    scene->addItem(playButton);
    //easy button
    Button *easyButton = new Button(QString("Easy"));
    int ex = this->width()/2 - easyButton->boundingRect().width()/2;;
    int ey = 200;
    easyButton->setPos(ex,ey);
    connect(easyButton,SIGNAL(clicked()),this,SLOT(Easy()));
    scene->addItem(easyButton);
    //medium
    Button *medButton = new Button(QString("Medium"));
    int mx = this->width()/2 - medButton->boundingRect().width()/2;;
    int my = 250;
    medButton->setPos(mx,my);
    connect(medButton,SIGNAL(clicked()),this,SLOT(Medium()));
    scene->addItem(medButton);
    //hard
    Button *hardButton = new Button(QString("Hard"));
    int hx = this->width()/2 - hardButton->boundingRect().width()/2;;
    int hy = 300;
    hardButton->setPos(hx,hy);
    connect(hardButton,SIGNAL(clicked()),this,SLOT(Hard()));
    scene->addItem(hardButton);
    //impo
    Button *imButton = new Button(QString("Impopopossible"));
    int ix = this->width()/2 - imButton->boundingRect().width()/2;;
    int iy = 350;
    imButton->setPos(ix,iy);
    connect(imButton,SIGNAL(clicked()),this,SLOT(Impossible()));
    scene->addItem(imButton);
    //create quit button
    Button *quitButton = new Button(QString("Quit"));
    int qx = this->width()/2 - quitButton->boundingRect().width()/2;
    int qy = 400;
    quitButton->setPos(qx,qy);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);
}
void Game::Easy(){
    easy = true;
    medium = false;
    hard = false;
    impossible = false;
    start();
}

void Game::Medium(){
    easy = false;
    medium = true;
    hard = false;
    impossible = false;
    start();
}

void Game::Hard(){
    easy = false;
    medium = false;
    impossible = false;
    hard = true;
    start();
}

void Game::Impossible(){
    easy = false;
    medium = false;
    hard = false;
    impossible = true;
    start();
}

void Game::gameover(){
    // I don't know whether it is working
    for(int i=0,n=scene->items().size();i<n;i++){
        scene->items()[i]->setEnabled(false);
    }
    //create transparent rect
    Draw *rect = new Draw(0,0,1200,800,0.65,Qt::black);
    scene->addItem(rect->rect);

    Draw *square = new Draw(450,100,300,300,0.75,Qt::lightGray);
    scene->addItem(square->rect);

    //create mortality
    Draw *left = new Draw(50,50,400,500,0.65,Qt::darkGray);
    scene->addItem(left->rect);
    QGraphicsTextItem *ltitle = new QGraphicsTextItem("Enemy Mortality");
    QFont lfont("Courier",20);
    ltitle->setFont(lfont);
    ltitle->setPos(125,100);
    scene->addItem(ltitle);

    Draw *right = new Draw(750,50,400,500,0.65,Qt::darkGray);
    scene->addItem(right->rect);
    QGraphicsTextItem *rtitle = new QGraphicsTextItem("Army Mortality");
    QFont rfont("Courier",20);
    rtitle->setFont(rfont);
    rtitle->setPos(825,100);
    scene->addItem(rtitle);

    //set title who win
    QString end,end1,end2;
    end1 = "You win!!";
    end2 = "You lose QAQ";
    if(e_tower->towerhp == 0){//win
         end = end1;
    }
    else{//lose
        end = end2;
    }
    QGraphicsTextItem *title = new QGraphicsTextItem(end);
    QFont titleFont("times",20);
    title->setFont(titleFont);
    if(e_tower->towerhp == 0){//win
         title->setPos(550,100);
    }
    else{//lose
        title->setPos(520,100);
    }

    scene->addItem(title);

    //create play again button
    Button *again = new Button(QString("Play Again"));
    int ax = this->width()/2 - again->boundingRect().width()/2;;
    int ay = 200;
    again->setPos(ax,ay);
    scene->addItem(again);
    connect(again,SIGNAL(clicked()),this,SLOT(restart()));

    // create quit button

    Button *quit = new Button(QString("Quit"));
    int qx = this->width()/2 - quit->boundingRect().width()/2;
    int qy = 300;
    quit->setPos(qx,qy);
    scene->addItem(quit);
    connect(quit,SIGNAL(clicked()),this,SLOT(close()));

    Text *textl1 = new Text(QString("Elf : ")+QString::number(E_Elf::mortal),200,150);
    Text *textl2 = new Text(QString("Horse : ")+QString::number(E_Horse::mortal),200,200);
    Text *textl3 = new Text(QString("Tank : ")+QString::number(E_Tank::mortal),200,250);
    Text *textr1 = new Text(QString("Elf : ")+QString::number(Elf::mortal),900,150);
    Text *textr2 = new Text(QString("Horse : ")+QString::number(Horse::mortal),900,200);
    Text *textr3 = new Text(QString("Tank : ")+QString::number(Tank::mortal),900,250);
    Text *textr4 = new Text(QString("Nurse : ")+QString::number(Nurse::mortal),900,300);
    Text *textr5 = new Text(QString("Watermelon : ")+QString::number(Watermelon::mortal),900,350);
}

void Game::restart(){
    scene->clear();
    displayMainMenu();
}
