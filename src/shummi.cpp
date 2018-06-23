#include "shummi.h"
#include "game.h"
#include <QDebug>
#include "view.h"

Shummi::Shummi(Game *game, int x, int y) : Entity(game, x, y, 34, 50)
{
    speed = 1.5;
}

void Shummi::update(int delta)
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
    if(dx != 0)
        rotation-= 5;
    else
        rotation = 0;
}

void Shummi::draw(QPainter *gl)
{
    QRect tex(496, 899, 34, 50);

    gl->drawImage(QRect(0, 0, width, height), game->textureNPC, tex);
}

void Shummi::onHit(){
    this->game->player->die();
    hp = hpMax;
}
