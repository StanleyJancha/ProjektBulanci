//
// Created by standa on 10/28/25.
//

#include "keybinds.h"
#include "player.h"

struct PlayerKeybindSet PlayerKeybindSets[NUM_PLAYER_KEYBINDSETS] = {
    {
        SDL_SCANCODE_UP,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_RIGHT,
        SDL_SCANCODE_SLASH
    },
{
    SDL_SCANCODE_W,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_V
    },
{
    SDL_SCANCODE_8,
    SDL_SCANCODE_4,
    SDL_SCANCODE_5,
    SDL_SCANCODE_6,
    SDL_SCANCODE_0
    },
    {
    SDL_SCANCODE_U,
    SDL_SCANCODE_H,
    SDL_SCANCODE_J,
    SDL_SCANCODE_K,
    SDL_SCANCODE_M
    }
};

int KeyBinds_GetPlayerIndexByInput(const struct World *world, SDL_Keycode key) {
    int KeyBindSetIndex = -1; // jaky keyBindSet byl pouzit

    for (int j = 0; j < NUM_PLAYER_KEYBINDSETS; ++j) { // hledame v jakem KeyBindSetu je prave zmacknuta klavesa
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

