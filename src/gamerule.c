//
// Created by standa on 11/28/25.
//

#include "gamerule.h"

#include <SDL_timer.h>

void Gamerule_PauseGame(struct Gamerule *gamerule) {
    gamerule->gamePaused = true;
    gamerule->gameTimes.pauseTime = SDL_GetTicks();
}

void Gamerule_UnpauseGame(struct Gamerule *gamerule) {
    gamerule->gamePaused = false;
    gamerule->gameTimes.timePaused += SDL_GetTicks() - gamerule->gameTimes.pauseTime;
}
