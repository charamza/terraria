#ifndef RABBIT_H
#define RABBIT_H

#include "entity.h"

class Rabbit : public Entity
{
public:
    Rabbit(Game *game, int x, int y);

    void update(int delta);
    void draw(QPainter *gl);
    void onHit();

    int steps = 0;
    int moves = 0, movesNext = 0;
};

#endif // RABBIT_H
