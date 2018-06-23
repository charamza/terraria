#include "entity.h"
#include "game.h"
#include <QDebug>

Entity::Entity(Game *game, int x, int y, int width, int height)
{
    this->game = game;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->game->scene->addItem(this);
}

QRectF Entity::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

QPainterPath Entity::shape() const
{
    QPainterPath path;
    return path;
}

void Entity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (dead) return;

    rx = x - game->camera->getPosition().rx();
    ry = y - game->camera->getPosition().ry();

    painter->save();
    painter->translate(rx + (dxLast==1?width:0), ry);

    // Vykreslí HPčka
    QRect hpBar = QRect((dxLast==1?-width:0), -15, width*(hp/hpMax), 5);
    QRect hpBarBackground = QRect((dxLast==1?-width:0), -15, width, 5);
    painter->fillRect(hpBarBackground, Qt::white);
    painter->fillRect(hpBar, Qt::red);

    if(dxLast==1){
        QTransform transf = painter->transform();
        transf.scale(-1, 1);
        painter->setTransform(transf);
    }
    if(rotation!=0){
        painter->translate(width/2, height/2);
        painter->rotate(rotation+(dxLast==1?180:0));
        painter->translate(-width/2, -height/2);
    }
    this->draw(painter);
    painter->restore();
}

// Samotný pohyb
void Entity::updateMove(int delta)
{
    if(dx != 0){
        int smer = dx * speed * delta / (float)16;
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
        if(dx!=0) dxLast = dx;
    }
    if(dy != 0){
        int smer = dy * speed * delta / (float)16;
        QRectF moveKolize = QRectF(x, y + smer, width, height);
        if(y+smer >= 0 && y+smer <= game->world->WIDTH*Block::SIZE - width){
            AABB kolize = game->world->collide(moveKolize);
            if(!kolize.collide){
                y += smer;
            }else{
                if(dy > 0){
                    y = kolize.b.y()-height;
                }else if(dy < 0){
                    y = kolize.b.y()+kolize.b.height();
                }
            }
        }
        if(dy!=0) dyLast = dy;
    }
}

// Pád a skok
void Entity::updateGravity(int delta){
    this->fallTime += delta;
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

void Entity::remove()
{
    game->scene->removeItem(this);
    game->world->entities->removeOne(this);
    dead = true;
}

// Skok
void Entity::jump(){
    if(onGround){
        this->fallTime = -jumpLevel*100;
        this->fallDistance = (fallTime/1000) * (fallTime/1000) * 250;
    }
}

void Entity::hit()
{
    if(lastHit + 600 < game->timed){
        lastHit = game->timed;
        hp -= 20;
        if(hp <= 0){
            hp = 0;
            remove();
        }
        onHit();
        qDebug() << "Entita byla zasazena.";
    }
}

