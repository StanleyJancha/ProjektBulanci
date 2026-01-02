#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

#include "src/animace.h"
#include "src/gamerule.h"

#include "src/object.h"
#include "src/keybinds.h"
#include "src/player.h"

#include "src/collisions.h"
#include "src/weapons.h"
#include "src/ai.h"
#include "src/render.h"
#include "src/ui.h"


#define GAME_LOOP_MILLIS_DELAY 16
#define RESPAWN_COOLDOWN_MS 2000

void handleInput(struct World *world, const Uint8 *keys) {
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



void loadMainMenuUIs(struct World *world,struct UI_Manager *ui_manager) {

    struct Vector2 exitGameButtonpos = {500,300};
    struct Vector2 exitGameButtonsize = {200,100};

    struct UI_Events *exitGameButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(exitGameButtonEvents->onClick, "exit_game");

    struct UI *exitGameButton = UI_CreateUI("exit_button",exitGameButtonpos,exitGameButtonsize,"Exit game",exitGameButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,exitGameButton,"basic_button");

    exitGameButton->text.color.r = 0;
    exitGameButton->text.color.g = 0;
    exitGameButton->text.color.b = 0;

    exitGameButton->text.size = 28;

    exitGameButton->text.textTexture = UI_GetTextTexture(world->renderer,exitGameButton->text);
    UI_Text_SetPadding(&exitGameButton->text,10,10);

    UI_Manager_AddUI(ui_manager,exitGameButton);

    free(exitGameButton);


    struct Vector2 startGamepos = {500,100};
    struct Vector2 startGamesize = {200,100};

    struct UI_Events *startGameEvents = malloc(sizeof(struct UI_Events));
    strcpy(startGameEvents->onClick, "start_game");

    struct UI *startGame = UI_CreateUI("start_button",startGamepos,startGamesize,"Start game",startGameEvents,false);
    Animation_AddAnimationToUI(world->renderer,startGame,"basic_button");

    startGame->text.color.r = 0;
    startGame->text.color.g = 0;
    startGame->text.color.b = 0;

    startGame->text.size = 28;
    startGame->text.textTexture = UI_GetTextTexture(world->renderer,startGame->text);
    UI_Text_SetPadding(&startGame->text,10,10);

    UI_Manager_AddUI(ui_manager,startGame);

    free(startGame);
}

void loadPreGameUIs(struct World *world,struct UI_Manager *ui_manager) {
    SDL_Color color = {0,0,0,255};

    struct Vector2 beginMatchButtonPos3 = {500,500};
    struct Vector2 beginMatchButtonSize3 = {200,100};

    struct UI_Events *beginMatchButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(beginMatchButtonEvents->onClick, "begin_match");

    struct UI *beginMatchButton = UI_CreateUI("begin_match_button",beginMatchButtonPos3,beginMatchButtonSize3,"Begin Match",beginMatchButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,beginMatchButton,"basic_button");

    if (strcmp(beginMatchButton->text.textToDisplay, "") != 0) {
        beginMatchButton->text.color.r = 0;
        beginMatchButton->text.color.g = 0;
        beginMatchButton->text.color.b = 0;

        beginMatchButton->text.size = 25;
        beginMatchButton->text.textTexture = UI_GetTextTexture(world->renderer,beginMatchButton->text);
        UI_Text_SetPadding(&beginMatchButton->text,10,10);
    }

    UI_Manager_AddUI(ui_manager,beginMatchButton);

    free(beginMatchButton);

    /////

    for (int i = 0; i < 4; ++i) {

        struct Vector2 pos1 = {200,50 + (i)*150};
        struct Vector2 size1 = {200,100};

        char identifier[64] =  "player_text_field-";
        char iToStringBuffer[20];

        sprintf(iToStringBuffer, "%d", i);
        strcat(identifier,iToStringBuffer);

        struct UI *textInput1 = UI_CreateUI_TextField(world,identifier,pos1,size1,"basic_player_text_field");

        textInput1->text.color.r = 0;
        textInput1->text.color.g = 0;
        textInput1->text.color.b = 0;

        textInput1->text.size = 20;
        textInput1->text.textTexture = UI_GetTextTexture(world->renderer,textInput1->text);
        UI_Text_SetPadding(&textInput1->text,10,10);


        UI_Manager_AddUI(ui_manager,textInput1);

        free(textInput1);

    }

    UI_Manager_PrintAllUIs(ui_manager);
}



void loadPostGameUIs(struct World *world,struct UI_Manager *ui_manager) {
    SDL_Color color = {0,0,0,255};

    struct Vector2 pos3 = {700,500};
    struct Vector2 size3 = {200,100};

    struct UI_Events *mainMenuButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(mainMenuButtonEvents->onClick, "exit_to_main_menu");

    struct UI *mainMenuButton = UI_CreateUI("main_menu_button",pos3,size3,"Main Menu",mainMenuButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,mainMenuButton,"basic_button");

    if (strcmp(mainMenuButton->text.textToDisplay, "") != 0) {
        mainMenuButton->text.color.r = 0;
        mainMenuButton->text.color.g = 0;
        mainMenuButton->text.color.b = 0;

        mainMenuButton->text.size = 25;
        mainMenuButton->text.textTexture = UI_GetTextTexture(world->renderer,mainMenuButton->text);
        UI_Text_SetPadding(&mainMenuButton->text,10,10);
    }

    UI_Manager_AddUI(ui_manager,mainMenuButton);

    free(mainMenuButton);

    struct Vector2 pos2 = {0,0};
    struct Vector2 size2 = {200,100};

    struct UI *scoreBoard = UI_CreateUI("score_board",pos2,size2,"winner is",NULL,false);
    Animation_AddAnimationToUI(world->renderer,scoreBoard,NULL);

    scoreBoard->text.color.r = 0;
    scoreBoard->text.color.g = 255;
    scoreBoard->text.color.b = 255;

    scoreBoard->text.size = 40;
    scoreBoard->text.textTexture = UI_GetTextTexture(world->renderer,scoreBoard->text);

    UI_Manager_AddUI(ui_manager,scoreBoard);

    free(scoreBoard);
}



void loadPauseMenuUIs(struct World *world,struct UI_Manager *ui_manager) {
    SDL_Color color = {0,0,0,255};

    struct Vector2 pos = {500,300};
    struct Vector2 size = {200,100};

    struct UI_Events *pauseButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(pauseButtonEvents->onClick, "unpause_game");

    struct UI *pauseButton = UI_CreateUI("button_unpause",pos,size,"Unpause",pauseButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,pauseButton,"basic_button");


    pauseButton->text.color.r = 0;
    pauseButton->text.color.g = 0;
    pauseButton->text.color.b = 0;

    pauseButton->text.size = 25;
    pauseButton->text.textTexture = UI_GetTextTexture(world->renderer,pauseButton->text);
    UI_Text_SetPadding(&pauseButton->text,20,10);


    UI_Manager_AddUI(ui_manager,pauseButton);

    free(pauseButton);

    /////

    struct Vector2 pos3 = {500,500};
    struct Vector2 size3 = {200,100};

    struct UI_Events *mainMenuButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(mainMenuButtonEvents->onClick, "exit_to_main_menu");

    struct UI *mainMenuButton = UI_CreateUI("main_menu_button",pos3,size3,"Main Menu",mainMenuButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,mainMenuButton,"basic_button");

    if (strcmp(mainMenuButton->text.textToDisplay, "") != 0) {

        mainMenuButton->text.color.r = 0;
        mainMenuButton->text.color.g = 0;
        mainMenuButton->text.color.b = 0;

        mainMenuButton->text.size = 20;
        mainMenuButton->text.textTexture = UI_GetTextTexture(world->renderer,mainMenuButton->text);

    }

    UI_Manager_AddUI(ui_manager,mainMenuButton);

    free(mainMenuButton);
}
void loadInGameUIs(struct World *world,struct UI_Manager *ui_manager) {

    struct Vector2 pos2 = {0,0};
    struct Vector2 size2 = {200,100};

    struct UI *gameTimerUI = UI_CreateUI("game_timer",pos2,size2,"00:00",NULL,false);
    Animation_AddAnimationToUI(world->renderer,gameTimerUI,NULL);

    gameTimerUI->text.color.r = 0;
    gameTimerUI->text.color.g = 0;
    gameTimerUI->text.color.b = 0;

    gameTimerUI->text.size = 40;
    gameTimerUI->text.textTexture = UI_GetTextTexture(world->renderer,gameTimerUI->text);

    UI_Text_SetPadding(&gameTimerUI->text,20,10);

    UI_Manager_AddUI(ui_manager,gameTimerUI);

    free(gameTimerUI);

}


int main() {
    // setup
    int width = 1080;
    int height = 720;
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *win = SDL_CreateWindow("Bulanci",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       width, height,
                                       0
                                       );

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 1;
    }

    struct Gamerule gamerule = {{0,1,GAME_IN_MAIN_MENU},{5,0,0,0}};

    struct Game_UIs game_UIs = {NULL,NULL,NULL,NULL,NULL};

    struct World world = World_Create();
    world.renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // RI PRVNIM ZAPNUTI SE OBJEVI
    game_UIs.mainMenu = UI_Manager_Create();
    loadMainMenuUIs(&world,game_UIs.mainMenu);

    game_UIs.pauseMenu = UI_Manager_Create();
    loadPauseMenuUIs(&world,game_UIs.pauseMenu);

    game_UIs.inGame = UI_Manager_Create();
    loadInGameUIs(&world,game_UIs.inGame);

    game_UIs.postGame = UI_Manager_Create();
    loadPostGameUIs(&world,game_UIs.postGame);

    game_UIs.preGame = UI_Manager_Create();
    loadPreGameUIs(&world,game_UIs.preGame);

    //TEST ROVNOU DO HRY
    // char PlayerNames[4][64] = {"fwef","fwefw","",""};
    // Gamerule_StartGame(&world,&gamerule,PlayerNames);
    // World_Print(&world);
    // gamerule.gamestates.gamestate = GAME_IN_GAME;

    SDL_Event e;
    SDL_StopTextInput();

    Uint32 lastFrameTime = SDL_GetTicks();

    while (gamerule.gamestates.appRunning) {
        // region Region Event Loop
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:{ // ZAVRIT OKNO
                    gamerule.gamestates.appRunning = false;
                    break;
                }

                case SDL_TEXTINPUT: {
                    int inputLength = strlen(e.text.text);

                    if (strlen(gamerule.inputUI->text.textToDisplay) + inputLength  < sizeof(gamerule.inputUI->text.textToDisplay) - 1 && strlen(gamerule.inputUI->text.textToDisplay) < 11) {
                        strcat(gamerule.inputUI->text.textToDisplay, e.text.text);
                        gamerule.inputUI->text.textTexture = UI_GetTextTexture(world.renderer,gamerule.inputUI->text);
                    }


                }break;

                case SDL_KEYDOWN: {
                    if (!gamerule.gamestates.gamePaused && gamerule.gamestates.gamestate == GAME_IN_GAME) {
                        for (int i = 0; i < world.playerCount; ++i) {
                            if (world.players[i].deathStatus.dead){continue;}
                            if (world.players[i].isBot){continue;}
                            if (e.key.keysym.scancode == PlayerKeybindSets[world.players[i].PlayerKeybindSetIndex].shoot) {
                                Player_Shoot(&world,&world.players[i],&gamerule);
                            }

                        }
                    }


                    switch (e.key.keysym.scancode) {
                        case SDL_SCANCODE_O: {
                        }break;
                        case SDL_SCANCODE_P: {
                            World_Print(&world);
                        }break;
                        case SDL_SCANCODE_ESCAPE:{ // ZAVRIT OKNO
                            if (gamerule.gamestates.gamestate == GAME_IN_GAME) {
                                (gamerule.gamestates.gamePaused)?Gamerule_UnpauseGame(&gamerule):Gamerule_PauseGame(&gamerule);
                            }
                            break;
                        }
                            case SDL_SCANCODE_BACKSPACE:{ // ZAVRIT OKNO
                            if (gamerule.inputUI != NULL && strlen(gamerule.inputUI->text.textToDisplay) >= 1) {
                                gamerule.inputUI->text.textToDisplay[strlen(gamerule.inputUI->text.textToDisplay)-1] = '\0';
                                gamerule.inputUI->text.textTexture = UI_GetTextTexture(world.renderer,gamerule.inputUI->text);
                            }
                            break;
                        }
                    }
                }break;
                case SDL_MOUSEBUTTONDOWN: {
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        struct Vector2 mousePos = {e.button.x,e.button.y};

                        struct UI_Manager *curManager = Gamerule_GetActiveUIManagerByGameState(&game_UIs,gamerule.gamestates.gamestate);

                        struct UI *clickedOnUI = UI_MouseOnUI(
                            curManager// ziskam aktivni ui_manager
                            ,mousePos); // nekontroluje zatim child UI, pouze prvni vrstvu

                        if (gamerule.gamestates.gamestate == GAME_IN_GAME && gamerule.gamestates.gamePaused) {
                            clickedOnUI = UI_MouseOnUI(game_UIs.pauseMenu,mousePos);
                        }
                        if (clickedOnUI != NULL) {
                            printf("Clicknul na '%s' UI\n",clickedOnUI->identifier);
                            UI_ButtonCallEvent(&world,&gamerule,curManager,clickedOnUI);
                        }
                    }

                }break;
            }
        }
        // endregion

        if (!gamerule.gamestates.gamePaused) {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            handleInput(&world, keys);

            for (int i = 0; i < world.playerCount; ++i) {
                struct Player *player = &world.players[i];


                if (player->deathStatus.dead == true) {
                    if (player->deathStatus.deathAnimationPlaying == true){
                        if (player->object.animations[player->object.activeAnimationIndex].currentFrame >= player->object.animations[player->object.activeAnimationIndex].framesCount) {
                            player->deathStatus.deathAnimationPlaying = false;
                        }
                    }else
                    if (SDL_GetTicks() - gamerule.gameTimes.timePaused - player->deathStatus.lastDeathTime > RESPAWN_COOLDOWN_MS) {
                        Player_Respawn(&world,player);
                    }
                }

                if (player->isBot) {
                    Ai_BotTick(&world,player,&gamerule);
                }
            }


            // region Dealing with dynamic objects (bullets and guns)
            for (int i = 0; i < world.objectCount; ++i) {
                if (world.objects[i].objectType == OBJECT_DYNAMIC) {
                    Object_Tick(&world.objects[i]);
                }
            }

            for (int i = 0; i < world.objectCount; ++i) {
                if (world.objects[i].objectType == OBJECT_DYNAMIC) {
                    char name[32];
                    strcpy(name,world.objects[i].name);
                    char *token = strtok(name,"_");
                    if (strcmp(token,"bullet") == 0) {
                        bool bulletDestroyed = false;

                        token = strtok(NULL,"_");
                        if (token == NULL){continue;}
                        for (int j = 0; j < world.objectCount; ++j) {
                            if (i == j){continue;}
                            if (world.objects[j].objectType != OBJECT_STATIC){continue;}
                            if (strcmp(world.objects[j].name,"pozadi") == 0){continue;}
                            if (Collsions_areColliding(&world.objects[i],&world.objects[j])) {
                                // printf("o1 %s \n o2 %s\n",&world.objects[i].name,&world.objects[j].name);
                                World_RemoveObject(&world,&world.objects[i],true);
                                bulletDestroyed = true;
                                break;
                            }
                        }

                        if (bulletDestroyed){continue;}

                        for (int j = 0; j < world.playerCount; ++j) {
                            if (Collsions_areColliding(&world.objects[i],&world.players[j].object)) {
                                if (strcmp(token,world.players[j].object.name) != 0) { //jestlize kulka neni od hrace, ktery ji vystrelil
                                    if (Player_TakeDamage(&world.players[j],1) == 1) { // jestlize kulka zabila hrace
                                        struct Player *killerPlayer = Player_GetByName(&world,token);
                                        killerPlayer->stats.kills++;
                                        Player_UpdateStatsUITexture(world.renderer,killerPlayer);
                                        Player_UpdateStatsUITexture(world.renderer,&world.players[j]);
                                    }
                                    World_RemoveObject(&world,&world.objects[i],true);
                                    break;
                                }
                            }
                        }
                    }
                    else if (1) {

                    }
                }
                else if (world.objects[i].objectType == OBJECT_PICKUP_WEAPON) {
                    for (int j = 0; j < world.playerCount; ++j) {
                        if (Collsions_areColliding(&world.objects[i],&world.players[j].object)) {
                            if (world.players[j].secondaryWeapon == NULL) { // pickup secondary weapon
                                world.objects[i].collision = COLLISION_NONE;
                                Player_PickUpWeapon(&world.players[j],&world.objects[i]);
                                World_RemoveObject(&world,&world.objects[i],false);
                                break;
                            }
                        }
                    }
                }
            }
            // endregion
        }

        /* ----------------- END Main loop ----------------- */


        // region Rendering objects and players

        SDL_RenderClear(world.renderer);

        if (gamerule.gamestates.gamestate == GAME_IN_GAME){
            // renderuj game objekty a ui, kdyz hra bezi

            // Static objects
        for (int i = 0; i < world.objectCount; ++i) {
            if (world.objects[i].objectType == OBJECT_STATIC) {
                Render_Object(world.renderer, &world.objects[i]);
            }
        }

        // Pickable weapons
        for (int i = 0; i < world.objectCount; ++i) {
            if (world.objects[i].objectType == OBJECT_PICKUP_WEAPON) {
                Render_Object(world.renderer, &world.objects[i]);
            }
        }

        // Players
        for (int i = 0; i < world.playerCount; ++i) {
            if (world.players[i].deathStatus.dead && world.players[i].deathStatus.deathAnimationPlaying == false){continue;}

            Render_Object(world.renderer, &world.players[i].object);

            if (world.players[i].secondaryWeapon != NULL) {
                Render_Object(world.renderer, &world.players[i].secondaryWeapon->object);
            }else
            if (world.players[i].primaryWeapon != NULL) {
                Render_Object(world.renderer, &world.players[i].primaryWeapon->object);
            }
            Render_UI(world.renderer,&world.players[i].stats.ui);
        }

        // Dynamic objects
        for (int i = 0; i < world.objectCount; ++i) {
            if (world.objects[i].objectType == OBJECT_DYNAMIC) {
                Render_Object(world.renderer, &world.objects[i]);
            }
        }
        // endregion

        // region Bullet time destruction
        if (!gamerule.gamestates.gamePaused) {   // mazani objektu kulek po case
            int bulletDestroyTimeMs = 2000; // zivotnost BULLET

            int *objectsToDestroyIndexes = malloc(sizeof(int)*world.objectCount);// buffer pro objekty na vymazani
            for (int i = 0; i < world.objectCount; ++i) {
                objectsToDestroyIndexes[i] = -1;
            }

            int j = 0; // index pro pozici v bufferu
            for (int i = 0; i < world.objectCount; ++i) {
                char nameCopy[32];
                strcpy(nameCopy,world.objects[i].name);
                if (strcmp(strtok(nameCopy,"_"),"bullet") == 0) {
                    if ((SDL_GetTicks() - gamerule.gameTimes.timePaused) - world.objects[i].spawnTime > bulletDestroyTimeMs) {
                        objectsToDestroyIndexes[j] = i;
                        j++;
                    }
                }
            }
            for (int i = 0; i < world.objectCount; ++i) { // mazani objektu
                if (objectsToDestroyIndexes[i] != -1) { // jestlize byl vybrany nejaky objekt na smazani
                    World_RemoveObject(&world,&world.objects[objectsToDestroyIndexes[i]],false);
                }
                else {// kdyz nebyl, tak muzeme zrusit cyklus, protoze pokud je jeden -1, tak vsechny za nim taky budou
                    break;
                }
            }
            free(objectsToDestroyIndexes);
        }
        //regionend

        // region Spawn Gun
        if (!gamerule.gamestates.gamePaused) {
            bool gunInGame = false; // jestli je sekundarni zbran stale nesebrana hracem

            for (int i = 0; i < world.objectCount; ++i) {
                if (strcmp(world.objects[i].name,"gun") == 0) {
                    gunInGame = true;
                    break;
                }
            }
            int randomVal = rand()%1000;
            if (!gunInGame && randomVal < 1) {
                struct Vector2 size3 = {50,50};
                struct Vector2 pos3 = {-200,-200};
                struct Object *object3 = Object_CreateObject("gun",size3,pos3,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
                Animation_AddAnimationsToObject(world.renderer,object3,ANIMATIONS_OBJECT,0,NULL);

                Object_SetRandomPosition(&world,object3,100,900,100,600);

                World_AddObject(&world,object3);
                free(object3);
                object3 = NULL;

            }


        }

        // regionend
        if (!gamerule.gamestates.gamePaused) {
            Uint32 curTime = SDL_GetTicks();
            char timerText[6];
            int seconds = (curTime-gamerule.gameTimes.startTime-gamerule.gameTimes.timePaused)/1000;


            // region Setting game time for UI
            struct UI *clockUI = UI_Manager_GetUIByIdentifier(game_UIs.inGame,"game_timer");
            if (clockUI) {
                sprintf(timerText,"%02d:%02d",
                    (seconds/60%100),
                    (seconds%60)%100);
                if (strcmp(timerText,clockUI->text.textToDisplay) != 0) {
                    strcpy(clockUI->text.textToDisplay,timerText);
                    clockUI->text.textTexture = UI_GetTextTexture(world.renderer,clockUI->text);
                }

            }else{printf("Nelze najit UI pro TIMER ve hre\n");}


            // if (seconds > 2) { // jestli vyprsi cas hry
            //     Gamerule_EndGame(&world,&gamerule,game_UIs.postGame,true);
            // }

            if (seconds/60 > gamerule.gameTimes.gameLengthMinutes) { // jestli vyprsi cas hry
                Gamerule_EndGame(&world,&gamerule,game_UIs.postGame,true);
            }
        }
        // endregion
        }

        // region Render UI

        struct UI_Manager *curUIManager = Gamerule_GetActiveUIManagerByGameState(&game_UIs,gamerule.gamestates.gamestate);

        if (curUIManager) {
            for (int i = 0; i < curUIManager->count; ++i) {
                Render_UI(world.renderer,&curUIManager->UIs[i]);
            }
        }
        if (gamerule.gamestates.gamestate == GAME_IN_GAME && gamerule.gamestates.gamePaused) {
            for (int i = 0; i < game_UIs.pauseMenu->count; ++i) {
                Render_UI(world.renderer,&game_UIs.pauseMenu->UIs[i]);
            }
        }

        // endregion


        SDL_RenderPresent(world.renderer);
        SDL_Delay(16); // ~60 FPS

    }

        // After game exit destruction

        World_Destroy(&world);

        UI_Manager_Destroy(game_UIs.mainMenu);
        UI_Manager_Destroy(game_UIs.pauseMenu);
        UI_Manager_Destroy(game_UIs.inGame);
        UI_Manager_Destroy(game_UIs.preGame);
        UI_Manager_Destroy(game_UIs.postGame);

        SDL_DestroyRenderer(world.renderer);
        SDL_DestroyWindow(win);

        SDL_Quit();
        IMG_Quit();

        return 0;
}
