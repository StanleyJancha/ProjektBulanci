#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

#include "src/animace.h"
#include "src/world.h"
#include "src/object.h"
#include "src/keybinds.h"
#include "src/player.h"

#include "src/collisions.h"
#include "src/weapons.h"
#include "src/animace.h"
#include "src/ai.h"
#include "src/render.h"
#include "src/gamerule.h"
#include "src/ui.h"

#include "src/WorkingWithFiles.h"

#define GAME_LOOP_MILLIS_DELAY 16


void handleInput(struct World *world, const Uint8 *keys) {
    for (int i = world->playerCount - 1; i >= 0; --i) {

        if (world->players[i].PlayerKeybindSetIndex == -1) {
            continue;
        }

        struct Player *player = &world->players[i];
        struct PlayerKeybindSet playerKeybindSet = PlayerKeybindSets[world->players[i].PlayerKeybindSetIndex];
        struct Vector2 addToPos = {0,0};

        enum ObjectFacing newDir = -1;
        char *playerAnim = NULL;
        char *weaponAnim = NULL;
        enum AnimationMirrorFlip animFlipPlayer = ANIMATION_NOT_MIRRORED_FLIPPED;
        enum AnimationMirrorFlip animFlipWeapon = ANIMATION_NOT_MIRRORED_FLIPPED;

        if (keys[playerKeybindSet.move_up]) {
            newDir = NORTH;
            addToPos.y = -player->speed;
            playerAnim = "moveUp";
            animFlipPlayer = ANIMATION_NOT_MIRRORED_FLIPPED;
            weaponAnim = "up";
            animFlipWeapon = ANIMATION_NOT_MIRRORED_FLIPPED;
        }
        else if (keys[playerKeybindSet.move_left]) {
            newDir = WEST;
            addToPos.x = -player->speed;
            playerAnim = "moveRight";
            animFlipPlayer = ANIMATION_FLIP;
            weaponAnim = "right";
            animFlipWeapon = ANIMATION_FLIP;
        }
        else if (keys[playerKeybindSet.move_down]) {
            newDir = SOUTH;
            addToPos.y = player->speed;
            playerAnim = "moveDown";
            animFlipPlayer = ANIMATION_NOT_MIRRORED_FLIPPED;
            weaponAnim = "down";
            animFlipWeapon = ANIMATION_NOT_MIRRORED_FLIPPED;
        }
        else if (keys[playerKeybindSet.move_right]) {
            newDir = EAST;
            addToPos.x = player->speed;
            playerAnim = "moveRight";
            animFlipPlayer = ANIMATION_NOT_MIRRORED_FLIPPED;
            weaponAnim = "right";
            animFlipWeapon = ANIMATION_NOT_MIRRORED_FLIPPED;
        }

        if (newDir != -1) {
            Player_SetFacingDirectin(player, newDir);

            Object_SetActiveAnimationByName(&player->object, playerAnim, animFlipPlayer);

            if (player->primaryWeapon)
                Object_SetActiveAnimationByName(&player->primaryWeapon->object, weaponAnim, animFlipWeapon);
            if (player->secondaryWeapon)
                Object_SetActiveAnimationByName(&player->secondaryWeapon->object, weaponAnim, animFlipWeapon);

        }
        player->object.objectAnimationsType = (newDir != -1)?ANIMATIONS_PLAYER:ANIMATIONS_SINGLE;
        if (player->primaryWeapon)
            player->primaryWeapon->object.objectAnimationsType = (newDir != -1)?ANIMATIONS_OBJECT:ANIMATIONS_SINGLE;
        if (player->secondaryWeapon)
            player->secondaryWeapon->object.objectAnimationsType = (newDir != -1)?ANIMATIONS_OBJECT:ANIMATIONS_SINGLE;


        if ((addToPos.x != 0 || addToPos.y != 0)) {

            bool canMove = true;

            struct Object playerObjectCopy = player->object;
            Object_MoveBy(&playerObjectCopy, addToPos);

            ///// KOLIZE
            for (int j = 0; j < world->objectCount; ++j) {
                if (Collsions_areColliding(&playerObjectCopy,&world->objects[j])){
                    if (world->objects[j].collision == COLLISION_BLOCK) {
                        canMove = false;
                    }
                }
            }


            if (canMove) {
                Player_MoveBy(player,addToPos);
            }

        }

    }
}



