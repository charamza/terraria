#ifndef AABB_H
#define AABB_H

#include <QRectF>

class AABB
{
public:
    AABB(QRectF a);
    AABB(QRectF a, QRectF b);
    QRectF a;
    QRectF b;

    bool collide = false;
};

#endif // AABB_H
