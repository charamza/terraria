#include "world.h"
#include "game.h"
#include <math.h>
#include <QDebug>
#include <time.h>
#include <QPainter>
#include <QFile>
#include "rabbit.h"

World::World(Game *game)
{
    this->game = game;
    spritesheet.load(":/res/spritesheet.png");
    background.load(":/res/background.png");

    entities = new QList<Entity*>();

    gen = new WorldGenerator(this);

    game->scene->addItem(this);
}



QRectF World::boundingRect() const
{
    return QRectF(0, 0, Block::SIZE*WIDTH, Block::SIZE*HEIGHT);
}

QPainterPath World::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void World::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int bx = game->camera->getPosition().rx()/Block::SIZE;
    int by = game->camera->getPosition().ry()/Block::SIZE;
    if(bx < 0) bx = 0;
    if(by < 0) by = 0;
    int fx = bx+game->WIDTH/Block::SIZE+2;
    int fy = by+game->HEIGHT/Block::SIZE+2;
    if(fx>WIDTH) fx = WIDTH;
    if(fy>HEIGHT) fy = HEIGHT;

    paintBackground(painter);

    for(int x = bx; x < fx; x++){
        for(int y = by; y < fy; y++){
            Block *block = world[x][y];
            if(block != NULL){
                block->update();

                qreal size = Block::SIZE;
                QRectF bounds = QRectF(block->getRX() - game->camera->getPosition().rx(),
                                 block->getRY() - game->camera->getPosition().ry(),
                                 size, size);
                painter->drawImage(bounds, spritesheet, block->texture);
                if(!block->breakTexture.isNull())
                    painter->drawImage(bounds, spritesheet, block->breakTexture);
            }
        }
    }
}

void World::paintBackground(QPainter *painter)
{

    int xOff = game->camera->pos.rx()/3;
    QRect bounds = QRect(-xOff%3840, 0, 3840, 1080);
    painter->drawImage(bounds, background);

    QRect bounds2 = QRect(-xOff%3840+3840, 0, 3840, 1080);
    painter->drawImage(bounds2, background);
}

void World::update(int delta)
{
    game->camera->pointingOnEntity = false;
    for(int i = 0; i < entities->length(); i++) {
        Entity *entity = entities->at(i);

        int DISTANCE_RESERVE = 1000;
        if(entity->x+entity->width > game->camera->pos.x() - DISTANCE_RESERVE && entity->x < game->camera->pos.x()+game->WIDTH + DISTANCE_RESERVE &&
           entity->y+entity->height > game->camera->pos.y() - DISTANCE_RESERVE && entity->y < game->camera->pos.y()+game->HEIGHT + DISTANCE_RESERVE){
            entity->update(delta);

            // Zkontroluje, jestli hrác náhodou nechce někoho trefit
            if(game->camera->clickLeft){
                QPointF mouse = game->camera->mouse;
                int rx = mouse.rx() + game->camera->pos.rx(),
                    ry = mouse.ry() + game->camera->pos.ry();
                if(game->player->canReach(rx, ry)){
                    if(entity->rx+entity->width > mouse.rx() && entity->rx < mouse.rx() && entity->ry+entity->height > mouse.ry() && entity->ry < mouse.ry()){
                        entity->hit();
                        game->player->handshake();
                        game->camera->pointingOnEntity = true;
                    }
                }
            }
        }
    }
}

void World::generate()
{
    gen->generateWorld();
    gen->generateVeins();
}

void World::load()
{
    world = new Block**[WIDTH];
    for (int i = 0; i < WIDTH; ++i) {
        world[i] = new Block*[HEIGHT];
    }
    for(int x = 0; x < WIDTH; x++){
        for(int y = 0; y < HEIGHT; y++){
            world[x][y] = NULL;
        }
    }

    QFile file(this->name + ".wrld");
    if (file.exists() && file.open(QIODevice::ReadWrite)){
        QTextStream in(&file);

        int y = 0;
        while(y<HEIGHT) {
            qDebug() << "line" << y;
            QStringList lineData = in.readLine().split(",");
            for(int x = 0; x < WIDTH; x++){
                QStringList bits = lineData[x].split(":");
                int id = bits[0].toInt();
                bool solid = (bits[1].toInt()==0?false:true);
                Block *block = new Block(x, y, id);
                block->solid = solid;
                if(id!=0) world[x][y] = block;
            }
            y++;
        }
    }else{
        generate();
    }
}

void World::save()
{
    QFile file("../Terraria/" + this->name + ".wrld");
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        for(int y = 0; y < HEIGHT; y++){
            for(int x = 0; x < WIDTH; x++){
                int id = 0;
                int solid = 1;
                if(world[x][y] != NULL){
                    id = world[x][y]->id;
                    if(!world[x][y]->solid) solid = 0;
                }
                stream << QString::number(id) << ":" << solid << ",";
            }
            stream << endl;
        }
    }
}

int World::height(int x)
{
    for(int y = 0; y < HEIGHT; y++){
        if(world[x][y] != NULL && world[x][y]->solid) return y;
    }
    return 0;
}

Block* World::getBlock(int x, int y)
{
    if(!validBounds(x, y)) return NULL;
    return world[x][y];
}

void World::setBlock(Block *block)
{
    //delete world[block->x][block->y];
    world[block->x][block->y] = block;
}

void World::addEntity(Entity *entity)
{
    entities->append(entity);
}

AABB World::collide(QRectF rect)
{
    int bx = rect.x()/Block::SIZE;
    int by = rect.y()/Block::SIZE;
    if(bx < 0) bx = 0;
    if(by < 0) by = 0;
    int fx = (bx+rect.width()/Block::SIZE)+game->WIDTH/Block::SIZE+1;
    int fy = (by+rect.height()/Block::SIZE)+game->HEIGHT/Block::SIZE+1;
    if(fx>WIDTH) fx = WIDTH;
    if(fy>HEIGHT) fy = HEIGHT;

    for(int x = bx; x < fx; x++){
        for(int y = by; y < fy; y++){
            Block *block = world[x][y];
            if(block != NULL && block->solid){
                QRectF blockBox = QRectF(block->getRX(), block->getRY(), Block::SIZE, Block::SIZE);
                AABB collision = AABB(rect, blockBox);
                if(collision.collide)
                    return collision;
            }
        }
    }
    return AABB(rect);
}

bool World::validBounds(int x, int y)
{
    if(x < 0) return false;
    if(y < 0) return false;
    if(x >= WIDTH) return false;
    if(y >= HEIGHT) return false;
    return true;
}

bool World::blockNextTo(int x, int y)
{
    for(int bx = -1; bx <= 1; bx++){
        for(int by = -1; by <= 1; by++){
            if(bx == 0 && by == 0) continue;
            if(getBlock(x+bx, y+by) != NULL) return true;
        }
    }
    return false;
}

int World::breakBlock(int x, int y)
{
    if(x >= 0 && y >= 0){
        if(world[x][y] != NULL){
            Block *block = world[x][y];
            if(--block->durability<=0) world[x][y] = NULL;
            if(block->durability <= 0)
                return block->id;
            else
                return -1;
        }
    }
    return -2;
}

int World::placeBlock(int x, int y)
{
    if(x >= 0 && y >= 0){
        if(world[x][y] == NULL){
            if(!blockNextTo(x, y)) return -1;
            int id = game->inventory->getActive().id;
            Block *block = new Block(x, y, id);
            world[x][y] = block;
            return id;
        }else
            return -1;
    }
    return -2;
}
