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


bool Object_MoveBy(struct Object *object, struct Vector2 addVector) {
    struct Vector2 newPos = {object->position.x + addVector.x, object->position.y + addVector.y};

    object->position = newPos;
}

bool Object_CheckCollision(struct World *world, struct Object *object) {
    for (int i = 0; i < world->objectCount; ++i) {
        printf("is player Colliding: %d", Collsions_areColliding(object, &world->objects[i]));

    }
}

void Object_Destroy(struct Object *object) {
    Animation_RemoveAnimations(object);

}

void Object_Print(const struct Object *object) {
    printf("\t\tObject:\n\t\t\tName: %s\n", object->name);
}
