//
// Created by standa on 10/28/25.
//

#include <SDL_image.h>
#include <SDL_render.h>
#include <stdbool.h>

#include "object.h"

#include "collisions.h"
#include "world.h"

#include "animace.h"

struct Object *Object_CreateObject(const char *name, struct Vector2 size, struct Vector2 position, int zLayer, enum Collisions collions, enum ObjectType objectType,enum ObjectFacing objectDir) {
    struct Object *object = malloc(sizeof(struct Object));
    if (!object) return NULL;

    strcpy(object->name,name);
    object->size = size;
    object->position = position;

    object->zLayer = zLayer;
    object->collision = collions;
    object->objectType = objectType;
    object->objectDir = objectDir;
    object->spawnTime = SDL_GetTicks();;

    return object;
}


bool Object_MoveBy(struct Object *object, struct Vector2 addVector) {
    struct Vector2 newPos = {object->position.x + addVector.x, object->position.y + addVector.y};

    object->position = newPos;
}

bool Object_CheckCollision(struct World *world, struct Object *object) {
    for (int i = 0; i < world->objectCount; ++i) {
        printf("is player Colliding: %d", Collsions_areColliding(object, &world->objects[i]));

    }
}

void Object_Tick(struct Object *object) {
    char name[32];
    strcpy(name,object->name);
    if (strcmp(strtok(name,"_"), "bullet") == 0) {
        int bulletSpeed = 10;
        struct Vector2 addVector = {
            (object->objectDir == WEST)?-bulletSpeed:(object->objectDir == EAST)?bulletSpeed:0,
            (object->objectDir == NORTH)?-bulletSpeed:(object->objectDir == SOUTH)?bulletSpeed:0
        };
        Object_MoveBy(object,addVector);
    }
}

void Object_OnOverLapWithObject(struct World *world,struct Object *object1, struct Object *object2) {

}

bool Object_SetActiveAnimationByName(struct Object * object, char *animName, enum AnimationMirrorFlip mirroredFlipped) {
    int newAnimationIndex = -1;

    for (int i = 0; i < object->animationsCount; ++i) {
        if ((strcmp(object->animations[i].name,animName) == 0) ) {
            if (object->activeAnimationIndex == i && object->animations[i].mirroredFlipped == mirroredFlipped) { // jestlize tato animace uz je aktivni, tak konec
                return true;
            }
            object->animations[i].currentFrame = 0; // nastavime aktivni smiek na 1., aby animace zacla od zacatku
            object->animations[i].mirroredFlipped = mirroredFlipped;
            newAnimationIndex = i;
            break;
        }
    }
    if (newAnimationIndex == -1) {
        printf("Nepodarilo se nacist animaci '%s' pro objekt '%s'",animName,object->name);
        return false;
    }

    object->activeAnimationIndex = newAnimationIndex;
    return true;

}

double Object_GetAngleFromDir(enum ObjectFacing dir) {
    switch (dir) {
        case NORTH: {
            return -90;
        }break;
        case EAST: {
            return 0;
        }break;
        case SOUTH: {
            return 90;
        }break;
        case WEST: {
            return 180;
        }break;
    }
    return 0;
}


void Object_Destroy(struct Object *object) {
    Animation_RemoveAnimations(object);

}

void Object_Print(const struct Object *object) {
    printf("Object:\n\tName: %s\n", object->name);
    // for (int i = 0; i < object->animationsCount; ++i) {
    //     Animation_PrintAnimation(&object->animations[i]);
    // }
}
