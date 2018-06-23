#include "bee.h"
#include "game.h"
#include <QPainter>

Bee::Bee(Game *game, int x, int y) : Entity(game, x, y, 47, 38)
{
    this->speed = 1;
}

void Bee::update(int delta)
{
    updateMove(delta);
    steps++;
    moves++;
    if(moves>movesNext){
        moves = 0;
        movesNext = rand()%60+60;
        dx = rand()%3 - 1;
        dy = rand()%3 - 1;
    }
}

void Bee::draw(QPainter *gl)
{
    QRect tex(344+((steps/10)%3)*47, 907, 47, 38);

    gl->drawImage(boundingRect(), game->textureNPC, tex);
}

void Bee::onHit()
{

}
