#ifndef SHUMMI_H
#define SHUMMI_H

#include "entity.h"

class Shummi : public Entity
{
public:
    Shummi(Game *game, int x, int y);
    void update(int delta);
    void draw(QPainter *gl);
    void onHit();

    int steps = 0;
    int moves = 0, movesNext = 0;
};

#endif // SHUMMI_H
