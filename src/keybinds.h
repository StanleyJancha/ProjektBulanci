//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_KEYBINDS_H
#define PROJEKT_KEYBINDS_H
#include <SDL_events.h>

#include "world.h"

struct PlayerKeybindSet {
    enum SDL_KeyCode move_up;
    enum SDL_KeyCode move_left;
    enum SDL_KeyCode move_down;
    enum SDL_KeyCode move_right;
};


#define NUM_PLAYER_KEYBINDS 3
extern struct PlayerKeybindSet PlayerKeybindSets[NUM_PLAYER_KEYBINDS];

void KeyBinds_HandleInput(SDL_Event *e);
int KeyBinds_GetPlayerIndexByInput(const struct World *world, SDL_Keycode key);

#endif //PROJEKT_KEYBINDS_H