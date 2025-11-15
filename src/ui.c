//
// Created by standa on 11/15/25.
//
#include <SDL_ttf.h>
#include "ui.h"

void UI_Manager_Destroy(struct UI_Manager *manager) {
    for (int i = 0; i < manager->count; ++i) {

    }
}


struct UI_Manager UI_Manager_Create() {
    struct UI_Manager ui_manager;
    ui_manager.UIs = NULL;
    ui_manager.count = 0;
    return ui_manager;
}

SDL_Texture *UI_GetTextTexture(SDL_Renderer *renderer, char *text, SDL_Color color, int ptsize) {
    TTF_Font *font = TTF_OpenFont("arial.ttf", 32);
    if (!font) {printf("Font error: %s\n", TTF_GetError());return NULL;}

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}
