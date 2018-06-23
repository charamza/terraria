#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

class World;

class WorldGenerator
{
public:
    WorldGenerator(World *world);

    World *world;

    void generateWorld();
    void generateTree(int x);
    void generateFlora(int x);
    void generateVeins();
    void generateFauna();
};

#endif // WORLDGENERATOR_H
