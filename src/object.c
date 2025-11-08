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

struct Object *Object_CreateObject(const char *name, struct Vector2 size, struct Vector2 position, int zLayer, enum Collisions collions, enum ObjectType objectType) {
    struct Object *object = malloc(sizeof(struct Object));
    if (!object) return NULL;

    strcpy(object->name,name);
    object->size = size;
    object->position = position;

    object->zLayer = zLayer;
    object->collision = collions;
    object->objectType = objectType;

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

void Object_Destroy(struct Object *object) {
    Animation_RemoveAnimations(object);

}

void Object_Print(const struct Object *object) {
    printf("Object:\n\tName: %s\n", object->name);
    // for (int i = 0; i < object->animationsCount; ++i) {
    //     Animation_PrintAnimation(&object->animations[i]);
    // }
}
