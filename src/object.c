//
// Created by standa on 10/28/25.
//

#include <SDL_image.h>
#include <SDL_render.h>
#include <stdbool.h>

#include "object.h"

#include "collisions.h"
#include "world.h"

bool Object_SetTextures(SDL_Renderer *ren, struct Object *object) {
    for (int i = 0; i < object->spriteCount; ++i) {
        SDL_Surface* surface = IMG_Load(object->sprites[i].spritePath);
        if (!surface) {
            printf("Failed to load image: %s\n", IMG_GetError());
            return false;
        }

        object->sprites[i].texture = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
    }
    return true;
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

void Object_Destroy(struct Object * object) {
    // for (int i = 0; i < object->spriteCount; ++i) {
    //     free(object->sprites[i].spritePath);
    //     free(object->sprites[i].texture);
    // }
    // free(object->name);
    free(object->sprites);
    object->sprites = NULL;
}

void Object_Print(const struct Object *object) {
    printf("\t\tObject:\n\t\t\tName: %s\n", object->name);
}
