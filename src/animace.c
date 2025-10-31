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

    if (filesCount < 1) {
        printf("Nenasly se sprity pro animaci: %s pro objekt: %s",animName,name);
    }

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
    strcpy(animation->name, animName);
    animation->currentFrame = 0;
    animation->frames = NULL;
    animation->lastFrameTime = 0;
    animation->mirrored = false;
    Animation_SetSprites(renderer,&animation->frames,&animation->framesCount,name,animName);

    return true;

}

int Animation_GetAnimationsCount(char objectName[32], char animNames[99][32]) {
    char path[256];

    snprintf(path, 256, "anims/%s", objectName); // vytvori cestu
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        return 0;
    }
    struct dirent *entry;
    int animationsCount = 0;
    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (entry->d_type == DT_DIR) {
            strcpy(animNames[animationsCount],entry->d_name);
            animationsCount++;
        }
        if (animationsCount >= 99) {
            printf("ERROR: pri pocet animaci pro objekt '%s' presahl limit %d",objectName,99);
            closedir(dir);
            return 0;
        }
    }
    closedir(dir);
    return animationsCount;
}


int Animation_AddAnimationsToObject(SDL_Renderer *renderer, struct Object *object, enum ObjectAnimationsType objectAnimationsType, int AnimationSetIndex) {

    char animationNames[99][32];

    int animationsCount = Animation_GetAnimationsCount(object->name,animationNames);

    // for (int i = 0; i < animationsCount; ++i) {
    //     printf("Object: %s | anim: %s\n",object->name,animationNames[i]);
    // }

    if (animationsCount < 1) {
        printf("Nenasly se animace pro objekt: %s",object->name);
    }
    object->animationSetIndex = 0;
    object->activeAnimationIndex = 0;
    object->animationsCount = animationsCount;
    object->animations = malloc(sizeof(struct Animation) * object->animationsCount);

    for (int i = 0; i < object->animationsCount; ++i) {
        Animation_SetAnimation(renderer,&object->animations[i],object->name,animationNames[i]);
    }

    return 1;
}

bool Animation_RemoveAnimations(struct Object *object) {
    for (int i = 0; i < object->animationsCount; ++i) {
        for (int j = 0; j < object->animations[i].framesCount; ++j) {
            SDL_DestroyTexture(object->animations[i].frames[j].texture);
        }

        free(object->animations[i].frames);
        object->animations[i].frames = NULL;
    }

    free(object->animations);
    object->animations = NULL;

    return true;
}


void Animation_PrintAnimation(const struct Animation *animation) {
    printf("\tAnimation\n\t\tname: %s\n\t\tframes:\n",animation->name);
    for (int i = 0; i < animation->framesCount; ++i) {
        printf("\t\t\tframe path: %s\n",animation->frames[i].spritePath);
    }
}

