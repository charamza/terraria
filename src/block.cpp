#include "block.h"

Block::Block(const int x, const int y, const int id)
{
    this->x = x;
    this->y = y;
    this->id = id;

    this->texture = Block::getTextureById(id);

    if(id==12||id==13||id==28||id==29) solid = false;
}



void Block::update()
{
    if(durability != lastDurability){
        int index = 10-durability/5+240;
        this->breakTexture = Block::getTextureById(index);
    }
}

int Block::getRX()
{
    return this->x * SIZE;
}
int Block::getRY()
{
    return this->y * SIZE;
}

QRectF Block::getTextureById(int id)
{
    int tileSize = 16, tilesOnLine = 16;
    float tx = id%tilesOnLine, ty = id/tilesOnLine;
    QRectF texture = QRectF(tx * tileSize, ty * tileSize, tileSize, tileSize);
    return texture;
}
