//
// Created by standa on 10/28/25.
//

#include "world.h"

#include "player.h"

struct World World_Create() {
    struct World world;
    world.objects = NULL;
    world.objectCount = 0;
    world.players = NULL;
    world.playerCount = 0;
    return world;
}

bool World_AddObject(struct World *world, struct Object *object) {
    struct Object *newArray = malloc(sizeof(struct Object) * (world->objectCount + 1));

    if (newArray == 0) {
        return false;
    }
    for (int i = 0; i < world->objectCount; i++) {
        newArray[i] = world->objects[i];
    }
    newArray[world->objectCount] = *object;

    world->objects = newArray;
    world->objectCount += 1;
    return true;
}

bool World_AddPlayer(struct World *world, struct Player *player) {
    struct Player *newArray = malloc(sizeof(struct Player) * (world->playerCount + 1));

    if (newArray == 0) {
        return false;
    }
    for (int i = 0; i < world->playerCount; i++) {
        newArray[i] = world->players[i];
    }
    newArray[world->playerCount] = *player;

    world->players = newArray;
    world->playerCount += 1;
    return true;
}

void World_Print(const struct World *world) {
    printf("World:\n\tObjects:\n\t\t");
    for (int i = 0; i < world->objectCount; i++) {
        printf("%d. ",i);
        struct Object object = world->objects[i];
        Object_Print(&object);
    }
    printf("World:\n\tObjects:\n\t\t");
    for (int i = 0; i < world->objectCount; i++) {
        printf("%d. ",i);
        struct Player player = world->players[i];
        Player_Print(&player);
    }
}

struct Player* World_GetPlayerByIndex(const struct World *world, int index) {
    if (world->playerCount <= index) {
        return NULL;
    }
    return &world->players[index];
}
