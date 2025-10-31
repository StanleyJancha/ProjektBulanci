//
// Created by standa on 10/30/25.
//
#include "render.h"

#include <SDL_timer.h>

bool Render_Object(SDL_Renderer *ren, struct Object *object) {

    SDL_Rect dst = {object->position.x, object->position.y, object->size.x, object->size.y}; // dimenze vykreseleni

    if (object->objectAnimationsType == ANIMATIONS_SINGLE) { // jestlize je objekt ciste jeden obrazek
        SDL_RenderCopyEx(ren,object->animations[object->activeAnimationIndex].frames[0].texture, NULL, &dst,0.0,NULL,((object->animations[object->activeAnimationIndex].mirrored)?SDL_FLIP_VERTICAL:SDL_FLIP_NONE));

    }else {
        if (object->animations[object->activeAnimationIndex].currentFrame >= object->animations[object->activeAnimationIndex].framesCount) { // jestlize je aktulani frame na konci, tak ho vynuluj, aby se aniamce zacla prehavat od zacatku
                object->animations[object->activeAnimationIndex].currentFrame = 0;
        }


        int currFrame = object->animations[object->activeAnimationIndex].currentFrame; // ziska aktulani frame a vykresli ho
        SDL_RenderCopyEx(
            ren,
            object->animations[object->activeAnimationIndex].frames[currFrame].texture, NULL, &dst, 0.0,NULL,((object->animations[object->activeAnimationIndex].mirrored)?SDL_FLIP_VERTICAL:SDL_FLIP_NONE));

        Uint32 time = SDL_GetTicks(); // ziska cas od zacatku zapnuti programu

        if (time - object->animations[object->activeAnimationIndex].lastFrameTime >= object->animations[object->activeAnimationIndex].frames[currFrame].timeMilis) { // nastavi dalsi frame, jestli uz uplynul cas, ktery frame musi trvat
            object->animations[object->activeAnimationIndex].currentFrame++;
            object->animations[object->activeAnimationIndex].lastFrameTime = SDL_GetTicks();
        }
    }

    return false;
}
