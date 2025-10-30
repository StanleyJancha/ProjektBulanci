//
// Created by standa on 10/30/25.
//

#ifndef PROJEKT_ANIMACE_H
#define PROJEKT_ANIMACE_H
#include <SDL_render.h>


struct Sprite {
    SDL_Texture *texture;
    char spritePath[64];
};

struct Animation {
    char name[32];
    struct Sprite *frames;
    int framesCount;
    int currentFrame;
};



#endif //PROJEKT_ANIMACE_H