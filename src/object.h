//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_OBJECT_H
#define PROJEKT_OBJECT_H
#include "basicStructs.h"
#include "animace.h"

enum Collisions {
    COLLISION_NONE,
    COLLISION_OVERLAP,
    COLLISION_BLOCK
};

struct Object {
    char name[32];
    struct Vector2 size;
    struct Vector2 position;
    struct Animation *animations;
    enum ObjectAnimationsType objectAnimationsType;
    int activeAnimationIndex;
    int animationSetIndex;
    int animationsCount;
    int zLayer;
    enum Collisions collision;
};

struct Object *Object_CreateObject(const char *name, struct Vector2 size, struct Vector2 position, int zLayer, enum Collisions collions);

bool Object_SetTextures(SDL_Renderer *ren, struct Object *object);
bool Object_SetActiveAnimationByName(struct Object * object, char *animName, bool mirrored);

bool Object_MoveBy(struct Object *object, struct Vector2 addVector);


void Object_Destroy(struct Object * object);

void Object_Print(const struct Object *object);

#endif //PROJEKT_OBJECT_H