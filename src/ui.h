//
// Created by standa on 11/15/25.
//

#ifndef PROJEKT_UI_H
#define PROJEKT_UI_H

#include "basicStructs.h"
#include "animace.h"
#include "gamerule.h"

struct World;

enum UI_Visibility {
    VISIBLE,
    HIDDEN
};

struct UI_Text {
    char textToDisplay[64];
    SDL_Texture* textTexture;
};

struct UI_Events {
    char onClick[64];
};

struct UI {
    char identifier[64];
    struct Vector2 position;
    struct Vector2 size;
    struct Animation animation;
    enum UI_Visibility visibility;
    struct UI *child;
    struct UI_Text text;
    struct UI_Events *events;
};

struct UI_Manager {
    struct UI *UIs;
    int count;
};

SDL_Texture *UI_GetTextTexture(SDL_Renderer *renderer, char *text, SDL_Color color, int ptsize);
struct UI_Manager UI_Manager_Create();
struct UI *UI_CreateUI(char identifier[64], struct Vector2 position, struct Vector2 size,char text[64],struct UI_Events *events);

bool UI_SetChild(struct UI *parent, struct UI *child);

bool UI_Manager_AddUI(struct UI_Manager *uiManager, struct UI *ui);
void UI_Manager_Destroy(struct UI_Manager *manager);
void UI_Manager_PrintAllUIs(struct UI_Manager *ui_manager);

struct UI *UI_Manager_GetUIByIdentifier(struct UI_Manager *uiManager,char *identifier);

struct UI *UI_MouseOnUI(struct UI_Manager uiManager,struct Vector2 mousePos);
bool UI_ButtonCallEvent(struct World *world,struct Gamerule *gamerule,char *eventName);
#endif //PROJEKT_UI_H