//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_KEYBINDS_H
#define PROJEKT_KEYBINDS_H
#include <SDL2/SDL.h>

#include "world.h"

struct PlayerKeybindSet {
    enum SDL_Scancode move_up;
    enum SDL_Scancode move_left;
    enum SDL_Scancode move_down;
    enum SDL_Scancode move_right;
    enum SDL_Scancode shoot;
};

#define NUM_PLAYER_KEYBINDS 4 // pocet keybindu na jednoho hrace

#define NUM_PLAYER_KEYBINDSETS 4 // kolik variant ruznych keybindu
extern struct PlayerKeybindSet PlayerKeybindSets[NUM_PLAYER_KEYBINDSETS];

void KeyBinds_HandleInput(SDL_Event *e);
int KeyBinds_GetPlayerIndexByInput(const struct World *world, SDL_Keycode key);

#endif //PROJEKT_KEYBINDS_H