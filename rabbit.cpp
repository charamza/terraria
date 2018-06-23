#include "rabbit.h"
#include "game.h"
#include <QDebug>

Rabbit::Rabbit(Game *game, int x, int y) : Entity(game, x, y, 30, 28)
{
    speed = 1.5;
}

void Rabbit::update(int delta)
{
    updateMove(delta);
    updateGravity(delta);
    steps++;
    moves++;
    if(moves>movesNext){
        moves = 0;
        movesNext = rand()%60+60;
        dx = rand()%3 - 1;
    }
    if(onGround && dx != 0){
        jump();
    }
}

void Rabbit::draw(QPainter *gl)
{
    QRect tex(830, 600+((steps/10)%7)*40*(dx!=0?1:0), 30, 28);

    gl->drawImage(QRect(0, 0, width, height), game->textureNPC, tex);
}

void Rabbit::onHit()
{

}
