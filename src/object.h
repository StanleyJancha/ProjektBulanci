//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_OBJECT_H
#define PROJEKT_OBJECT_H
#include "basicStructs.h"

struct Object {
    char name[32];
    struct Vector2 size;
    struct Vector2 position;
    struct Sprite *sprites; // pole struktur sprite - struktura pro texturu a jeji cestu
    int spriteCount;
};

bool Object_SetTexture(SDL_Renderer *ren, struct Object *object);

void Object_Print(const struct Object *object);

#endif //PROJEKT_OBJECT_H