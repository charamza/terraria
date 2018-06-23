#include "aabb.h"

AABB::AABB(QRectF a)
{
    this->a = a;
    collide = false;
}


AABB::AABB(QRectF a, QRectF b)
{
    this->a = a;
    this->b = b;
    if(a.x() + a.width() > b.x() && a.x() < b.x() + b.width() &&
       a.y() + a.height() > b.y() && a.y() < b.y() + b.height()){
        collide = true;
    }else{
        collide = false;
    }
}
