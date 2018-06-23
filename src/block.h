#ifndef BLOCK_H_
#define BLOCK_H_

#include <QRectF>

class Block
{
public:
    Block(int x, int y, int id);
    void update();

    // Skutečné souřadnice
    int getRX();
    int getRY();
    static QRectF getTextureById(int id);

    QRectF texture;
    QRectF breakTexture;

    const static int SIZE = 32;
    int id, x, y;
    bool solid = true;
    float durability = 49, lastDurability = 49;
};

#endif // BLOCK_H
