//
// Created by standa on 11/28/25.
//

#ifndef PROJEKT_GAMERULE_H
#define PROJEKT_GAMERULE_H

#include <SDL_stdinc.h>
#include <stdbool.h>

enum PlayerColors {
    PLAYER_WHITE,
    PLAYER_GREEN,
    PLAYER_RED,
    PLAYER_BLUE
};


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
    unsigned int gameLengthMinutes;
    Uint32 startTime;
    Uint32 pauseTime;
    Uint32 timePaused;
};

struct Gamerule {
    struct Gamestates gamestates;
    struct GameTimes gameTimes;
    struct UI *inputUI;
};

struct PlayerSave {
    char name[60];
    struct PlayerStats *playerStats;
};

struct MatchSave {
    unsigned int gameLength;
    struct PlayerSave playerSave[4];
};



void Gamerule_PauseGame(struct Gamerule *gamerule);
void Gamerule_UnpauseGame(struct Gamerule *gamerule);

struct Game_UIs;
struct UI_Manager *Gamerule_GetActiveUIManagerByGameState(struct Game_UIs *ui_managers,enum Gamestate gamestate);
struct Vector2;

struct World;
void Gamerule_SpawnPlayers(struct World *world);
void Gamerule_SpawnPlayer(struct World *world, char *displayName, struct Vector2 playerPos, int playerIndex, bool isBot, enum PlayerColors playerColor);
void Gamerule_SpawnObjects(struct World *world);
void Gamerule_StartGame(struct World *world,struct Gamerule *gamerule, char playerNames[4][64]);
void Gamerule_EndGame(struct World *world,struct Gamerule *gamerule, struct UI_Manager *endScreenUI,bool saveStats);

struct MatchSave Gamerule_SaveMatch(struct World *world,struct Gamerule *gamerule);
void Gamerule_GetMatchHistory(struct MatchSave **matchSaves, int *count);


void Gamerule_GetScoreboard();
void Gamerule_SetScoreboard();

#endif //PROJEKT_GAMERULE_H