void loadMainMenuUIs(struct World *world,struct UI_Manager *ui_manager) {
    SDL_Color color = {255,255,255,255};

    struct Vector2 exitGameButtonpos = {500,300};
    struct Vector2 exitGameButtonsize = {200,100};

    struct UI_Events *exitGameButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(exitGameButtonEvents->onClick, "exit_game");

    struct UI *exitGameButton = UI_CreateUI("exit_button",exitGameButtonpos,exitGameButtonsize,"Exit game",exitGameButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,exitGameButton,"basic_button");

    exitGameButton->text.textTexture = UI_GetTextTexture(world->renderer,exitGameButton->text.textToDisplay,color,25);

    UI_Manager_AddUI(ui_manager,exitGameButton);

    free(exitGameButton);


    struct Vector2 startGamepos = {500,100};
    struct Vector2 startGamesize = {200,100};

    struct UI_Events *startGameEvents = malloc(sizeof(struct UI_Events));
    strcpy(startGameEvents->onClick, "start_game");

    struct UI *startGame = UI_CreateUI("start_button",startGamepos,startGamesize,"Start game",startGameEvents,false);
    Animation_AddAnimationToUI(world->renderer,startGame,"basic_button");

    startGame->text.textTexture = UI_GetTextTexture(world->renderer,startGame->text.textToDisplay,color,25);

    UI_Manager_AddUI(ui_manager,startGame);

    free(startGame);
}

void loadPreGameUIs(struct World *world,struct UI_Manager *ui_manager) {
    SDL_Color color = {255,255,255,255};

    struct Vector2 beginMatchButtonPos3 = {500,500};
    struct Vector2 beginMatchButtonSize3 = {200,100};

    struct UI_Events *beginMatchButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(beginMatchButtonEvents->onClick, "begin_match");

    struct UI *beginMatchButton = UI_CreateUI("begin_match_button",beginMatchButtonPos3,beginMatchButtonSize3,"Begin Match",beginMatchButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,beginMatchButton,"basic_button");

    if (strcmp(beginMatchButton->text.textToDisplay, "") != 0) {
        beginMatchButton->text.textTexture = UI_GetTextTexture(world->renderer,beginMatchButton->text.textToDisplay,color,25);
    }

    UI_Manager_AddUI(ui_manager,beginMatchButton);

    free(beginMatchButton);

    struct Vector2 textInputFieldPlayer1Pos3 = {500,150};
    struct Vector2 textInputFieldPlayer1Size3 = {200,100};

    struct UI_Events *textInputFieldPlayer1Events = malloc(sizeof(struct UI_Events));
    strcpy(textInputFieldPlayer1Events->onClick, "text_field");

    struct UI *textInputFieldPlayer1 = UI_CreateUI("player_text_field-1",textInputFieldPlayer1Pos3,textInputFieldPlayer1Size3,"",textInputFieldPlayer1Events,true);
    Animation_AddAnimationToUI(world->renderer,textInputFieldPlayer1,"basic_button");

    textInputFieldPlayer1->text.textTexture = UI_GetTextTexture(world->renderer,textInputFieldPlayer1->text.textToDisplay,color,25);

    UI_Manager_AddUI(ui_manager,textInputFieldPlayer1);

    free(textInputFieldPlayer1);

    /////////////

     struct Vector2 textInputFieldPlayer2Pos3 = {500,300};
     struct Vector2 textInputFieldPlayer2Size3 = {200,100};


     struct UI_Events *textInputFieldPlayer2Events = malloc(sizeof(struct UI_Events));
     strcpy(textInputFieldPlayer2Events->onClick, "text_field");

     struct UI *textInputFieldPlayer2 = UI_CreateUI("player_text_field-2",textInputFieldPlayer2Pos3,textInputFieldPlayer2Size3,"",textInputFieldPlayer2Events,true);
     Animation_AddAnimationToUI(world->renderer,textInputFieldPlayer2,"basic_button");

     textInputFieldPlayer2->text.textTexture = UI_GetTextTexture(world->renderer,textInputFieldPlayer2->text.textToDisplay,color,25);


     UI_Manager_AddUI(ui_manager,textInputFieldPlayer2);

     free(textInputFieldPlayer2);

}



