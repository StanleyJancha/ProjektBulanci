//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_BASICSTRUCT_H
#define PROJEKT_BASICSTRUCT_H
#include <SDL_render.h>
#include <stdbool.h>

struct Vector2 {
    int x;
    int y;
};

struct PlayerStats {
    int kills;
    int deaths;
};


struct UI_Text_Padding {
    int x;
    int y;
};

struct UI_Text {
    char textToDisplay[256];
    SDL_Texture* textTexture;
    int size;
    SDL_Color color;
    bool isInput;
    struct UI_Text_Padding padding;
};


struct Vector2 Vector2_Subtract(struct Vector2 u,struct Vector2 v);
float CalculateKD(int kills, int deaths);

#endif //PROJEKT_BASICSTRUCT_H