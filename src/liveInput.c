//
// Created by standa on 10/28/25.
//
#include "liveInput.h"

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

int LiveInput_RemoveKey(struct LiveInputKeyManager *liveInputKeyManager, SDL_EventType eventType, SDL_Keycode keyCode) {
    if (liveInputKeyManager->activeKeysCount <= 0){return -3;} // jesli je pocet keys 0, tak neni co mazat

    int indexToRemove = -1;

    for (int i = 0; i < liveInputKeyManager->activeKeysCount; i++) {
        if (liveInputKeyManager->activeKeys[i] == keyCode) { // hledame index klavesy, kterou chceme vymazat
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) { // pokud klavesa v poli neni vratime false
        return false;
    }

    if (liveInputKeyManager->activeKeysCount - 1 == 0) { // nastane, pokud se keycode nasel a zaroven je nova velikost nulova
        free(liveInputKeyManager->activeKeys);
        liveInputKeyManager->activeKeys = NULL;
        liveInputKeyManager->activeKeysCount = 0;
        return true;
    }

    SDL_Keycode *newKeys = malloc(sizeof(SDL_Keycode) * (liveInputKeyManager->activeKeysCount - 1) ); // vytvorime nove pole s velikosti o jednu mensi

    for (int i = 0; i < liveInputKeyManager->activeKeysCount; i++) {
        newKeys[i] = liveInputKeyManager->activeKeys[i - ((i >= indexToRemove)?1:0)];
    }

    liveInputKeyManager->activeKeys = newKeys;
    liveInputKeyManager->activeKeysCount -= 1;

    return true;
}

bool LiveInput_KeyInArray(struct LiveInputKeyManager *liveInputKeyManager, SDL_Keycode keyCode) {
    for (int i = 0; i < liveInputKeyManager->activeKeysCount; i++) {
        if (liveInputKeyManager->activeKeys[i] == keyCode) {
            return true;
        }
    }
    return false;
}


void LiveInput_Print(struct LiveInputKeyManager *liveInputKeyManager) {
    printf("KEYS:\n");
    for (int i = 0; i < liveInputKeyManager->activeKeysCount; i++) {
        printf("Keybind #%d: ", liveInputKeyManager->activeKeys[i]);
    }
    printf("\n");
}
