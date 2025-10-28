//
// Created by standa on 10/28/25.
//

#include <SDL_image.h>
#include <SDL_render.h>
#include <stdbool.h>

#include "object.h"

bool Object_SetTexture(SDL_Renderer *ren, struct Object *object) {
    SDL_Surface* surface = IMG_Load(object->sprites[0].spritePath);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return false;
    }

    object->sprites[0].texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    return object->sprites[0].texture != NULL;
}

void Object_Print(const struct Object *object) {
    printf("Object:\n\t\t\tName: %s\n", object->name);
}
