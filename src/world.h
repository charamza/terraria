#ifndef WORLD_H
#define WORLD_H

#include <QGraphicsItem>
#include <QImage>
#include "block.h"
#include "aabb.h"
#include "entity.h"
#include "worldgenerator.h"

class Game;

class World : public QGraphicsItem
{
public:
    World(Game *game);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintBackground(QPainter *painter);
    void update(int delta);

    QString name = "starter";

    const static int WIDTH = 2048, HEIGHT = 128;
    QImage spritesheet, background;
    Block*** world;
    QList<Entity*> *entities;
    Game *game;
    WorldGenerator *gen;

    void generate();
    void load();
    void save();

    int height(int x);
    Block* getBlock(int x, int y);
    void setBlock(Block *block);
    void addEntity(Entity *entity);
    AABB collide(QRectF rect);
    bool validBounds(int x, int y);
    bool blockNextTo(int x, int y);
    int breakBlock(int x, int y);
    int placeBlock(int x, int y);

private:
};

#endif // WORLD_H
