//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_LIVEINPUT_H
#define PROJEKT_LIVEINPUT_H
#include <SDL_events.h>
#include <stdbool.h>

struct LiveInputKeyManager {
    SDL_Keycode *activeKeys;
    int activeKeysCount;
};

bool LiveInput_AddKey(struct LiveInputKeyManager *liveInputKeyManager, SDL_EventType eventType, SDL_Keycode keyCode);
int LiveInput_RemoveKey(struct LiveInputKeyManager *liveInputKeyManager, SDL_EventType eventType, SDL_Keycode keyCode);
bool LiveInput_KeyInArray(struct LiveInputKeyManager *liveInputKeyManager, SDL_Keycode keyCode);
void LiveInput_Print(struct LiveInputKeyManager *liveInputKeyManager);
#endif //PROJEKT_LIVEINPUT_H