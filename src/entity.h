#ifndef ENTITY_H
#define ENTITY_H

#include <QPainter>
#include <QGraphicsItem>

class Game;

class Entity : public QGraphicsItem
{
public:
    Entity(Game *game, int x, int y, int width, int height);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    float x, y, rx, ry;
    int width, height;
    int dx = 0, dy = 0, dxLast = 0, dyLast = 0;
    double rotation = 0;
    float speed = 1;
    float hp = 100, hpMax = 100;
    bool dead = false;
    bool onGround = false;
    float fallTime = 0, fallDistance = 0, jumpLevel = 6;
    Game *game;

    virtual void update(int delta) = 0;
    virtual void draw(QPainter *painter) = 0;
    virtual void onHit() = 0;

    void updateMove(int delta);
    void updateGravity(int delta);
    void remove();
    void jump();
    void hit();

private:
    long lastHit = 0;
};

#endif // ENTITY_H
