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

struct UI_Button {
    char functionName[64];
};


union UI_Content {
    struct UI_Text text;
    struct UI_Button button;
};

enum UI_ContentType {
    UI_TEXT,
    UI_BUTTON
};

struct UI {
    char identifier[64];
    struct Vector2 position;
    struct Vector2 size;
    struct Animation animation;
    enum UI_Visibility visibility;
    struct UI *child;
    union UI_Content content;
    enum UI_ContentType contentType;
};

struct UI_Manager {
    struct UI *UIs;
    int count;
};

SDL_Texture *UI_GetTextTexture(SDL_Renderer *renderer, char *text, SDL_Color color, int ptsize);
struct UI_Manager UI_Manager_Create();
struct UI *UI_CreateUI(char identifier[64], struct Vector2 position, struct Vector2 size);
struct UI *UI_CreateTextUI(char identifier[64], struct Vector2 position, struct Vector2 size, char defaultText[64]);
struct UI *UI_CreateButtonUI(char identifier[64], struct Vector2 position, struct Vector2 size, char functionName[64]);
bool UI_SetChild(struct UI *parent, struct UI *child);

bool UI_Manager_AddUI(struct UI_Manager *uiManager, struct UI *ui);
void UI_Manager_Destroy(struct UI_Manager *manager);
void UI_Manager_PrintAllUIs(struct UI_Manager *ui_manager);

struct UI *UI_Manager_GetUIByIdentifier(struct UI_Manager *uiManager,char *identifier);

struct UI *UI_MouseOnUI(struct UI_Manager uiManager,struct Vector2 mousePos);
bool UI_ButtonCallEvent(struct World *world,struct Gamerule *gamerule,char functionName[64]);

#endif //PROJEKT_UI_H