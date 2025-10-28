//
// Created by standa on 10/28/25.
//

#include "keybinds.h"
#include "player.h"

struct PlayerKeybindSet PlayerKeybindSets[NUM_PLAYER_KEYBINDS] = {
    {
        SDLK_UP,
        SDLK_LEFT,
        SDLK_DOWN,
        SDLK_RIGHT,
    },
{
    SDLK_w,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    },
{
    SDLK_8,
    SDLK_4,
    SDLK_5,
    SDLK_6,
    }
};

int KeyBinds_GetPlayerIndexByInput(const struct World *world, SDL_Event *e) {
    SDL_Keycode key = e->key.keysym.sym; // zmackunta klavesa

    int KeyBindSetIndex = -1; // jaky keyBindSet byl pouzit

    for (int j = 0; j < NUM_PLAYER_KEYBINDS; ++j) { // hledame v jakem KeyBindSetu je prave zmacknuta klavesa
        if (key == PlayerKeybindSets[j].move_up ||
            key == PlayerKeybindSets[j].move_down ||
            key == PlayerKeybindSets[j].move_left ||
            key == PlayerKeybindSets[j].move_right)
            {
            KeyBindSetIndex = j;
            break;
            }
    }
    if (KeyBindSetIndex == -1) { // jeslize zmacknute tlacitko neni z ani jednoho KeyBindSetu
        return -1;
    }
    for (int j = 0; j < world->playerCount; ++j) { // hledame pro ziskany KeyBindSet strukturu Player ve World (ve svete)
        if (world->players[j].PlayerKeybindSetIndex == KeyBindSetIndex) {
            return j;
        }
    }

    return -1;
}

