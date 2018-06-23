#include "inventory.h"
#include "game.h"

#include <QPainter>
#include <QDebug>

Inventory::Inventory(Game *game)
{
    this->game = game;
    hud.load(":/res/hud.png");
    spritesheet.load(":/res/spritesheet.png");

    for(int i = 0; i < 9; i++){
        InventoryCell item;
        inventory[i] = item;
    }

    game->scene->addItem(this);
}

QRectF Inventory::boundingRect() const
{
    return QRectF(0, 0, 0, 0);
}

QPainterPath Inventory::shape() const
{
    QPainterPath path;
    return path;
}

void Inventory::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    for(int i = 0; i < 9; i++){
        if(active != i){
            painter->drawImage(QRectF(25 + 55*i, 25, 48, 48), hud);
        }else{
            painter->drawImage(QRectF(20 + 55*i, 20, 58, 58), hud);
            /*painter->drawRect(QRectF(25 + 50*i, 25, 45, 45));
            painter->drawRect(QRectF(26 + 50*i, 26, 43, 43));*/
        }
        if(inventory[i].id!=-1){
            painter->drawImage(QRectF(35 + 55*i, 36, 28, 28), spritesheet, Block::getTextureById(inventory[i].id));

            painter->setPen(QPen(QColor(255,255,255,255)));
            QFont font("Comic Sans MS", 10, QFont::Bold);
            painter->setFont(font);
            painter->drawText(QPointF(37 + 55*i, 60), QString::number(inventory[i].count));
        }
    }
}

void Inventory::update()
{

}

void Inventory::add(int id, int count)
{
    int emptyIndex = -1;
    for(int i = 0; i < 9; i++){
        if(inventory[i].id == id){ inventory[i].count += count; return; }
        if(inventory[i].id == -1 && emptyIndex == -1) emptyIndex = i;
    }
    // Přidej nový
    if(emptyIndex != -1){
        inventory[emptyIndex].id = id;
        inventory[emptyIndex].count = count;
    }
}

void Inventory::remove(int id, int count)
{
    for(int i = 0; i < 9; i++){
        if(inventory[i].id == id){
            inventory[i].count -= count;
            if(inventory[i].count <= 0){
                inventory[i].id = -1;
            }
            return;
        }
    }
}

void Inventory::setActive(int index)
{
    this->active = index;
}

Inventory::InventoryCell Inventory::getActive()
{
    return inventory[active];
}
