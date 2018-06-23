#include "player.h"
#include "game.h"
#include <stdlib.h>
#include <QDebug>
#include <QPainter>
#include "block.h"
#include <math.h>
#include "aabb.h"

Player::Player(Game *game, int x, int y)
{
    this->game = game;
    this->x = x;
    this->y = y;
    skin.load(":/res/skin_naruto.png");
    spritesheet.load(":/res/spritesheet.png");

    game->scene->addItem(this);
}

QRectF Player::boundingRect() const
{
    return QRectF(0, 0, width, height);

}

QPainterPath Player::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    float x1 = (game->WIDTH - width) / 2;
    float y1 = (game->HEIGHT - height) / 2;

    painter->setOpacity(paintDeadScreen(painter));

    float angle = -atan2(game->camera->mouse.rx() - game->WIDTH / 2, game->camera->mouse.ry() - game->HEIGHT / 2 + 10) * 180 / 3.14;

    // Začátek
    painter->save();
    painter->translate(QPointF(game->WIDTH / 2, game->HEIGHT / 2));

    // Pravá noha
    painter->save();
    painter->translate(0, 8);
    painter->rotate(-legRotation);

    QRectF legRight = QRectF(-4, 0, 8, 24);
    QRectF tleg = QRectF(0, 20, 4, 12);
    painter->drawImage(legRight, skin, tleg);

    painter->restore();

    // Tělo
    QRectF body = QRectF(-4, -16, 8, 24);
    QRectF tbody = QRectF(16, 20, 4, 12);
    painter->drawImage(body, skin, tbody);

    // Levá noha
    painter->save();
    painter->translate(0, 8);
    painter->rotate(legRotation);

    QRectF legLeft = QRectF(-4, 0, 8, 24);
    painter->drawImage(legLeft, skin, tleg);

    painter->restore();

    // Hlava
    painter->save();
    painter->translate(0, -16);
    painter->rotate(angle - 90 + (angle < 0 ? 180 : 0));

    QRectF head = QRectF(-8, -16, 16, 16);
    QRectF thead;
    if(angle < 0) thead = QRectF(0, 8, 8, 8);
    else thead = QRectF(16, 8, 8, 8);
    painter->drawImage(head, skin, thead);

    painter->restore();

    // Ruce
    painter->save();
    painter->translate(0, -8);
    painter->rotate(angle + (angle < 0 ? -handRotation : handRotation));

    if(game->inventory->getActive().id != -1){
        painter->save();
        QRectF armBlock = QRectF(-8, -8, 16, 16);
        QRectF armBlockTexture = Block::getTextureById(game->inventory->getActive().id);

        if(angle < 0){
            painter->translate(6, 18);
            painter->rotate(90);
        }else{
            painter->translate(-6, 18);
            painter->rotate(-90);
        }

        painter->drawImage(armBlock, spritesheet, armBlockTexture);
        painter->restore();
    }

    QRectF arm = QRectF(-4, -4, 8, 24);
    QRectF tarm = QRectF(40, 20, 4, 12);
    painter->drawImage(arm, skin, tarm);

    painter->restore();

    // Konec
    painter->restore();
    painter->setOpacity(1);
}

float Player::paintDeadScreen(QPainter *painter)
{
    int state = wasted;
    if(wasted > 100) state = 100;
    if(wasted > 0){
        QRect wastedRect = QRect(game->WIDTH/2-100, game->HEIGHT/2-30, 200, 60);
        painter->setOpacity(state/(float)100);
        painter->drawImage(wastedRect, game->textureWasted);
        painter->setOpacity(1);

        if(wasted++ > 300){
            x = World::WIDTH * Block::SIZE / 2;
            y = 0;
            wasted = 0;
        }
    }
    return (100 - state)/(float)100;
}

void Player::update(int delta)
{
    handMove = false;
    if(wasted > 0) return;

    updateMove();

    if(game->camera->clickLeft && !game->camera->pointingOnEntity){
        int brx = game->camera->mouseBlockX * Block::SIZE + Block::SIZE/2,
            bry = game->camera->mouseBlockY * Block::SIZE + Block::SIZE/2;
        if(canReach(brx, bry)){
            int breakState = game->world->breakBlock(game->camera->mouseBlockX, game->camera->mouseBlockY);
            if(breakState == -1) handMove = true;
            if(breakState >= 0) game->inventory->add(breakState, 1);
        }
    }

    if(game->camera->clickRight){
        int brx = game->camera->mouseBlockX * Block::SIZE + Block::SIZE/2,
            bry = game->camera->mouseBlockY * Block::SIZE + Block::SIZE/2;
        if(canReach(brx, bry)){
            if(game->inventory->getActive().id != -1){
                int placeState = game->world->placeBlock(game->camera->mouseBlockX, game->camera->mouseBlockY);
                if(placeState >= 0) game->inventory->remove(placeState, 1);
            }
        }
    }


    if(handMove || handshakeProgress){
        shakes++;
        handRotation += handDirection*5;
        if(handRotation > 30) handDirection = -1;
        if(handRotation < -30) handDirection = 1;
        if(handshakeProgress > 0){
            handshakeProgress++;
            if(handshakeProgress > 30) handshakeProgress = 0;
        }
    }else if(!handMove){
        shakes = 0;
        handRotation = 0;
        handDirection = 1;
    }

}

void Player::updateMove()
{
    // Animace pohybu
    if(dx == 0){
        steps = 0;
        legRotation = 0;
        legDirection = 1;
    }else{
        steps++;
        legRotation += legDirection*2;
        if(legRotation > 45) legDirection = -1;
        if(legRotation < -45) legDirection = 1;
    }

    // Samotný pohyb
    int smer = dx * 5;
    QRectF moveKolize = QRectF(x + smer, y, width, height);
    if(x+smer >= 0 && x+smer <= game->world->WIDTH*Block::SIZE - width){
        AABB kolize = game->world->collide(moveKolize);
        if(!kolize.collide){
            x += smer;
        }else{
            if(dx > 0){
                x = kolize.b.x()-width;
            }else if(dx < 0){
                x = kolize.b.x()+kolize.b.width();
            }
        }
    }

    // Pád a skok
    this->fallTime += 16;
    float fall = (fallTime/1000) * (fallTime/1000) * 250 - this->fallDistance;
    this->fallDistance += fall;

    QRectF fallKolize;
    if(fall > 0)
        fallKolize = QRectF(x, y, width, height + fall);
    else
        fallKolize = QRectF(x, y + fall, width, height);

    AABB kolize = game->world->collide(fallKolize);
    if(!kolize.collide){
        y += fall;
        onGround = false;
    }else{
        fallTime = 0;
        fallDistance = 0;
        if(fall > 0){
            onGround = true;
            y = kolize.b.y() - height;
        }else{
            y = kolize.b.y() + kolize.b.height();
        }
    }
}

void Player::jump(){
    if(onGround){
        this->fallTime = -900;
        this->fallDistance = (fallTime/1000) * (fallTime/1000) * 250;
    }
}

void Player::die()
{
    if(wasted <= 0) wasted = 1;
}

void Player::handshake()
{
    if(handshakeProgress <= 0){
        handshakeProgress = 1;
        shakes = 0;
        handRotation = 0;
        handDirection = 1;
    }
}

bool Player::canReach(int rx, int ry)
{
    int cx = game->camera->pos.rx() + game->WIDTH/2,
        cy = game->camera->pos.ry() + game->HEIGHT/2;
    int distance = sqrt(pow(abs(rx-cx), 2)+pow(abs(ry-cy), 2));
    if(distance < 300) return true;
    return false;
}
