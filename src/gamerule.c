//
// Created by standa on 11/28/25.
//

#include "gamerule.h"

#include <SDL_timer.h>

#include "object.h"
#include "player.h"
#include "weapons.h"

void Gamerule_PauseGame(struct Gamerule *gamerule) {
    gamerule->gamestates.gamePaused = true;
    gamerule->gameTimes.pauseTime = SDL_GetTicks();
}

void Gamerule_UnpauseGame(struct Gamerule *gamerule) {
    gamerule->gamestates.gamePaused = false;
    gamerule->gameTimes.timePaused += SDL_GetTicks() - gamerule->gameTimes.pauseTime;
}

bool Gamerule_ChangeGamestate(struct Gamerule *gamerule,struct Game_UIs *gameUIs, enum Gamestate newGameState) {
    if (gamerule == NULL) {return false;}
    if (gameUIs == NULL) {return false;}

    switch (newGameState) {
        case GAME_IN_GAME: {

        }break;
        case GAME_IN_MAIN_MENU: {

        }break;

        case GAME_PRE_PLAY: {

        }break;
        case GAME_POST_GAME: {

        }break;
    }
}



void Gamerule_SpawnPlayer(struct World *world, char *displayName, struct Vector2 playerPos, int playerIndex, bool isBot) {
    //object
    struct Vector2 player1Size = {100,100};

    char playerObjectName[64];
    strcpy(playerObjectName,(isBot)?"bot":"player");
    strcat(playerObjectName,"-");
    char playerIndexString[2];
    snprintf(playerIndexString, sizeof(playerIndexString), "%d", playerIndex+1);
    strcat(playerObjectName,playerIndexString);

    struct Object *object1 = Object_CreateObject(playerObjectName,player1Size,playerPos,0,COLLISION_OVERLAP,OBJECT_PLAYER,WEST);
    Animation_AddAnimationsToObject(world->renderer,object1,ANIMATIONS_OBJECT,0);
    //weapon
    struct Vector2 player1WeaponSize = {20,20};
    struct Vector2 player1WeaponPos = {500,500};
    struct Object *weaponObject1 = Object_CreateObject("pistol",player1WeaponSize,player1WeaponPos,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
    Animation_AddAnimationsToObject(world->renderer,weaponObject1,ANIMATIONS_OBJECT,0);

    struct Weapon *primaryWeapon1 = Weapon_CreateWeapon(weaponObject1,5,3);

    struct Player *player1 = Player_CreatePlayer(object1,primaryWeapon1,NULL,(isBot)?-1:playerIndex,5,2,isBot);
    strcpy(player1->displayName,displayName);

    // UI STATS
    SDL_Color color = {PLAYER_STATS_UI_TEXT_COLOR};

    struct Vector2 player1UIPos = {500,300};
    struct Vector2 player1UISize = {50,50};

    char statsUIIdentifier[50];
    strcat(statsUIIdentifier,player1->object.name);
    strcat(statsUIIdentifier,"-");
    strcat(statsUIIdentifier,"statsUI");

    struct UI *player1StatsUI = UI_CreateUI(statsUIIdentifier,player1UIPos,player1UISize,"K:0 D:0",NULL,false);
    Animation_AddAnimationToUI(world->renderer,player1StatsUI,"kda_UI");

    player1StatsUI->text.textTexture = UI_GetTextTexture(world->renderer,player1StatsUI->text.textToDisplay,color,PLAYER_STATS_UI_TEXT_SIZE);

    player1->stats.ui = *player1StatsUI;


    World_AddPlayer(world,player1);
    free(player1);
    player1 = NULL;

    free(weaponObject1);
    weaponObject1 = NULL;

}
// }void Gamerule_SpawnPlayers(struct World *world)  {
//     //object
//     struct Vector2 player1Size = {100,100};
//     struct Vector2 player1Pos = {500,500};
//     struct Object *object1 = Object_CreateObject("player1",player1Size,player1Pos,0,COLLISION_NONE,OBJECT_PLAYER,WEST);
//     Animation_AddAnimationsToObject(world->renderer,object1,ANIMATIONS_OBJECT,0);
//     //weapon
//     struct Vector2 player1WeaponSize = {20,20};
//     struct Vector2 player1WeaponPos = {500,500};
//     struct Object *weaponObject1 = Object_CreateObject("gun",player1WeaponSize,player1WeaponPos,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
//     Animation_AddAnimationsToObject(world->renderer,weaponObject1,ANIMATIONS_OBJECT,0);
//
//     struct Weapon *primaryWeapon1 = Weapon_CreateWeapon(weaponObject1,5,3);
//
//     struct Player *player1 = Player_CreatePlayer(object1,primaryWeapon1,NULL,1,5,5);
//
//     // UI STATS
//     SDL_Color color = {255,255,255,255};
//
//     struct Vector2 player1UIPos = {500,300};
//     struct Vector2 player1UISize = {50,50};
//
//     char statsUIIdentifier[50];
//     strcat(statsUIIdentifier,"statsUI");
//     strcat(statsUIIdentifier,"_");
//     strcat(statsUIIdentifier,player1->object.name);
//
//     struct UI *player1StatsUI = UI_CreateUI(statsUIIdentifier,player1UIPos,player1UISize,"K:0 D:0",NULL,false);
//     Animation_AddAnimationToUI(world->renderer,player1StatsUI,"kda_UI");
//
//     player1StatsUI->text.textTexture = UI_GetTextTexture(world->renderer,player1StatsUI->text.textToDisplay,color,25);
//
//     player1->stats.ui = *player1StatsUI;
//
//
//     World_AddPlayer(world,player1);
//     free(player1);
//     player1 = NULL;
//
//     free(weaponObject1);
//     weaponObject1 = NULL;
//
//
//
//     //player2
//
//     //object
//     struct Vector2 player2Size = {100,100};
//     struct Vector2 player2Pos = {600,300};
//     struct Object *object6 = Object_CreateObject("player2",player2Size,player2Pos,0,COLLISION_NONE,OBJECT_PLAYER,WEST);
//     Animation_AddAnimationsToObject(world->renderer,object6,ANIMATIONS_OBJECT,0);
//     //weapon
//     struct Vector2 player2UIPos = {600,200};
//     struct Vector2 player2UISize = {50,50};
//     struct Object *weaponObject2 = Object_CreateObject("gun",player2UISize,player2UIPos,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
//     Animation_AddAnimationsToObject(world->renderer,weaponObject2,ANIMATIONS_OBJECT,0);
//
//     struct Weapon *primaryWeapon2 = Weapon_CreateWeapon(weaponObject2,5,3);
//
//     struct Player *player2 = Player_CreatePlayer(object6,primaryWeapon2,NULL,0,5,5);
//
//     struct UI *player2StatsUI = UI_CreateUI(statsUIIdentifier,player2UIPos,player2UISize,"K:0 D:0",NULL,false);
//     Animation_AddAnimationToUI(world->renderer,player2StatsUI,"kda_UI");
//
//     player2StatsUI->text.textTexture = UI_GetTextTexture(world->renderer,player2StatsUI->text.textToDisplay,color,25);
//
//     player2->stats.ui = *player2StatsUI;
//
//     World_AddPlayer(world,player2);
//     free(player2);
//     player2 = NULL;
//
//     free(weaponObject2);
//     weaponObject2 = NULL;
//
// }

void Gamerule_SpawnObjects(struct World *world) {

    int w,h;
    SDL_GetRendererOutputSize(world->renderer,&w,&h);


    struct Vector2 size1 = {w,h};
    struct Vector2 pos1 = {0,0};
    struct Object *object1 = Object_CreateObject("pozadi",size1,pos1,0,COLLISION_NONE,OBJECT_STATIC,WEST);
    Animation_AddAnimationsToObject(world->renderer,object1,ANIMATIONS_SINGLE,0);

    World_AddObject(world,object1);
    free(object1);
    object1 = NULL;

    struct Vector2 size = {300,300};
    struct Vector2 pos = {50,50};
    struct Object *object = Object_CreateObject("psik",size,pos,0,COLLISION_BLOCK,OBJECT_STATIC,WEST);
    Animation_AddAnimationsToObject(world->renderer,object,ANIMATIONS_OBJECT,0);

    World_AddObject(world,object);
    free(object);
    object = NULL;

    struct Vector2 size3 = {50,50};
    struct Vector2 pos3 = {650,500};
    struct Object *object3 = Object_CreateObject("gun",size3,pos3,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
    Animation_AddAnimationsToObject(world->renderer,object3,ANIMATIONS_OBJECT,0);

    World_AddObject(world,object3);
    free(object3);
    object3 = NULL;

}

void Gamerule_StartGame(struct World *world,struct Gamerule *gamerule, char playerNames[4][64]) {
    Gamerule_SpawnObjects(world);
    int playerIndexing = 0;
    int botIndexing = 0;
    for (int i = 0; i < 4; ++i) {
        if (strcmp(playerNames[i],"") != 0) {
            struct Vector2 loc = {100*(i+1),400};
            Gamerule_SpawnPlayer(world,playerNames[playerIndexing],loc,playerIndexing,false);
            playerIndexing++;
            printf("cislo %d je hrac\n",i);
        }
        else {
            struct Vector2 loc = {200*i,600};
            Gamerule_SpawnPlayer(world,"BOT",loc,botIndexing,true);
            botIndexing++;
            printf("cislo %d je bot\n",i);
        }
        Player_OnMove(world,&world->players[i],WEST); // pohneme s postavickama, aby hned zacly animace
    }
    gamerule->gameTimes.startTime = SDL_GetTicks();

    for (int i = 0; i < 4; ++i) {
        printf("player %d name: %s\n",i,playerNames[i]);
    }
};

void Gamerule_EndGame(struct World *world,struct Gamerule *gamerule, bool saveStats) {

    Gamerule_SaveMatch(world,gamerule);

    gamerule->gamestates.gamestate = GAME_POST_GAME;
    World_Destroy(world);
};

struct UI_Manager *Gamerule_GetActiveUIManagerByGameState(struct Game_UIs *ui_managers,enum Gamestate gamestate) {
    struct UI_Manager *curUIManager = 0;

    switch (gamestate) {
        case GAME_IN_MAIN_MENU:{curUIManager=ui_managers->mainMenu;}break;
        case GAME_IN_GAME:{curUIManager=ui_managers->inGame;}break;
        case GAME_PRE_PLAY:{curUIManager=ui_managers->preGame;}break;
        case GAME_POST_GAME:{curUIManager=ui_managers->postGame;}break;
    }
    return curUIManager;

}


void Gamerule_SaveMatch(struct World *world,struct Gamerule *gamerule) {
    struct MatchSave match;

    match.gameLength = gamerule->gameTimes.gameLengthMinutes;

    for (int i = 0; i < world->playerCount; ++i) {
        struct Player player = world->players[i];

        strcpy(match.playerSave[i].name,player.displayName);
        match.playerSave[i].playerStats = malloc(sizeof(struct PlayerStats));
        memcpy(match.playerSave[i].playerStats,&player.stats,sizeof(struct PlayerStats));
    }

    struct MatchSave *oldMatches = NULL;
    int count = 0;
    Gamerule_GetMatchHistory(&oldMatches,&count);
    struct MatchSave *matches = realloc(oldMatches,(count + 1)*(sizeof(struct MatchSave)));
    memcpy(&matches[count], &match,sizeof(struct MatchSave));

    // for (int i = 0; i < 4; ++i) { // spatne
    //     //free(match.playerSave[i].playerStats);
    // }

    FILE *file = fopen("matchHistory", "wb");
    if (file == NULL) {
        perror("Nepodarilo se otervit matchHistory file");
        return;
    }

    fwrite(matches,sizeof(struct MatchSave),count + 1,file);

    fclose(file);

    free(matches);
}

void Gamerule_GetMatchHistory(struct MatchSave **matchSaves, int *count) {

    FILE *fp = fopen("matchHistory", "rb");

    if (fp == NULL) {
        *matchSaves = malloc(sizeof(struct MatchSave));
        perror("Nepodarilo se otervit matchHistory file");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    *count = fileSize / sizeof(struct MatchSave);
    *matchSaves = malloc(sizeof(struct MatchSave) * *count);

    fread(*matchSaves, sizeof(struct MatchSave), *count, fp);

    // printf("Pocet zapasu v matchHistory je %d (bez noveho)\n",*count);

    fclose(fp);
}

void Gamerule_GetScoreboard(struct PlayerSave *scoreboard[5]) {
    FILE *file = fopen("scoreboard", "rb");

    if (file == NULL) {
        perror("Nepodarilo se otervit scoreboard file");
        return;
    }

    fread(scoreboard,sizeof(struct PlayerSave),5,file);

    fclose(file);
}

void Gamerule_TryToAddToScoreboard(struct PlayerSave playerSave) {
    struct PlayerSave scoreboard[5];

    for (int i = 0; i < 5; ++i) {

    }


}
