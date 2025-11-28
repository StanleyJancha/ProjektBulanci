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
#include "src/render.h"
#include "src/gamerule.h"
#include "src/ui.h"

#include "src/WorkingWithFiles.h"

#define GAME_LOOP_MILLIS_DELAY 16


void STARTPLAYERS(struct World *world) {
    //object
    struct Vector2 size2 = {100,100};
    struct Vector2 pos2 = {500,500};
    struct Object *object2 = Object_CreateObject("player1",size2,pos2,0,COLLISION_NONE,OBJECT_PLAYER,WEST);
    Animation_AddAnimationsToObject(world->renderer,object2,ANIMATIONS_OBJECT,0);
    //weapon
    struct Vector2 size3 = {50,50};
    struct Vector2 pos3 = {500,500};
    struct Object *weaponObject = Object_CreateObject("gun",size3,pos3,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
    Animation_AddAnimationsToObject(world->renderer,weaponObject,ANIMATIONS_OBJECT,0);

    struct Weapon *primaryWeapon = Weapon_CreateWeapon(weaponObject,5,3);

    struct Player *player2 = Player_CreatePlayer(object2,primaryWeapon,NULL,1,5,5);

    World_AddPlayer(world,player2);
    free(player2);
    player2 = NULL;

    free(weaponObject);
    weaponObject = NULL;

    //object
    struct Vector2 size6 = {100,100};
    struct Vector2 pos6 = {600,300};
    struct Object *object6 = Object_CreateObject("player2",size6,pos6,0,COLLISION_NONE,OBJECT_PLAYER,WEST);
    Animation_AddAnimationsToObject(world->renderer,object6,ANIMATIONS_OBJECT,0);
    //weapon
    struct Vector2 size9 = {50,50};
    struct Vector2 pos9 = {500,500};
    struct Object *weaponObject9 = Object_CreateObject("gun",size9,pos9,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON,WEST);
    Animation_AddAnimationsToObject(world->renderer,weaponObject9,ANIMATIONS_OBJECT,0);

    struct Weapon *primaryWeapon9 = Weapon_CreateWeapon(weaponObject9,5,3);

    struct Player *player1 = Player_CreatePlayer(object6,primaryWeapon9,NULL,0,5,5);

    World_AddPlayer(world,player1);
    free(player1);
    player1 = NULL;

    free(weaponObject9);
    weaponObject9 = NULL;

}

void STARTMAP(struct World *world) {

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
    struct Vector2 pos = {100,100};
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

void handleInput(struct World *world, const Uint8 *keys) {
    for (int i = world->playerCount - 1; i >= 0; --i) {

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

}
void loadPauseMenuUIs(struct World *world,struct UI_Manager *ui_manager) {
    struct Vector2 pos = {500,300};
    struct Vector2 size = {200,100};

    struct UI *buttonUI = UI_CreateButtonUI("button_pause",pos,size,"unpause_game");
    Animation_AddAnimationToUI(world->renderer,buttonUI);

    struct Vector2 pos2 = {500,300};
    struct Vector2 size2 = {200,100};

    struct UI *textUI = UI_CreateTextUI("button_pause_text",pos2,size2,"Continue");
    Animation_AddAnimationToUI(world->renderer,textUI);

    SDL_Color color = {255,255,255,255};

    textUI->content.text.textTexture = UI_GetTextTexture(world->renderer,textUI->content.text.textToDisplay,color,25);

    UI_SetChild(buttonUI,textUI);

    UI_Manager_AddUI(ui_manager,buttonUI);

    free(buttonUI);

    struct Vector2 pos3 = {500,500};
    struct Vector2 size3 = {200,100};

    struct UI *buttonUI2 = UI_CreateButtonUI("main_menu_button",pos3,size3,"goToMainMenu");
    Animation_AddAnimationToUI(world->renderer,buttonUI2);

    struct Vector2 pos4 = {500,500};
    struct Vector2 size4 = {200,100};

    struct UI *textUI2 = UI_CreateTextUI("button_pause_text",pos4,size4,"Continue");
    Animation_AddAnimationToUI(world->renderer,textUI2);

    SDL_Color color2 = {255,255,255,255};

    textUI2->content.text.textTexture = UI_GetTextTexture(world->renderer,textUI2->content.text.textToDisplay,color2,25);

    UI_SetChild(buttonUI2,textUI2);

    UI_Manager_AddUI(ui_manager,buttonUI2);

    free(buttonUI2);
}
void loadInGameUIs(struct World *world,struct UI_Manager *ui_manager) {

    struct Vector2 pos2 = {0,0};
    struct Vector2 size2 = {200,100};

    struct UI *textUI = UI_CreateTextUI("game_timer",pos2,size2,"00:00");
    Animation_AddAnimationToUI(world->renderer,textUI);

    SDL_Color color = {255,255,255,255};

    textUI->content.text.textTexture = UI_GetTextTexture(world->renderer,textUI->content.text.textToDisplay,color,25);

    UI_Manager_AddUI(ui_manager,textUI);

    free(textUI);

}

void startGame() {

};

int main() {
    // setup
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *win = SDL_CreateWindow("Bulanci",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       1080, 720,
                                       0
                                       );

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 1;
    }

    struct Gamerule gamerule = {false,{0,0,0}};

    gamerule.gameTimes.startTime = SDL_GetTicks();

    struct World world = World_Create();
    world.renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // ui
    struct UI_Manager ui_managerPauseMenu = UI_Manager_Create();
    struct UI_Manager ui_managerInGame = UI_Manager_Create();

    loadInGameUIs(&world,&ui_managerInGame);
    loadPauseMenuUIs(&world,&ui_managerPauseMenu);

    UI_Manager_PrintAllUIs(&ui_managerInGame);

    // spawnuti objektu a hracu
    STARTMAP(&world);

    STARTPLAYERS(&world);

    //World_Print(&world);

    /* ----------------- Main loop ----------------- */

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:{ // ZAVRIT OKNO
                    running = false;
                    break;
                }
                case SDL_KEYDOWN: {
                    if (!gamerule.gamePaused) {
                        for (int i = 0; i < world.playerCount; ++i) {
                            if (e.key.keysym.scancode == PlayerKeybindSets[world.players[i].PlayerKeybindSetIndex].shoot) {
                                Player_Shoot(&world,&world.players[i]);
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
                            (gamerule.gamePaused)?Gamerule_UnpauseGame(&gamerule):Gamerule_PauseGame(&gamerule);
                            break;
                        }
                    }
                }break;
                case SDL_MOUSEBUTTONDOWN: {
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        struct Vector2 mousePos = {e.button.x,e.button.y};
                        struct UI *clickedOnUI = UI_MouseOnUI(ui_managerPauseMenu,mousePos); // nekontroluje zatim child UI, pouze prvni vrstvu
                        if (clickedOnUI != NULL) {
                            printf("Clicknul na '%s' UI\n",clickedOnUI->identifier);
                            UI_ButtonCallEvent(&world,&gamerule,clickedOnUI->content.button.functionName);
                        }
                    }

                }break;
            }
        }

        if (!gamerule.gamePaused) {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            handleInput(&world, keys);


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
                                if (strcmp(token,world.players[j].object.name) != 0) {
                                    Player_TakeDamage(&world.players[j],1);
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
        }

        /* ----------------- END Main loop ----------------- */

        /////RENDER

        SDL_RenderClear(world.renderer);

        for (int i = 0; i < world.objectCount; ++i) {
            if (world.objects[i].objectType == OBJECT_STATIC) {
                Render_Object(world.renderer, &world.objects[i]);
            }
        }

        for (int i = 0; i < world.objectCount; ++i) {
            if (world.objects[i].objectType == OBJECT_PICKUP_WEAPON) {
                Render_Object(world.renderer, &world.objects[i]);
            }
        }

        for (int i = 0; i < world.playerCount; ++i) {
            Render_Object(world.renderer, &world.players[i].object);

            if (world.players[i].secondaryWeapon != NULL) {
                Render_Object(world.renderer, &world.players[i].secondaryWeapon->object);
            }else

            if (world.players[i].primaryWeapon != NULL) {
                Render_Object(world.renderer, &world.players[i].primaryWeapon->object);
            }
        }

        for (int i = 0; i < world.objectCount; ++i) {
            if (world.objects[i].objectType == OBJECT_DYNAMIC) {
                Render_Object(world.renderer, &world.objects[i]);
            }
        }

        if (!gamerule.gamePaused) {
            int bulletDestroyTimeMs = 2000;
            // mazani objektu kulek po case
            int objectsToDestroy[5] = {-1,-1,-1,-1,-1};
            int j = 0;
            for (int i = 0; i < world.objectCount; ++i) {
                char nameCopy[32];
                strcpy(nameCopy,world.objects[i].name);
                if (strcmp(strtok(nameCopy,"_"),"bullet") == 0) {
                    if ((SDL_GetTicks() - gamerule.gamePaused) - world.objects[i].spawnTime > bulletDestroyTimeMs) {
                        objectsToDestroy[j] = i;
                        j++;
                    }
                }
                // free(tmp);
            }
            for (int i = 0; i < 5; ++i) {
                if (objectsToDestroy[i] != -1) {
                    World_RemoveObject(&world,&world.objects[objectsToDestroy[i]],false);
                }
            }
        }

        // NASTAVENI CASU V TIMERU
        if (!gamerule.gamePaused) {
            struct UI *clockUI = UI_Manager_GetUIByIdentifier(&ui_managerInGame,"game_timer");
            if (clockUI) {
                Uint32 curTime = SDL_GetTicks();
                char timerText[6];
                int seconds = (curTime-gamerule.gameTimes.startTime-gamerule.gameTimes.timePaused)/1000;
                sprintf(timerText,"%02d:%02d",
                    (seconds/60%100),
                    (seconds%60)%100);
                if (strcmp(timerText,clockUI->content.text.textToDisplay) != 0) {
                    SDL_Color color = {255,255,255};
                    strcpy(clockUI->content.text.textToDisplay,timerText);
                    clockUI->content.text.textTexture = UI_GetTextTexture(world.renderer,timerText,color,32);
                }
            }else{printf("Nelze najit UI pro TIMER ve hre");}
        }


        //render UI

        for (int i = 0; i < ui_managerInGame.count; ++i) {
            Render_UI(world.renderer,&ui_managerInGame.UIs[i]);
        }

        if (gamerule.gamePaused) {
            for (int i = 0; i < ui_managerPauseMenu.count; ++i) {
                Render_UI(world.renderer,&ui_managerPauseMenu.UIs[i]);
            }
        }



        SDL_RenderPresent(world.renderer);
        SDL_Delay(16); // ~60 FPS

    }

        World_Destroy(&world);

        UI_Manager_Destroy(&ui_managerPauseMenu);
        UI_Manager_Destroy(&ui_managerInGame);

        SDL_DestroyRenderer(world.renderer);
        SDL_DestroyWindow(win);

        SDL_Quit();
        IMG_Quit();

        return 0;
}
