#include "worldgenerator.h"
#include "world.h"
#include "rabbit.h"
#include "bee.h"
#include "shummi.h"
#include "block.h"
#include <QDebug>
#include <time.h>
#include <math.h>

WorldGenerator::WorldGenerator(World *world)
{
    this->world = world;
}

void WorldGenerator::generateWorld()
{
    world->world = new Block**[World::WIDTH];
    for (int i = 0; i < World::WIDTH; ++i) {
        world->world[i] = new Block*[World::HEIGHT];
    }

    srand(time(0));
    const int heightNodes = World::WIDTH/16+1;
    int heightMap[heightNodes];
    for(int i = 0; i < heightNodes; i++){
        int heightOffset = 8;
        heightMap[i] = rand()%32+heightOffset;
    }

    for (int x = 0; x < World::WIDTH; ++x) {
        float i = x%16;
        const int n1 = x/16;
        const int n2 = n1 + 1;

        i = (1-cos(i/16*3.14))*8;
        int interpolate = (heightMap[n1]*(16-i) + heightMap[n2]*i)/16;
        //qDebug() << i << interpolate  << n1 << heightMap[n1] << n2 << heightMap[n2];
        for (int y = 0; y < World::HEIGHT; ++y) {
            world->world[x][y] = NULL;
            int id = rand()%4;

            if(y > interpolate){
                int depth = y - interpolate;
                if(depth == 1)
                    world->world[x][y] = new Block(x, y, 3);
                else if(depth > 8)
                    world->world[x][y] = new Block(x, y, 1);
                else
                    world->world[x][y] = new Block(x, y, 2);
            }
        }
    }

    int lastTree = 0;
    for(int x = 1; x < World::WIDTH - 1; x++){
        if(rand() % 5 == 0 && lastTree != x - 1){
            generateTree(x);
            lastTree = x;
        }else if(rand() % 5 == 0){
            generateFlora(x);
        }
    }

    generateVeins();
}

void WorldGenerator::generateTree(int tx)
{
    int ty = world->height(tx);
    int type = (rand()%3);
    int height = 2;
    int upblock = 53;
    int downblock = 20;
    if(type==0){
        height += rand()%2;
    }
    if(type==1){
        upblock = 133;
        downblock = 117;
    }
    if(type==2){
        upblock = 197;
        downblock = 153;
        height += (type==2?rand()%4+1:0);
    }

    for (int y = 0; y < height; y++){
        Block *block = new Block(tx, ty-y-1, downblock);
        block->solid = false;
        world->setBlock(block);
    }
    for (int x = -1; x <= 1; x++){
        for (int y = -1; y <= 1; y++){
            Block *block = new Block(tx+x, ty-height-y-2, upblock);
            block->solid = false;
            world->setBlock(block);
        }
    }
    qDebug() << "Generoval jsem strom na souřadnicích" << tx << ty;
}

void WorldGenerator::generateFlora(int x)
{
    int type = (rand()%4);
    int id = 0;
    int ty = world->height(x);

    if(type == 0){
        id = 12;
    }
    if(type == 1){
        id = 13;
    }
    if(type == 2){
        id = 12+16;
    }
    if(type == 3){
        id = 13+16;
    }

    Block *block = new Block(x, ty-1, id);
    block->solid = false;
    world->setBlock(block);
}

void WorldGenerator::generateVeins()
{
    int ores[5][2] = {{0, 34}, {50, 33}, {80, 32}, {100, 51}, {100, 50}};
    for(int x = 0; x < World::WIDTH; x++){
        int ty = world->height(x)+8;
        for(int y = World::HEIGHT; y > ty; y--){
            // Generuj rudu
            if(rand()%50==0){
                int id = 0;
                while(true){
                    id = (int)(rand()%5);
                    if(y > ores[id][0]){
                        id = ores[id][1];
                        break;
                    }
                }

                int vlength = rand()%4;
                int vx = x;
                int vy = y;
                for(int vv = 0; vv <= vlength; vv++){
                    int rd = rand()%4;
                    if(rd==0) vx--;
                    if(rd==1) vx++;
                    if(rd==2) vy--;
                    if(rd==3) vy++;
                    if(!world->validBounds(vx, vy)) continue;
                    Block *block = new Block(vx, vy, id);
                    world->setBlock(block);
                }
            }
        }
    }
}

void WorldGenerator::generateFauna()
{
    for(int i = 0; i < World::WIDTH; i++){
        if(rand()%10!=0) continue;
        Rabbit *rabbit = new Rabbit(world->game, i*Block::SIZE, world->height(i)*Block::SIZE);
        world->addEntity(rabbit);
    }
    for(int i = 0; i < World::WIDTH; i++){
        if(rand()%100!=0) continue;
        int height = rand() % world->height(i)*Block::SIZE;
        qDebug() << (world->HEIGHT*Block::SIZE - world->height(i)*Block::SIZE);
        Bee *bee = new Bee(world->game, i*Block::SIZE, height);
        world->addEntity(bee);
    }
    Shummi *shummi = new Shummi(world->game, world->WIDTH*Block::SIZE/2, 0);
    world->addEntity(shummi);
}
