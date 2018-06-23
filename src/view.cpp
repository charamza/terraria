#include "view.h"
#include <QOpenGLWidget>
#include <QDebug>
#include "block.h"
#include <QApplication>

View::View(QGraphicsScene *scene)
{
    setScene(scene);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewport(new QOpenGLWidget(this));
    setMouseTracking(true);

    game = new Game(this);
    game->load();

    timerUpdate = new QTimer(this);
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(update()));
    timerUpdate->start(16);
}

void View::update()
{
    game->update();

    this->scene()->update(QRectF(0, 0, game->WIDTH, game->HEIGHT));
}

void View::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        game->camera->clickLeft = true;
    if(event->button() == Qt::RightButton)
        game->camera->clickRight = true;
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        game->camera->clickLeft = false;
    if(event->button() == Qt::RightButton)
        game->camera->clickRight = false;
}

void View::mouseMoveEvent(QMouseEvent *event){
    game->camera->mouse = event->pos();
}

void View::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Escape: {game->world->save(); QApplication::quit();} break;
        case Qt::Key_A: game->player->dx = -1; break;
        case Qt::Key_D: game->player->dx = 1; break;
        case Qt::Key_Space: game->player->jump(); break;
        case 43: game->inventory->setActive(0); break;
        case 282: game->inventory->setActive(1); break;
        case 352: game->inventory->setActive(2); break;
        case 268: game->inventory->setActive(3); break;
        case 344: game->inventory->setActive(4); break;
        case 381: game->inventory->setActive(5); break;
        case 221: game->inventory->setActive(6); break;
        case 193: game->inventory->setActive(7); break;
        case 205: game->inventory->setActive(8); break;
    }
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_A: if(game->player->dx == -1) game->player->dx = 0; break;
        case Qt::Key_D: if(game->player->dx == 1) game->player->dx = 0; break;
    }
}
