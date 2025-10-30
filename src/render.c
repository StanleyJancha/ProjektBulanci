//
// Created by standa on 10/30/25.
//
#include "render.h"

#include <SDL_timer.h>

bool Render_Object(SDL_Renderer *ren, struct Object *object) {

    SDL_Rect dst = {object->position.x, object->position.y, object->size.x, object->size.y};

    if (object->objectAnimationsType == ANIMATIONS_OBJECT) {
            // jestli je current frame vetsi nez maximalni pocet, tak dej na 0
            if (object->animationsSet.objectAnimations.Idle.currentFrame >= object->animationsSet.objectAnimations.Idle.framesCount) {
                object->animationsSet.objectAnimations.Idle.currentFrame = 0;
            }

            int currFrame = object->animationsSet.objectAnimations.Idle.currentFrame;
            SDL_RenderCopy(
                ren,
                object->animationsSet.objectAnimations.Idle.frames[currFrame].texture, NULL, &dst);

            Uint32 time = SDL_GetTicks();

            if (time - object->animationsSet.objectAnimations.Idle.lastFrameTime >= object->animationsSet.objectAnimations.Idle.frames[currFrame].timeMilis) {
                object->animationsSet.objectAnimations.Idle.currentFrame++;
                object->animationsSet.objectAnimations.Idle.lastFrameTime = SDL_GetTicks();
            }
        }
    else if (object->objectAnimationsType == ANIMATIONS_SINGLE){
        SDL_RenderCopy(
        ren,
        object->animationsSet.objectAnimations.Idle.frames[0].texture, NULL, &dst);
    }

    return false;
}
