#include "camera.h"
#include "game.h"

Camera::Camera(Game *game)
{
    this->game = game;
}

void Camera::update(){
    float x = game->player->x - (game->WIDTH - game->player->width) / 2;
    float y = game->player->y - (game->HEIGHT - game->player->height) / 2;
    setPosition(x, y);
    mouseBlockX = (mouse.x() + getPosition().rx()) / Block::SIZE;
    mouseBlockY = (mouse.y() + getPosition().ry()) / Block::SIZE;
}

QPointF Camera::getPosition()
{
    return this->pos;
}

void Camera::setPosition(float x, float y)
{
    this->pos = QPointF(x, y);
}

void Camera::setPosition(QPointF position)
{
    this->pos = position;
}

void Camera::move(float x, float y)
{
    this->pos += QPointF(x, y);
}

void Camera::move(QPointF move)
{
    this->pos += move;
}

