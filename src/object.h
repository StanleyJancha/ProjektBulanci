//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_OBJECT_H
#define PROJEKT_OBJECT_H
#include "basicStructs.h"
#include "animace.h"
#include "world.h"

enum Collisions {
    COLLISION_NONE,
    COLLISION_OVERLAP,
    COLLISION_BLOCK
};

enum ObjectType {
    OBJECT_STATIC,
    OBJECT_DYNAMIC,
    OBJECT_PICKUP_WEAPON,
    OBJECT_PLAYER
};

enum ObjectFacing {
    NORTH,
    EAST,
    SOUTH,
    WEST
};


struct Object {
    char name[64];
    struct Vector2 size;
    struct Vector2 position;
    enum ObjectFacing objectDir;
    struct Animation *animations;
    enum ObjectAnimationsType objectAnimationsType;
    int activeAnimationIndex;
    int animationSetIndex;
    int animationsCount;
    int zLayer;
    enum Collisions collision;
    enum ObjectType objectType;
    Uint32 spawnTime;
};

struct Object *Object_CreateObject(const char *name, struct Vector2 size, struct Vector2 position, int zLayer, enum Collisions collions, enum ObjectType objectType,enum ObjectFacing objectDir) ;
bool Object_SetTextures(SDL_Renderer *ren, struct Object *object);
bool Object_SetActiveAnimationByName(struct Object * object, char *animName, enum AnimationMirrorFlip mirroredFlipped);
bool Object_MoveBy(struct Object *object, struct Vector2 addVector);
void Object_OnOverLapWithObject(struct World *world,struct Object *object1, struct Object *object2);
void Object_Tick(struct Object *object);
double Object_GetAngleFromDir(enum ObjectFacing dir);
void Object_SetRandomPosition(struct World *world, struct Object *object, int boundsMinX, int boundsMaxX, int boundsMinY, int boundsMaxY);

void Object_Destroy(struct Object * object);

void Object_Print(const struct Object *object);

#endif //PROJEKT_OBJECT_H