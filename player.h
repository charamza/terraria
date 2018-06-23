#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>

class Game;

class Player : public QGraphicsItem
{
public:
    Player(Game *game, int x, int y);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    float paintDeadScreen(QPainter *painter);
    void update(int delta);
    void updateMove();
    void jump();
    void die();
    void handshake();
    bool canReach(int rx, int ry);

    int width = 8, height = 64;
    int dx = 0, dy = 0;
    float x = 0, y = 0;
    bool onGround = false;
    int wasted = 0;

    Game *game;
    QImage skin;
    QImage spritesheet;

private:
    // Pád
    float fallTime = 0, fallDistance = 0;

    int steps = 0, shakes = 0;

    // Animace
    float legRotation = 0, legDirection = 1;
    float handRotation = 0, handDirection = 1, handshakeProgress = 0;
    bool handMove = false;
};

#endif // PLAYER_H
