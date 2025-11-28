//
// Created by standa on 11/15/25.
//
#include <SDL_ttf.h>
#include "ui.h"

#include "world.h"

void UI_Manager_Destroy(struct UI_Manager *manager) {
    for (int i = 0; i < manager->count; ++i) {
        Animation_RemoveAnimation(&manager->UIs[i].animation);
        if (manager->UIs[i].contentType == UI_TEXT) {
            SDL_DestroyTexture(manager->UIs[i].content.text.textTexture);
        }
    }
}


struct UI_Manager UI_Manager_Create() {
    struct UI_Manager ui_manager;
    ui_manager.UIs = NULL;
    ui_manager.count = 0;
    return ui_manager;
}

struct UI *UI_CreateUI(char identifier[64], struct Vector2 position, struct Vector2 size) {
    struct UI *ui = malloc(sizeof(struct UI));
    if (ui == NULL) return NULL;
    strcpy(ui->identifier, identifier);
    ui->size = size;
    ui->position = position;
    ui->visibility = VISIBLE;
    ui->child = NULL;

    return ui;
}

void UI_Text_SetTextTexture(struct World world, struct UI_Text ui) {

}

struct UI *UI_CreateTextUI(char identifier[64], struct Vector2 position, struct Vector2 size, char defaultText[64]) {
    struct UI *ui = UI_CreateUI(identifier,position,size);
    ui->contentType = UI_TEXT;

    strcpy(ui->content.text.textToDisplay,defaultText);

    return ui;
}

struct UI *UI_CreateButtonUI(char identifier[64], struct Vector2 position, struct Vector2 size, char functionName[64]) {
    struct UI *ui = UI_CreateUI(identifier,position,size);
    ui->contentType = UI_BUTTON;

    strcpy(ui->content.button.functionName,functionName);

    return ui;
}

bool UI_SetChild(struct UI *parent, struct UI *child) {
    if (parent == NULL) {printf("UI_SetChild - parent not valid") ;return false;}
    if (child == NULL) {printf("UI_SetChild - child for \"%s\" not valid",parent->identifier) ;return false;}

    parent->child = child;
    return true;

}

bool UI_Manager_AddUI(struct UI_Manager *uiManager, struct UI *ui) {
    if (uiManager->count == 0) {
        uiManager->UIs = malloc(sizeof(struct UI) * 1);
        if (uiManager->UIs == NULL){printf("UI_Manager_Add - nepodarilo se nacist pamet");return false;}
        uiManager->UIs[0] = *ui;
        uiManager->count = 1;
        return true;
    }

    struct UI *temp = malloc(sizeof(struct UI) * (uiManager->count + 1));;
    if (temp == NULL) {printf("UI_Manager_Add - nepodarilo se nacist pamet");return false;}
    for (int i = 0; i < uiManager->count; ++i) {
        temp[i] = uiManager->UIs[i];
    }
    temp[uiManager->count] = *ui;
    uiManager->UIs = temp;
    uiManager->count++;
    return true;
}



SDL_Texture *UI_GetTextTexture(SDL_Renderer *renderer, char *text, SDL_Color color, int ptsize) {
    TTF_Font *font = TTF_OpenFont("fonts/Playwrite.ttf", ptsize);
    if (!font) {printf("Font error: %s\n", TTF_GetError());return NULL;}

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}


void UI_Print(struct UI *ui) {
    printf("id: %s\nanimation name:\n",ui->identifier);
}

void UI_Manager_PrintAllUIs(struct UI_Manager *ui_manager) {
    for (int i = 0; i < ui_manager->count; ++i) {
        UI_Print(&ui_manager->UIs[i]);
    }
}

bool UI_MouseInside(struct UI *ui, struct Vector2 mouse) {
    return  mouse.x >= ui->position.x &&
            mouse.x <= ui->position.x + ui->size.x &&
            mouse.y >= ui->position.y &&
            mouse.y <= ui->position.y + ui->size.y;
}


struct UI *UI_MouseOnUI(struct UI_Manager uiManager,struct Vector2 mousePos) {
    for (int i = uiManager.count - 1; i >= 0; --i) {
        if (uiManager.UIs[i].contentType == UI_BUTTON) {
            if (UI_MouseInside(&uiManager.UIs[i],mousePos)) {
                return &uiManager.UIs[i];
            }
        }
    }
    return NULL;
}

bool UI_ButtonCallEvent(struct World *world,struct Gamerule *gamerule,char functionName[64]) {
    if (strcmp(functionName,"unpause_game") == 0) {
        Gamerule_UnpauseGame(gamerule);
    }else
    if (strcmp(functionName,"") == 0) {

    }
}

struct UI *UI_Manager_GetUIByIdentifier(struct UI_Manager *uiManager,char *identifier) {
    for (int i = 0; i < uiManager->count; ++i) {
        if (strcmp(uiManager->UIs[i].identifier,identifier) == 0) {
            return &uiManager->UIs[i];
        }
    }
    return NULL;
}
