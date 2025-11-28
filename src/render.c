//
// Created by standa on 10/30/25.
//
#include "render.h"

#include <SDL_timer.h>

#include "ui.h"

bool Render_Object(SDL_Renderer *ren, struct Object *object) {
    double scale = 1;
    SDL_Rect dst = {object->position.x*scale, object->position.y*scale, object->size.x*scale, object->size.y*scale}; // dimenze vykreseleni

    SDL_RendererFlip flippedMirrored = SDL_FLIP_NONE;

    if (&object->animations[object->activeAnimationIndex] == NULL) {
        printf("Chyba u objektu %s",object->name);
    }

    switch (object->animations[object->activeAnimationIndex].mirroredFlipped) {
        case ANIMATION_FLIP: {
            flippedMirrored = SDL_FLIP_HORIZONTAL;
        }break;
        case ANIMATION_MIRROR: {
            flippedMirrored = SDL_FLIP_VERTICAL;
        }break;
        case ANIMATION_MIRROR_FLIP: {
            flippedMirrored = SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL;
        }break;
    }


    if (object->objectAnimationsType == ANIMATIONS_SINGLE) { // jestlize je objekt ciste jeden obrazek
        double angle = (object->objectType == OBJECT_DYNAMIC)?Object_GetAngleFromDir(object->objectDir):0;

        SDL_RenderCopyEx(
            ren,
            object->animations[object->activeAnimationIndex].frames[0].texture,
            NULL,
            &dst,
            angle,
            NULL,
            flippedMirrored);
    }else {
        if (object->animations[object->activeAnimationIndex].currentFrame >= object->animations[object->activeAnimationIndex].framesCount) { // jestlize je aktulani frame na konci, tak ho vynuluj, aby se aniamce zacla prehavat od zacatku
                object->animations[object->activeAnimationIndex].currentFrame = 0;
        }

        int currFrame = object->animations[object->activeAnimationIndex].currentFrame; // ziska aktulani frame a vykresli ho
        SDL_RenderCopyEx(
            ren,
            object->animations[object->activeAnimationIndex].frames[currFrame].texture,
            NULL,
            &dst,
            0.0,
            NULL,
            flippedMirrored);

        Uint32 time = SDL_GetTicks(); // ziska cas od zacatku zapnuti programu

        if (time - object->animations[object->activeAnimationIndex].lastFrameTime >= object->animations[object->activeAnimationIndex].frames[currFrame].timeMilis) { // nastavi dalsi frame, jestli uz uplynul cas, ktery frame musi trvat
            object->animations[object->activeAnimationIndex].currentFrame++;
            object->animations[object->activeAnimationIndex].lastFrameTime = SDL_GetTicks();
        }
    }

    // if (object->animations->repeatType == ANIMATION_ONCE) {
    //     if (object->animations[object->activeAnimationIndex].currentFrame >= object->animations[object->activeAnimationIndex].framesCount) {
    //         Object_SetActiveAnimationByName(object,"idle",object->animations[object->activeAnimationIndex].mirroredFlipped);
    //     }
    // }

    return false;
}


bool Render_UI(SDL_Renderer *ren,struct UI *ui) {
    double scale = 1;
    SDL_Rect dst = {ui->position.x*scale, ui->position.y*scale, ui->size.x*scale, ui->size.y*scale}; // dimenze vykreseleni

    if (ui->animation.frames != NULL) {

        if (ui->animation.framesCount == 1) { // jestlize je objekt ciste jeden obrazek

            SDL_RenderCopyEx(
                ren,
                ui->animation.frames[0].texture,
                NULL,
                &dst,
                0,
                NULL,
                SDL_FLIP_NONE);
        }else {
            if (ui->animation.currentFrame >= ui->animation.framesCount) { // jestlize je aktulani frame na konci, tak ho vynuluj, aby se aniamce zacla prehavat od zacatku
                    ui->animation.currentFrame = 0;
            }

            int currFrame = ui->animation.currentFrame; // ziska aktulani frame a vykresli ho

            SDL_RenderCopyEx(
                ren,
                ui->animation.frames[ui->animation.currentFrame].texture,
                NULL,
                &dst,
                0.0,
                NULL,
                SDL_FLIP_NONE);

            Uint32 time = SDL_GetTicks(); // ziska cas od zacatku zapnuti programu

            if (time - ui->animation.lastFrameTime >= ui->animation.frames[currFrame].timeMilis) { // nastavi dalsi frame, jestli uz uplynul cas, ktery frame musi trvat
                ui->animation.currentFrame++;
                ui->animation.lastFrameTime = SDL_GetTicks();
            }
        }
    }

    if (ui->contentType == UI_TEXT) {
        SDL_RenderCopyEx(
            ren,
            ui->content.text.textTexture,
            NULL,
            &dst,
            0,
            NULL,
            SDL_FLIP_NONE);
    }

    if (ui->child != NULL) {
        Render_UI(ren,ui->child);
    }
}