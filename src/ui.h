//
// Created by standa on 11/15/25.
//

#ifndef PROJEKT_UI_H
#define PROJEKT_UI_H

#include "basicStructs.h"
#include "animace.h"

struct UI_Text {

};

struct UI_Button {
    char *functionality;
    struct UI_Text textUI;
};



union UI_TYPE {

};

struct UI {
    char *identifier;
    struct Vector2 position;
    struct Vector2 size;
    struct Animation animation;
    union UI_TYPE type;
};

struct UI_Manager {
    struct UI *UIs;
    int count;
};

SDL_Texture *UI_GetTextTexture(SDL_Renderer *renderer, char *text, SDL_Color color, int ptsize);
struct UI_Manager UI_Manager_Create();
void UI_Manager_Destroy(struct UI_Manager *manager);

#endif //PROJEKT_UI_H