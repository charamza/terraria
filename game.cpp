#include "game.h"
#include <QDebug>
#include <time.h>
#include "block.h"
//#include "windows.h"
//#include <X11/Xlib.h>
#include "view.h"
#include <QApplication>
#include <QDesktopWidget>

Game::Game(View *view)
{
    this->view = view;
    this->scene = view->scene();
    QRect resolution = QApplication::desktop()->screenGeometry();
    this->WIDTH = resolution.width();
    this->HEIGHT = resolution.height();
    time.start();
}

void Game::load()
{
    this->camera = new Camera(this);
    this->world = new World(this);
    this->world->load();
    this->world->gen->generateFauna();
    this->player = new Player(this, World::WIDTH*Block::SIZE/2, -100);
    this->inventory = new Inventory(this);
    getDelta(); // Iniciální delta

    textureNPC.load(":/res/npc.png");
    textureWasted.load(":/res/wasted.png");
}

void Game::update(){
    int delta = getDelta();

    world->update(16);
    player->update(16);
    camera->update();
    inventory->update();

    countFPS();
}

int Game::getDelta()
{
    long ms = time.elapsed();
    int delta = ms - timed;
    timed = ms;
    return delta;
}


void Game::countFPS(){
    int t = time.elapsed() / 1000;
    if(lastTime == t){
        afps++;
    }else{
        fps = afps;
        afps = 1;
        lastTime = t;
        qDebug() << "FPS: " << fps;
    }
}
