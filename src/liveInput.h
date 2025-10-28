//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_LIVEINPUT_H
#define PROJEKT_LIVEINPUT_H
#include <SDL_events.h>

struct LiveInputKeyManager {
    SDL_Keycode *activeKeys;
    int activeKeysCount;
};

#endif //PROJEKT_LIVEINPUT_H