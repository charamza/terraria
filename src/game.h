#ifndef GAME_H
#define GAME_H

#include <QList>
#include <QGraphicsScene>
#include <QTime>
#include "camera.h"
#include "world.h"
#include "player.h"
#include "inventory.h"

class View;

class Game
{
public:
    Game(View *view);
    void load();
    void update();
    int WIDTH, HEIGHT;
    //const static int WIDTH = 1440, HEIGHT = 900;
    //const static int WIDTH = 1920, HEIGHT = 1080;
    //const static int WIDTH = 800, HEIGHT = 600;

    Camera *camera;
    World *world;
    Player *player;
    Inventory *inventory;
    View *view;
    QGraphicsScene *scene;

    QTime time;
    long timed = 0;

    QImage textureNPC;
    QImage textureWasted;

private:
    int lastTime = 0, fps = 0, afps = 0;
    int getDelta();
    void countFPS();
};

#endif // GAME_H