void loadPostGameUIs(struct World *world,struct UI_Manager *ui_manager) {
    SDL_Color color = {255,255,255,255};

    struct Vector2 pos3 = {500,500};
    struct Vector2 size3 = {200,100};

    struct UI_Events *mainMenuButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(mainMenuButtonEvents->onClick, "exit_to_main_menu");

    struct UI *mainMenuButton = UI_CreateUI("main_menu_button",pos3,size3,"Main Menu",mainMenuButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,mainMenuButton,"basic_button");

    if (strcmp(mainMenuButton->text.textToDisplay, "") != 0) {
        mainMenuButton->text.textTexture = UI_GetTextTexture(world->renderer,mainMenuButton->text.textToDisplay,color,25);
    }

    UI_Manager_AddUI(ui_manager,mainMenuButton);

    free(mainMenuButton);

    struct Vector2 pos2 = {0,0};
    struct Vector2 size2 = {200,100};

    struct UI *scoreBoard = UI_CreateUI("score_board",pos2,size2,"winner is",NULL,false);
    Animation_AddAnimationToUI(world->renderer,scoreBoard,NULL);

    scoreBoard->text.textTexture = UI_GetTextTexture(world->renderer,scoreBoard->text.textToDisplay,color,25);

    UI_Manager_AddUI(ui_manager,scoreBoard);

    free(scoreBoard);
}



