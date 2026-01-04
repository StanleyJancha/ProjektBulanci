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

    // Nacteni UIs
    Game_UIs_Load(&world,&game_UIs);

    //TEST ROVNOU DO HRY
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

                    if (gamerule.inputUI && gamerule.gamestates.gamestate == GAME_PRE_PLAY){
                        if (strlen(gamerule.inputUI->text.textToDisplay) + inputLength  < sizeof(gamerule.inputUI->text.textToDisplay) - 1 && strlen(gamerule.inputUI->text.textToDisplay) < 11) {
                            strcat(gamerule.inputUI->text.textToDisplay, e.text.text);
                            gamerule.inputUI->text.textTexture = UI_GetTextTexture(world.renderer,gamerule.inputUI->text);
                        }
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
                case SDL_MOUSEBUTTONDOWN: { // KLIKANI NA UI PRVKY
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
                            UI_ButtonCallEvent(&world,&gamerule,&game_UIs,curManager,clickedOnUI);
                        }
                    }

                }break;
            }
        }
        // endregion

        if (!gamerule.gamestates.gamePaused) {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            Gamerule_HandleInput(&world, keys);

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
            Render_UI(world.renderer,&world.players[i].playerStatsUI);
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
            Gamerule_SpawnWeaponLogic(&world);


        }

        // regionend
        if (!gamerule.gamestates.gamePaused) {
            Uint32 curTime = SDL_GetTicks();

            int seconds = (curTime-gamerule.gameTimes.startTime-gamerule.gameTimes.timePaused)/1000;

            Gamerule_UpdateTimer(&world,&gamerule,&game_UIs,seconds);

            // if (seconds > 30) { // jestli vyprsi cas hry
            //     Gamerule_EndGame(&world,&gamerule,&game_UIs,true);
            // }

            if (seconds/60 > gamerule.gameTimes.gameLengthMinutes) { // jestli vyprsi cas hry
                Gamerule_EndGame(&world,&gamerule,&game_UIs,true);
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
