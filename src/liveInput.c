//
// Created by standa on 10/28/25.
//
#include "liveInput.h"

#include <SDL_events.h>
#include <stdbool.h>

bool LiveInput_AddKey(struct LiveInputKeyManager *liveInputKeyManager, SDL_EventType eventType, SDL_Keycode keyCode) {
    SDL_Keycode *newKeys = malloc(sizeof(SDL_Keycode) * (liveInputKeyManager->activeKeysCount + 1) );

    if (newKeys == 0) {
        return false;
    }

    for (int i = 0; i < liveInputKeyManager->activeKeysCount; i++) {
            newKeys[i] = liveInputKeyManager->activeKeys[i];
        }
    newKeys[liveInputKeyManager->activeKeysCount] = keyCode;

    liveInputKeyManager->activeKeys = newKeys;
    liveInputKeyManager->activeKeysCount += 1;
    return true;
}
// bool World_AddPlayer(struct World *world, struct Player *player) {
//     struct Player *newArray = malloc(sizeof(struct Player) * (world->playerCount + 1));
//
//     if (newArray == 0) {
//         return false;
//     }
//     for (int i = 0; i < world->playerCount; i++) {
//         newArray[i] = world->players[i];
//     }
//     newArray[world->playerCount] = *player;
//
//     world->players = newArray;
//     world->playerCount += 1;
//     return true;
// }