void loadPauseMenuUIs(struct World *world,struct UI_Manager *ui_manager) {
    SDL_Color color = {255,255,255,255};

    struct Vector2 pos = {500,300};
    struct Vector2 size = {200,100};

    struct UI_Events *pauseButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(pauseButtonEvents->onClick, "unpause_game");

    struct UI *pauseButton = UI_CreateUI("button_unpause",pos,size,"Unpause",pauseButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,pauseButton,"basic_button");

    pauseButton->text.textTexture = UI_GetTextTexture(world->renderer,pauseButton->text.textToDisplay,color,25);

    UI_Manager_AddUI(ui_manager,pauseButton);

    free(pauseButton);

    struct Vector2 pos3 = {500,500};
    struct Vector2 size3 = {200,100};

    struct UI_Events *mainMenuButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(mainMenuButtonEvents->onClick, "exit_to_main_menu");

    struct UI *mainMenuButton = UI_CreateUI("main_menu_button",pos3,size3,"Main Menu",mainMenuButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,mainMenuButton,"basic_button");

    if (strcmp(mainMenuButton->text.textToDisplay, "") != 0) {
        mainMenuButton->text.textTexture = UI_GetTextTexture(world->renderer,mainMenuButton->text.textToDisplay,color,25);
    }

    UI_Manager_AddUI(ui_manager,mainMenuButton);

    free(mainMenuButton);
}
void loadInGameUIs(struct World *world,struct UI_Manager *ui_manager) {

    struct Vector2 pos2 = {0,0};
    struct Vector2 size2 = {200,100};

    struct UI *gameTimerUI = UI_CreateUI("game_timer",pos2,size2,"00:00",NULL,false);
    Animation_AddAnimationToUI(world->renderer,gameTimerUI,NULL);

    SDL_Color color = {255,255,255,255};

    gameTimerUI->text.textTexture = UI_GetTextTexture(world->renderer,gameTimerUI->text.textToDisplay,color,25);

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

    struct Gamerule gamerule = {{0,1,GAME_IN_MAIN_MENU},{0,0,0}};

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

    // char PlayerNames[4][64] = {"fwef","fwefw","",""};
    // Gamerule_StartGame(&world,&gamerule,PlayerNames);
    // World_Print(&world);
    // gamerule.gamestates.gamestate = GAME_IN_GAME;
    SDL_Event e;
    SDL_StopTextInput();

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

                    if (strlen(gamerule.inputUI->text.textToDisplay) + inputLength  < sizeof(gamerule.inputUI->text.textToDisplay) - 1) {
                        strcat(gamerule.inputUI->text.textToDisplay, e.text.text);
                        SDL_Color color = {255,255,255,255};
                        gamerule.inputUI->text.textTexture = UI_GetTextTexture(world.renderer,gamerule.inputUI->text.textToDisplay,color,32);
                    }


                }break;

                case SDL_KEYDOWN: {
                    if (!gamerule.gamestates.gamePaused && gamerule.gamestates.gamestate == GAME_IN_GAME) {
                        for (int i = 0; i < world.playerCount; ++i) {
                            if (world.players[i].isBot == false){
                            if (e.key.keysym.scancode == PlayerKeybindSets[world.players[i].PlayerKeybindSetIndex].shoot) {
                                Player_Shoot(&world,&world.players[i]);
                            }
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
                                SDL_Color color = {255,255,255,255};
                                gamerule.inputUI->text.textTexture = UI_GetTextTexture(world.renderer,gamerule.inputUI->text.textToDisplay,color,32);
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
                if (world.players[i].isBot) {
                    Ai_BotTick(&world,&world.players[i]);
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
                    for (int j = 0; j < world.playerCount; ++j) {
                        if (Collsions_areColliding(&world.objects[i],&world.players[j].object)) {
                            char name[32];
                            strcpy(name,world.objects[i].name);
                            char *token = strtok(name,"_");
                            if (strcmp(token,"bullet") == 0) {
                                token = strtok(NULL,"_");
                                if (token == NULL){continue;}
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
        if (!gamerule.gamestates.gamePaused) {
            int bulletDestroyTimeMs = 2000;
            // mazani objektu kulek po case
            int objectsToDestroy[5] = {-1,-1,-1,-1,-1};
            int j = 0;
            for (int i = 0; i < world.objectCount; ++i) {
                char nameCopy[32];
                strcpy(nameCopy,world.objects[i].name);
                if (strcmp(strtok(nameCopy,"_"),"bullet") == 0) {
                    if ((SDL_GetTicks() - gamerule.gamestates.gamePaused) - world.objects[i].spawnTime > bulletDestroyTimeMs) {
                        objectsToDestroy[j] = i;
                        j++;
                    }
                }
            }
            for (int i = 0; i < 5; ++i) {
                if (objectsToDestroy[i] != -1) {
                    World_RemoveObject(&world,&world.objects[objectsToDestroy[i]],false);
                }
            }
        }
        // regionend

        // region Setting game time for UI
        if (!gamerule.gamestates.gamePaused) {
            struct UI *clockUI = UI_Manager_GetUIByIdentifier(game_UIs.inGame,"game_timer");
            if (clockUI) {
                Uint32 curTime = SDL_GetTicks();
                char timerText[6];
                int seconds = (curTime-gamerule.gameTimes.startTime-gamerule.gameTimes.timePaused)/1000;
                sprintf(timerText,"%02d:%02d",
                    (seconds/60%100),
                    (seconds%60)%100);
                if (strcmp(timerText,clockUI->text.textToDisplay) != 0) {
                    SDL_Color color = {255,255,255};
                    strcpy(clockUI->text.textToDisplay,timerText);
                    clockUI->text.textTexture = UI_GetTextTexture(world.renderer,timerText,color,32);
                }
            }else{printf("Nelze najit UI pro TIMER ve hre\n");}
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
