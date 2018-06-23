#ifndef INVENTORY_H
#define INVENTORY_H

#include <QGraphicsItem>

class Game;

class Inventory : public QGraphicsItem
{

public:
    struct InventoryCell{
        int id = -1;
        int count = 0;
        bool item = false;

    };

    Inventory(Game *game);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void update();
    void add(int id, int count);
    void remove(int id, int count);
    void setActive(int index);
    InventoryCell getActive();

    InventoryCell inventory[9];
    int active = 0;

    Game *game;
    QImage hud, spritesheet;
};

#endif // INVENTORY_H
