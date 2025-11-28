//
// Created by standa on 10/30/25.
//

#ifndef PROJEKT_ANIMACE_H
#define PROJEKT_ANIMACE_H
#include <SDL2/SDL.h>
#include <stdbool.h>

struct Object;
struct UI;

enum ObjectAnimationsType {
    ANIMATIONS_PLAYER,
    ANIMATIONS_OBJECT,
    ANIMATIONS_SINGLE
};

enum ObjectAnimationRepeatType {
    ANIMATION_REPEAT,
    ANIMATION_ONCE
};


struct Sprite {
    SDL_Texture *texture;
    char spritePath[256];
    int timeMilis;
};

enum AnimationMirrorFlip {
    ANIMATION_NOT_MIRRORED_FLIPPED,
    ANIMATION_MIRROR,
    ANIMATION_FLIP,
    ANIMATION_MIRROR_FLIP
};
struct Animation {
    char name[32];
    struct Sprite *frames;
    int framesCount;
    int currentFrame;
    enum ObjectAnimationRepeatType repeatType;
    Uint32 lastFrameTime;
    enum AnimationMirrorFlip mirroredFlipped;
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

enum Animation_ObjectType {
    ANIMATION_OBJECT,
    ANIMATION_UI
};

int Animation_AddAnimationsToObject(SDL_Renderer *renderer, struct Object *object, enum ObjectAnimationsType objectAnimationsType, int AnimationSetIndex) ;
bool Animation_GetAnimation(struct Animation *animation,char objectName[32], char animName[32]);
bool Animation_RemoveAnimations(struct Object *object);
bool Animation_RemoveAnimation(struct Animation *animation);
int Animation_AddAnimationToUI(SDL_Renderer *renderer, struct UI *ui);
void Animation_PrintAnimation(const struct Animation *animation);

#endif //PROJEKT_ANIMACE_H