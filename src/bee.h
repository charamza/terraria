#ifndef BEE_H
#define BEE_H

#include "entity.h"

class Bee : public Entity
{
public:
    Bee(Game *game, int x, int y);

    void update(int delta);
    void draw(QPainter *gl);
    void onHit();

    int steps = 0;
    int moves = 0, movesNext = 0;
};

#endif // BEE_H
