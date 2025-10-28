//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_WORLD_H
#define PROJEKT_WORLD_H

#include "object.h"

struct World {
    struct Object *objects;
    int objectCount;
    struct Player *players;
    int playerCount;
};

struct World World_Create();
bool World_AddObject(struct World *world, struct Object *object);
bool World_AddPlayer(struct World *world, struct Player *player);
void World_Print(const struct World *world);
struct Player* World_GetPlayerByIndex(const struct World *world, int index);

#endif //PROJEKT_WORLD_H