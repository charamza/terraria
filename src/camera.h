#ifndef CAMERA_H
#define CAMERA_H

#include <QPointF>

class Game;

class Camera
{
public:
    Camera(Game *game);

    Game *game;
    QPointF pos;
    QPointF mouse;
    bool clickLeft = false, clickRight = false, pointingOnEntity = false;
    int mouseBlockX = 0, mouseBlockY = 0;

    void update();
    QPointF getPosition();
    void setPosition(float x, float y);
    void setPosition(QPointF position);
    void move(float x, float y);
    void move(QPointF move);
};

#endif // CAMERA_H
