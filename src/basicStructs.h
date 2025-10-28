//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_BASICSTRUCT_H
#define PROJEKT_BASICSTRUCT_H
#include <SDL_render.h>
#include <stdbool.h>

struct Vector2 {
    int x;
    int y;
};

struct Sprite {
    SDL_Texture *texture;
    char spritePath[64];
};
#endif //PROJEKT_BASICSTRUCT_H