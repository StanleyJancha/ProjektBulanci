//
// Created by standa on 11/28/25.
//

#ifndef PROJEKT_GAMERULE_H
#define PROJEKT_GAMERULE_H
#include <SDL_stdinc.h>
#include <stdbool.h>

enum Gamestate{
    GAME_IN_MAIN_MENU,
    GAME_PRE_PLAY,
    GAME_POST_GAME,
    GAME_IN_GAME
};

struct Gamestates {
    bool gamePaused;
    bool appRunning;
    enum Gamestate gamestate;
};

struct GameTimes {
    Uint32 startTime;
    Uint32 pauseTime;
    Uint32 timePaused;
};

struct Gamerule {
    struct Gamestates gamestates;
    struct GameTimes gameTimes;
    struct UI *inputUI;
};


void Gamerule_PauseGame(struct Gamerule *gamerule);
void Gamerule_UnpauseGame(struct Gamerule *gamerule);

struct Game_UIs;
struct UI_Manager *Gamerule_GetActiveUIManagerByGameState(struct Game_UIs *ui_managers,enum Gamestate gamestate);
struct Vector2;

struct World;
void Gamerule_SpawnPlayers(struct World *world);
void Gamerule_SpawnPlayer(struct World *world, char *displayName, struct Vector2 playerPos, int playerIndex,bool isBot);
void Gamerule_SpawnObjects(struct World *world);
void Gamerule_StartGame(struct World *world,struct Gamerule *gamerule, char playerNames[4][64]);
void Gamerule_EndGame(struct World *world,struct Gamerule *gamerule,bool saveStats);
#endif //PROJEKT_GAMERULE_H