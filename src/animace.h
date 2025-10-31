//
// Created by standa on 10/30/25.
//

#ifndef PROJEKT_ANIMACE_H
#define PROJEKT_ANIMACE_H
#include <SDL2/SDL.h>
#include <stdbool.h>

struct Object;

enum ObjectAnimationsType {
    ANIMATIONS_PLAYER,
    ANIMATIONS_OBJECT,
    ANIMATIONS_SINGLE
};


struct Sprite {
    SDL_Texture *texture;
    char spritePath[256];
    int timeMilis;
};

struct Animation {
    char name[32];
    struct Sprite *frames;
    int framesCount;
    int currentFrame;
    Uint32 lastFrameTime;
    bool mirrored;
};

// #define PLAYER_ANIMATIONS_COUNT 3
// struct PlayerAnimations {
//     struct Animation Idle;
//     struct Animation MoveUp;
//     struct Animation MoveRight;
// };
// #define OBJECT_ANIMATIONS_COUNT 1
// struct ObjectAnimations {
//     struct Animation Idle;
// };
//
// union AnimationSetType {
//     struct PlayerAnimations playerAnimations;
//     struct ObjectAnimations objectAnimations;
//     struct Animation singleAnimation;
// };

int Animation_AddAnimationsToObject(SDL_Renderer *renderer, struct Object *object, enum ObjectAnimationsType objectAnimationsType, int AnimationSetIndex) ;
bool Animation_GetAnimation(struct Animation *animation,char name[32], char animName[32]);
bool Animation_RemoveAnimations(struct Object *object);
void Animation_PrintAnimation(const struct Animation *animation);

#endif //PROJEKT_ANIMACE_H