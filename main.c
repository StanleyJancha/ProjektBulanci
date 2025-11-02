#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

#include "src/WorkingWithFiles.h"

#define GAME_LOOP_MILLIS_DELAY 16


void STARTPLAYERS(struct World *world, SDL_Renderer *renderer) {
    //object
    struct Vector2 size2 = {100,100};
    struct Vector2 pos2 = {500,500};
    struct Object *object2 = Object_CreateObject("tucnacek",size2,pos2,0,COLLISION_NONE,OBJECT_PLAYER);
    Animation_AddAnimationsToObject(renderer,object2,ANIMATIONS_OBJECT,0);
    //weapon
    struct Vector2 size3 = {50,50};
    struct Vector2 pos3 = {600,550};
    struct Object *weaponObject = Object_CreateObject("gun",size3,pos3,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON);
    Animation_AddAnimationsToObject(renderer,weaponObject,ANIMATIONS_OBJECT,0);

    struct Weapon *primaryWeapon = Weapon_CreateWeapon(weaponObject,5,3);

    struct Player *player2 = Player_CreatePlayer(object2,primaryWeapon,NULL,1,5,5);

    World_AddPlayer(world,player2);
    free(player2);
    player2 = NULL;

    free(weaponObject);
    weaponObject = NULL;

}

void STARTMAP(struct World *world, SDL_Renderer *renderer) {

    int w,h;
    SDL_GetRendererOutputSize(renderer,&w,&h);


    struct Vector2 size1 = {w,h};
    struct Vector2 pos1 = {0,0};
    struct Object *object1 = Object_CreateObject("pozadi",size1,pos1,0,COLLISION_NONE,OBJECT_STATIC);
    Animation_AddAnimationsToObject(renderer,object1,ANIMATIONS_SINGLE,0);

    World_AddObject(world,object1);
    free(object1);
    object1 = NULL;

    struct Vector2 size = {300,300};
    struct Vector2 pos = {100,100};
    struct Object *object = Object_CreateObject("psik",size,pos,0,COLLISION_BLOCK,OBJECT_STATIC);
    Animation_AddAnimationsToObject(renderer,object,ANIMATIONS_OBJECT,0);

    World_AddObject(world,object);
    free(object);
    object = NULL;

    struct Vector2 size3 = {100,100};
    struct Vector2 pos3 = {650,500};
    struct Object *object3 = Object_CreateObject("shotgun",size3,pos3,0,COLLISION_OVERLAP,OBJECT_PICKUP_WEAPON);
    Animation_AddAnimationsToObject(renderer,object3,ANIMATIONS_OBJECT,0);

    World_AddObject(world,object3);
    free(object3);
    object3 = NULL;

}

void handleInput(struct World *world, const Uint8 *keys) {
    for (int i = world->playerCount - 1; i >= 0; --i) {

        struct Player *player = &world->players[i];
        struct PlayerKeybindSet playerKeybindSet = PlayerKeybindSets[world->players[i].PlayerKeybindSetIndex];
        struct Vector2 addToPos = {0,0};

        if (keys[playerKeybindSet.move_up]) {
            addToPos.y -= player->speed;
            Player_SetFacingDirectin(player,NORTH);
            Object_SetActiveAnimationByName(&player->object,"wave", false);
        }
        if (keys[playerKeybindSet.move_left]) {
            addToPos.x -= player->speed;
            Player_SetFacingDirectin(player,WEST);
            Object_SetActiveAnimationByName(&player->object,"idle", true);
        }
        if (keys[playerKeybindSet.move_down]) {
            addToPos.y += player->speed;
            Player_SetFacingDirectin(player,SOUTH);
            Object_SetActiveAnimationByName(&player->object,"wave", true);
        }
        if (keys[playerKeybindSet.move_right]) {
            addToPos.x += player->speed;
            Player_SetFacingDirectin(player,EAST);
            Object_SetActiveAnimationByName(&player->object,"idle", false);
        }


        if ((addToPos.x != 0 || addToPos.y != 0)) {

            bool canMove = true;

            struct Object playerObjectCopy = player->object;
            Object_MoveBy(&playerObjectCopy, addToPos);

            ///// KOLIZE
            for (int j = 0; j < world->objectCount; ++j) {
                if (Collsions_areColliding(&playerObjectCopy,&world->objects[j])){
                    if (world->objects[j].collision == COLLISION_BLOCK) {
                        canMove = false;
                    }else if (world->objects[j].collision == COLLISION_OVERLAP) {
                        Player_OnOverlapObject(world,player,&world->objects[j]);
                    }
                }
            }


            if (canMove) {
                Player_MoveBy(player,addToPos);
            }

        }

    }
}


int main() {
    // setup
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *win = SDL_CreateWindow("Bulanci",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1080, 720,
                                       0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    Uint32 startTime = SDL_GetTicks();

    struct World world = World_Create();

    STARTMAP(&world,ren);

    STARTPLAYERS(&world,ren);

    World_Print(&world);

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
                    for (int i = 0; i < world.playerCount; ++i) {
                        if (e.key.keysym.scancode == PlayerKeybindSets[world.players[i].PlayerKeybindSetIndex].shoot) {
                            Player_Shoot(&world.players[i]);
                        }

                    }

                    // switch (e.key.keysym.scancode) {
                    //     case SDL_SCANCODE_O: {
                    //         world.players[0].object.activeAnimationIndex = 1;
                    //     }break;
                    //     case SDL_SCANCODE_P: {
                    //         world.players[0].object.activeAnimationIndex = 0;
                    //     }break;
                    //     case SDL_SCANCODE_K: {
                    //         if (world.objectCount > 0) {
                    //             printf("Vymazani objektu %s se %d\n\n",world.objects[0].name,World_RemoveObject(&world, &world.objects[0],true));
                    //         }
                    //         World_Print(&world);
                    //     }break;
                    // }
                }break;
            }
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handleInput(&world, keys);

        /* ----------------- END Main loop ----------------- */

        /////RENDER

        SDL_RenderClear(ren);

        for (int i = 0; i < world.objectCount; ++i) {
            Render_Object(ren, &world.objects[i]);
        }

        for (int i = 0; i < world.playerCount; ++i) {
            Render_Object(ren, &world.players[i].object);

            if (world.players[i].secondaryWeapon != NULL) {
                Render_Object(ren, &world.players[i].secondaryWeapon->object);
            }else

            if (world.players[i].primaryWeapon != NULL) {
                Render_Object(ren, &world.players[i].primaryWeapon->object);
            }

        }

        SDL_RenderPresent(ren);
        SDL_Delay(16); // ~60 FPS

    }

        World_Destroy(&world);

        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);

        SDL_Quit();
        IMG_Quit();

        return 0;
}
