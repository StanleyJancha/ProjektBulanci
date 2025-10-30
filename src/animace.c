//
// Created by standa on 10/30/25.
//

#include "animace.h"

#include <stdlib.h>
#include <dirent.h>
#include <SDL_image.h>

#include "object.h"

bool Animation_SetTexture(SDL_Renderer *ren, struct Sprite *sprite) {
    SDL_Surface* surface = IMG_Load(sprite->spritePath);
    if (!surface) {
        printf("IMG_Load failed for %s: %s\n", sprite->spritePath, IMG_GetError());
        return false;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);

    sprite->texture = texture;
    SDL_FreeSurface(surface);
    return true;
}

bool Animation_SetSprites(SDL_Renderer *renderer,struct Sprite **sprites, int *spriteCount,char name[32], char animName[32]) {
    struct dirent *entry;

    char path[256];

    snprintf(path, 256, "anims/%s/%s", name, animName); // vytvori cestu

    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        return false;
    }

    int filesCount = 0; // pocet frames ve slozce
    char temporaryArray[99][256];


    while ((entry = readdir(dir)) != NULL) { // ziskava soubory

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // pokud to neni soubor, ale dir
            continue;

        char fullPath[256];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name); // vytvori celou cestu
        strcpy(temporaryArray[filesCount], fullPath);

        // printf("%s\n", fullPath);
        filesCount++;
    }
    closedir(dir);

    *sprites = malloc(sizeof(struct Sprite) * filesCount);

    for (int i = 0; i < filesCount; ++i) {
        strcpy((*sprites)[i].spritePath,temporaryArray[i]); // nastaveni cesty
        Animation_SetTexture(renderer,&(*sprites)[i]); // nastavi textury
        if (strcmp(name,"psik") == 0) {
            (*sprites)[i].timeMilis = 500;//
        }
        else {
            (*sprites)[i].timeMilis = 100;//
        }
    }
    *spriteCount = filesCount;
}

bool Animation_SetAnimation(SDL_Renderer *renderer,struct Animation *animation,char name[32], char animName[32]) {
    if (strcmp(animName, "idle") == 0) {
        strcpy(animation->name, "idle");
        animation->currentFrame = 0;
        animation->frames = NULL;
        animation->lastFrameTime = 0;
        Animation_SetSprites(renderer,&animation->frames,&animation->framesCount,name,animName);

        return true;

    }else
    if (strcmp(animName, "") == 0) {

    }
    return false;
}



int Animation_AddAnimationsToObject(SDL_Renderer *renderer, struct Object *object, enum ObjectAnimationsType objectAnimationsType, int AnimationSetIndex) {
    struct Animation newAnimIdle;
    Animation_SetAnimation(renderer,&newAnimIdle,object->name,"idle");

    if (objectAnimationsType == ANIMATIONS_OBJECT) { // pokud je objekt hrac

        struct ObjectAnimations newObjectAnimations = {  // ziskej animace
            newAnimIdle
        };
        object->animationsSet.objectAnimations = newObjectAnimations;
        object->objectAnimationsType = objectAnimationsType;
        object->animationsCount = PLAYER_ANIMATIONS_COUNT;

    } else if (objectAnimationsType == ANIMATIONS_SINGLE) {
        object->animationsSet.singleAnimation = newAnimIdle;
        object->objectAnimationsType = objectAnimationsType;
        object->animationsCount = 1;

    // }else if (objectAnimationsType == ANIMATIONS_SINGLE) {
    //     object->animations = malloc(sizeof(struct Animation));
    //     object->animationsCount = 1;
    }else {
        printf("ERROR: neznamy ObjectAnimationsType");
    }

    return 1;
}

bool Animation_RemoveAnimations(struct Object *object) {
    free(object->animationsSet.objectAnimations.Idle.frames);
    object->animationsSet.objectAnimations.Idle.frames = NULL;

    return true;
}