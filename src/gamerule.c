//
// Created by standa on 11/28/25.
//

#include "gamerule.h"

#include <SDL_timer.h>
#include <stdlib.h>

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



void Gamerule_SpawnPlayer(struct World *world, char *displayName, struct Vector2 playerPos, int playerIndex, bool isBot, enum PlayerColors playerColor, int index) {
    //object
    struct Vector2 player1Size = {100,100};

    char playerObjectName[64];
    strcpy(playerObjectName,(isBot)?"bot":"player");
    strcat(playerObjectName,"-");
    char playerIndexString[2];
    snprintf(playerIndexString, sizeof(playerIndexString), "%d", playerIndex+1);
    strcat(playerObjectName,playerIndexString);

    struct Object *object1 = Object_CreateObject(playerObjectName,player1Size,playerPos,0,COLLISION_OVERLAP,OBJECT_PLAYER,WEST);

    char objectName[60];
    switch (playerColor) {
        case PLAYER_RED: {
            strcpy(objectName,"red");
        }break;
        case PLAYER_GREEN: {
            strcpy(objectName,"green");
        }break;
        case PLAYER_BLUE: {
            strcpy(objectName,"blue");
        }break;
        case PLAYER_WHITE: {
            strcpy(objectName,"white");
        }break;
        default: {
            strcpy(objectName,"");
        }
    }

    Animation_AddAnimationsToObject(world->renderer,object1,ANIMATIONS_OBJECT,0,objectName); //
    //weapon
    struct Vector2 player1WeaponSize = {50,50};
    struct Vector2 player1WeaponPos = {500,500};
    struct Object *weaponObject1 = Object_CreateObject("pistol",player1WeaponSize,player1WeaponPos,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
    Animation_AddAnimationsToObject(world->renderer,weaponObject1,ANIMATIONS_OBJECT,0,NULL);

    struct Weapon *primaryWeapon1 = Weapon_CreateWeapon(weaponObject1,5,3);

    struct Player *player1 = Player_CreatePlayer(object1,primaryWeapon1,NULL,(isBot)?-1:index,5,2,isBot);
    strcpy(player1->displayName,displayName);

    // UI STATS

    struct Vector2 player1UIPos = {500,300};
    struct Vector2 player1UISize = {60,20};

    char statsUIIdentifier[50];
    strcat(statsUIIdentifier,player1->object.name);
    strcat(statsUIIdentifier,"-");
    strcat(statsUIIdentifier,"statsUI");

    struct UI *player1StatsUI = UI_CreateUI(statsUIIdentifier,player1UIPos,player1UISize,"K:0 D:0",NULL,false);
    Animation_AddAnimationToUI(world->renderer,player1StatsUI,"kda_UI");

    player1StatsUI->text.color.r = 0;
    player1StatsUI->text.color.g = 0;
    player1StatsUI->text.color.b = 0;

    player1StatsUI->text.size = 10;
    player1StatsUI->text.textTexture = UI_GetTextTexture(world->renderer,player1StatsUI->text);

    UI_Text_SetPadding(&player1StatsUI->text,5,0);

    player1->playerStatsUI = *player1StatsUI;


    Object_SetRandomPosition(world,&player1->object,100,980,100,620);


    World_AddPlayer(world,player1);
    free(player1);
    player1 = NULL;

    free(weaponObject1);
    weaponObject1 = NULL;
}

void Gamerule_SpawnObjects(struct World *world) {

    int w,h;
    SDL_GetRendererOutputSize(world->renderer,&w,&h);


    struct Vector2 size1 = {w,h};
    struct Vector2 pos1 = {0,0};
    struct Object *object1 = Object_CreateObject("pozadi",size1,pos1,0,COLLISION_NONE,OBJECT_STATIC,WEST);
    Animation_AddAnimationsToObject(world->renderer,object1,ANIMATIONS_SINGLE,0,NULL);

    World_AddObject(world,object1);
    free(object1);
    object1 = NULL;

    struct Vector2 size = {150,150};
    struct Vector2 pos = {50,300};
    struct Object *object = Object_CreateObject("crate",size,pos,0,COLLISION_BLOCK,OBJECT_STATIC,WEST);
    Animation_AddAnimationsToObject(world->renderer,object,ANIMATIONS_OBJECT,0,NULL);

    World_AddObject(world,object);
    free(object);
    object = NULL;

    struct Vector2 size2 = {50,50};
    struct Vector2 pos2 = {500,330};
    struct Object *object2 = Object_CreateObject("crate",size2,pos2,0,COLLISION_BLOCK,OBJECT_STATIC,WEST);
    Animation_AddAnimationsToObject(world->renderer,object2,ANIMATIONS_OBJECT,0,NULL);

    World_AddObject(world,object2);
    free(object2);
    object2 = NULL;

    struct Vector2 size4 = {100,100};
    struct Vector2 pos4 = {460,500};
    struct Object *object4 = Object_CreateObject("crate",size4,pos4,0,COLLISION_BLOCK,OBJECT_STATIC,WEST);
    Animation_AddAnimationsToObject(world->renderer,object4,ANIMATIONS_OBJECT,0,NULL);

    World_AddObject(world,object4);
    free(object4);
    object4 = NULL;

    struct Vector2 size3 = {50,50};
    struct Vector2 pos3 = {650,500};
    struct Object *object3 = Object_CreateObject("gun",size3,pos3,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
    Animation_AddAnimationsToObject(world->renderer,object3,ANIMATIONS_OBJECT,0,NULL);

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
            Gamerule_SpawnPlayer(world,playerNames[playerIndexing],loc,playerIndexing,false,i,i);
            playerIndexing++;
            printf("cislo %d je hrac\n",i);
        }
        else {
            struct Vector2 loc = {200*i,600};
            Gamerule_SpawnPlayer(world,"BOT",loc,botIndexing,true,i,i);
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

void Gamerule_EndGame(struct World *world,struct Gamerule *gamerule, struct Game_UIs *gameUIs,bool saveStats) {
    gamerule->inputUI = NULL;
    Game_UIs_ClearAllTextfieldsInManager(world,gameUIs->preGame);

    if (saveStats == false) {
        gamerule->gamestates.gamestate = GAME_IN_MAIN_MENU;
    }
    else {
        gamerule->gamestates.gamestate = GAME_POST_GAME;

        struct MatchSave curMatch = Gamerule_SaveMatch(world,gamerule);

        char scoreboardText[256] = {""};


        char tmpInt[5];
        for (int i = 0; i < 4; ++i) {
            strcat(scoreboardText,curMatch.playerSave[i].name);
            strcat(scoreboardText,"   Kills: ");
            sprintf(tmpInt,"%d",curMatch.playerSave[i].playerStats.kills);
            strcat(scoreboardText,tmpInt);

            strcat(scoreboardText,"   Deaths: ");
            sprintf(tmpInt,"%d",curMatch.playerSave[i].playerStats.deaths);
            strcat(scoreboardText,tmpInt);
            strcat(scoreboardText,"\n");
        }
        strcat(scoreboardText,"\0");

        struct UI *scoreboardUi = UI_Manager_GetUIByIdentifier(gameUIs->postGame, "score_board");
        strcpy(scoreboardUi->text.textToDisplay,scoreboardText);
        scoreboardUi->text.textTexture = UI_GetTextTexture(world->renderer,scoreboardUi->text);

        for (int i = 0; i < 4; ++i) {
            Gamerule_TryToAddToScoreboard(&curMatch.playerSave[i]);
        }
    }

    struct UI * mainmenuScoreboard = UI_Manager_GetUIByIdentifier(gameUIs->mainMenu,"score_board_main_menu");
    Gamerule_UpdateMainMenuScoreboard(world->renderer,&mainmenuScoreboard->text);

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


struct MatchSave Gamerule_SaveMatch(struct World *world,struct Gamerule *gamerule) {
    struct MatchSave match;

    match.gameLength = gamerule->gameTimes.gameLengthMinutes;

    for (int i = 0; i < world->playerCount; ++i) {
        struct Player player = world->players[i];

        strcpy(match.playerSave[i].name,player.displayName);
        match.playerSave[i].playerStats.kills = player.stats.kills;
        match.playerSave[i].playerStats.deaths = player.stats.deaths;
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

    }
    else {
        fwrite(matches,sizeof(struct MatchSave),count + 1,file);
    }

    fclose(file);
    free(matches);

    return match;
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

void Gamerule_GetScoreboard(struct PlayerSave **scoreboard) {
    FILE *file = fopen("scoreboard", "rb");

    if (file == NULL) {
        perror("Nepodarilo se otervit scoreboard file");
        *scoreboard = NULL;
        return;
    }

    fread(*scoreboard,sizeof(struct PlayerSave),5,file);

    fclose(file);
}

void Gamerule_TryToAddToScoreboard(struct PlayerSave *playerSave) {
    struct PlayerSave *scoreboard = malloc(sizeof(struct PlayerSave) * 5);
    Gamerule_GetScoreboard(&scoreboard);

    if (scoreboard == NULL) {
        scoreboard = malloc(sizeof(struct PlayerSave) * 5);
        for (int i = 0; i < 5; ++i) {
            strcpy(scoreboard[i].name,"-");
            scoreboard[i].playerStats.kills = 0;
            scoreboard[i].playerStats.deaths = 0;
        }
    }

    FILE *file = fopen("scoreboard", "wb");

    if (file == NULL) {
        perror("Nepodarilo se otervit scoreboard file");
        return;
    }


    int insertIdx = -1;
    float playerKD = CalculateKD(playerSave->playerStats.kills, playerSave->playerStats.deaths);

    for (int i = 0; i < 5; i++) {
        float currentKD = CalculateKD(scoreboard[i].playerStats.kills, scoreboard[i].playerStats.deaths);
        if (playerKD > currentKD) {
            insertIdx = i;
            break;
        }
    }

    if (insertIdx != -1) {
        for (int i = 4; i > insertIdx; i--) {
            scoreboard[i] = scoreboard[i - 1];
        }
        scoreboard[insertIdx] = *playerSave;
    }

    fwrite(scoreboard,sizeof(struct PlayerSave),5,file);

    fclose(file);

    free(scoreboard);
}


void Gamerule_UpdateMainMenuScoreboard(SDL_Renderer *renderer,struct UI_Text *scorboardTextUI) {
    struct PlayerSave *scoreboard = malloc(sizeof(struct PlayerSave)*5);
    Gamerule_GetScoreboard(&scoreboard);
    char scoreboardText[256] = {0};

    strcat(scoreboardText,"Scoreboard\n");

    if (scoreboard != NULL) {
        for (int i = 0; i < 5; ++i) {
            struct PlayerSave playerSave = scoreboard[i];
            char line[64];
            snprintf(line, sizeof(line),
                "%d. %s   KD: %.2f\n", i+1,playerSave.name,
                (strcmp(playerSave.name, "-") != 0)?CalculateKD(playerSave.playerStats.kills,playerSave.playerStats.deaths):0);
            strcat(scoreboardText,line);
        }
        strcat(scoreboardText,"\0");
    }
    strcpy(scorboardTextUI->textToDisplay,scoreboardText);
    scorboardTextUI->textTexture = UI_GetTextTexture(renderer,*scorboardTextUI);
}

void Gamerule_HandleInput(struct World *world, const Uint8 *keys) {
    for (int i = world->playerCount - 1; i >= 0; --i) {
        struct Player *player = &world->players[i];

        if (player->deathStatus.dead) {
            continue;
        }

        if (player->PlayerKeybindSetIndex == -1) { // PLAYER je bot, takze nema zadne keybindy
            continue;
        }

        struct PlayerKeybindSet playerKeybindSet = PlayerKeybindSets[world->players[i].PlayerKeybindSetIndex];


        if (keys[playerKeybindSet.move_up]) {
            Player_OnMove(world,player,NORTH);

        }else
        if (keys[playerKeybindSet.move_right]) {
            Player_OnMove(world,player,EAST);

        }else
        if (keys[playerKeybindSet.move_down]) {
            Player_OnMove(world,player,SOUTH);

        }else
        if (keys[playerKeybindSet.move_left]) {
            Player_OnMove(world,player,WEST);
        }
        else {
            Object_SetActiveAnimationByName(&player->object,"idle",ANIMATION_NOT_MIRRORED_FLIPPED);
        }

    }
}

void Gamerule_SpawnWeaponLogic(struct World *world) {
    bool gunInGame = false; // jestli je sekundarni zbran stale nesebrana hracem

    for (int i = 0; i < world->objectCount; ++i) {
        if (strcmp(world->objects[i].name,"gun") == 0) {
            gunInGame = true;
            break;
        }
    }
    int randomVal = rand()%1000;
    if (!gunInGame && randomVal < 1) {
        struct Vector2 size3 = {50,50};
        struct Vector2 pos3 = {-200,-200};
        struct Object *object3 = Object_CreateObject("gun",size3,pos3,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
        Animation_AddAnimationsToObject(world->renderer,object3,ANIMATIONS_OBJECT,0,NULL);

        Object_SetRandomPosition(world,object3,100,900,100,600);

        World_AddObject(world,object3);
        free(object3);
        object3 = NULL;

    }
}


int Gamerule_UpdateTimer(struct World *world,struct Gamerule *gamerule, struct Game_UIs *game_UIs, int gameSeconds) {
    char timerText[6];

    // region Setting game time for UI
    struct UI *clockUI = UI_Manager_GetUIByIdentifier(game_UIs->inGame,"game_timer");
    if (clockUI) {
        sprintf(timerText,"%02d:%02d",
            (gameSeconds/60%100),
            (gameSeconds%60)%100);
        if (strcmp(timerText,clockUI->text.textToDisplay) != 0) {
            strcpy(clockUI->text.textToDisplay,timerText);
            clockUI->text.textTexture = UI_GetTextTexture(world->renderer,clockUI->text);
        }

    }else{printf("Nelze najit UI pro TIMER ve hre\n");}
}