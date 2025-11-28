//
// Created by standa on 11/28/25.
//

#ifndef PROJEKT_GAMERULE_H
#define PROJEKT_GAMERULE_H
#include <SDL_stdinc.h>
#include <stdbool.h>

struct GameTimes {
    Uint32 startTime;
    Uint32 pauseTime;
    Uint32 timePaused;
};

struct Gamerule {
    bool gamePaused;
    struct GameTimes gameTimes;
};


void Gamerule_PauseGame(struct Gamerule *gamerule);
void Gamerule_UnpauseGame(struct Gamerule *gamerule);

#endif //PROJEKT_GAMERULE_H