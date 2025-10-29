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

void World_Destroy(struct World * world) {
    for (int i = 0; i < world->objectCount; ++i) {
        Object_Destroy(&world->objects[i]);
    }
    free(world->objects);
    world->objects = NULL;
    world->objectCount = 0;

    for (int i = 0; i < world->playerCount; ++i) {
        Object_Destroy(&world->players[i].object);
    }
    free(world->players);
    world->players = NULL;
    world->playerCount = 0;
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

    free(world->objects);

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

    free(world->players);

    world->players = newArray;
    world->playerCount += 1;
    return true;
}

bool World_RemoveObject(struct World *world, struct Object *object) {
    if (world->objectCount <= 0){return -3;} // jesli je pocet keys 0, tak neni co mazat

    int indexToRemove = -1;

    for (int i = 0; i < world->objectCount; i++) {
        if (world->objects[i].name == object->name) {
            indexToRemove = i;
            break;
        }
    }


    if (indexToRemove == -1) { // pokud tam neni prvek, ktery chceme vymazat
        return false;
    }

    if (world->objectCount - 1 == 0) { // nastane, pokud se nasel a zaroven je nova velikost nulova
        free(world->objects);
        world->objects = NULL;
        world->objectCount = 0;
        return true;
    }

    struct Object *newObjectArray = malloc(sizeof(struct Object) * (world->objectCount - 1) ); // vytvorime nove pole s velikosti o jednu mensi

    for (int i = 0; i < world->objectCount; i++) {
        newObjectArray[i] = world->objects[i - ((i >= indexToRemove)?1:0)];
    }

    free(world->objects);

    world->objects = newObjectArray;
    world->objectCount -= 1;

    return true;
}
bool World_RemovePlayer(struct World *world, struct Player *player) {
    if (world->playerCount <= 0){return -3;} // jesli je pocet keys 0, tak neni co mazat

    int indexToRemove = -1;

    for (int i = 0; i < world->playerCount; i++) {
        if (world->players[i].object.name == player->object.name) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) { // pokud tam neni prvek, ktery chceme vymazat
        return false;
    }

    if (world->playerCount - 1 == 0) { // nastane, pokud se nasel a zaroven je nova velikost nulova
        free(world->players);
        world->players = NULL;
        world->playerCount = 0;
        return true;
    }

    struct Player *newPlayerArray = malloc(sizeof(struct Player) * (world->playerCount - 1) ); // vytvorime nove pole s velikosti o jednu mensi

    for (int i = 0; i < world->playerCount; i++) {
        newPlayerArray[i] = world->players[i];
    }

    free(world->players);

    world->players = newPlayerArray;
    world->playerCount -= 1;

    return true;
}

void World_Print(const struct World *world) {
    printf("World:\n\tObjects:\n");
    for (int i = 0; i < world->objectCount; i++) {
        printf("%d. ",i);
        struct Object object = world->objects[i];
        Object_Print(&object);
    }
    printf("World:\n\tPlayers:\n");
    for (int i = 0; i < world->playerCount; i++) {
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
