#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

#include "src/animace.h"
#include "src/world.h"
#include "src/object.h"
#include "src/keybinds.h"
#include "src/player.h"

#include "src/collisions.h"

#include "src/WorkingWithFiles.h"
struct Object *Object_CreateObject(const char *name, struct Vector2 size, struct Vector2 position, char *spritePaths[], int spriteCount, int zLayer, enum Collisions collions) {
    struct Object *object = malloc(sizeof(struct Object));
    if (!object) return NULL;

    strcpy(object->name,name);
    object->size = size;
    object->position = position;
    object->sprites = malloc(sizeof(struct Sprite) * spriteCount);
    if (!object->sprites) {
        free(object);
        return NULL;
    }
    object->spriteCount = spriteCount;
    for (int i = 0; i < spriteCount; ++i) {
        strcpy(object->sprites[i].spritePath, spritePaths[i]);
    }

    object->zLayer = zLayer;
    object->collision = collions;

    return object;
}

struct Player *Player_CreatePlayer(struct Object *object, int PlayerKeybindSetIndex, int speed, int HP) {
    struct Player *player = malloc(sizeof(struct Player));
    if (!player) return NULL;
    if (!object) {
        free(player);
        return NULL;
    };

    player->object = *object;
    free(object);
    object = NULL;

    player->PlayerKeybindSetIndex = PlayerKeybindSetIndex;
    player->speed = speed;
    player->HP = HP;

    return player;
}

void STARTMAP(struct World *world, SDL_Renderer *renderer) {

    int w,h;
    SDL_GetRendererOutputSize(renderer,&w,&h);

    struct Vector2 size2 = {w,h};
    struct Vector2 pos2 = {0,0};
    char *texturePaths2[] = {"textures/background.jpeg"};
    struct Object *object2 = Object_CreateObject("Background",size2,pos2,texturePaths2, 1,0,COLLISION_NONE);
    Object_SetTextures(renderer, object2);

    World_AddObject(world,object2);
    free(object2);
    object2 = NULL;

    struct Vector2 size = {100,100};
    struct Vector2 pos = {300,300};
    char *texturePaths[] = {"textures/tree.png"};
    struct Object *object = Object_CreateObject("Tree",size,pos,texturePaths, 1,1,COLLISION_BLOCK);
    Object_SetTextures(renderer, object);

    World_AddObject(world,object);
    free(object);
    object = NULL;

}



void _temp_spawnPlayers(struct World *world, SDL_Renderer *ren) {
    /* ----------------- Player1 ----------------- */

    struct Object object = {
        "Postavicka1",
        {50,50},
        {50,50},
        NULL,
        1,
        1,
        COLLISION_BLOCK,
    };
    object.sprites = (struct Sprite*)malloc(sizeof(struct Sprite) * object.spriteCount);
    strcpy(object.sprites[0].spritePath , "textures/image.png");

    struct Player player = {
        object,
        1,
        1,
        5
    };
    Object_SetTextures(ren, &object);

    /* ----------------- End Player1 ----------------- */

    /* ----------------- Player2 ----------------- */

    struct Object object2 = {
        "Postavicka2",
        {50,50},
        {100,100},
        NULL,
        1,
        1,
        COLLISION_BLOCK,
    };
    object2.sprites = (struct Sprite*)malloc(sizeof(struct Sprite) * object2.spriteCount);
    strcpy(object2.sprites[0].spritePath , "textures/image.png");

    struct Player player2 = {
        object2,
        0,
        1,
        5
    };
    Object_SetTextures(ren, &object2);

    /* ----------------- End Player2 ----------------- */


    World_AddPlayer(world, &player);
    World_AddPlayer(world, &player2);


}



bool Render_StaticObject(SDL_Renderer *ren, struct Object *object) {

    SDL_Rect dst = {object->position.x, object->position.y, object->size.x, object->size.y};
    SDL_RenderCopy(ren, object->sprites[0].texture, NULL, &dst);
}

void handleInput(struct World *world, const Uint8 *keys) {
    for (int i = world->playerCount - 1; i >= 0; --i) {

        struct Player *player = &world->players[i];
        struct PlayerKeybindSet playerKeybindSet = PlayerKeybindSets[world->players[i].PlayerKeybindSetIndex];
        struct Vector2 addToPos = {0,0};

        if (keys[playerKeybindSet.move_up]) {addToPos.y -= player->speed;}
        if (keys[playerKeybindSet.move_left]) {addToPos.x -= player->speed;}
        if (keys[playerKeybindSet.move_down]) {addToPos.y += player->speed;}
        if (keys[playerKeybindSet.move_right]) {addToPos.x += player->speed;}

        if ((addToPos.x != 0 || addToPos.y != 0)) {

            bool canMove = true;

            struct Object playerObjectCopy = player->object;
            Object_MoveBy(&playerObjectCopy, addToPos);

            struct Object *obj2 = &world->objects[1];

            if (Collsions_areColliding(&playerObjectCopy,obj2)){
                if (obj2->collision == COLLISION_BLOCK) {
                    canMove = false;
                }else if (obj2->collision == COLLISION_OVERLAP) {

                }
            }

            if (canMove) {
                Object_MoveBy(&player->object,addToPos);
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


    struct World world = World_Create();
    STARTMAP(&world, ren);

    struct Vector2 size = {100,100};
    struct Vector2 pos = {50,50};
    char *texturePaths[] = {"textures/image.png"};

    struct Player *player = Player_CreatePlayer(
        Object_CreateObject(
            "Player1",
            size,
            pos,
            texturePaths,
            1,
            2,
            COLLISION_OVERLAP
            ),
            0,
            5,
            5
        );
    Object_SetTextures(ren, &player->object);

    World_AddPlayer(&world,player);
    free(player);
    player = NULL;

    struct Vector2 size2 = {100,100};
    struct Vector2 pos2 = {300,300};
    char *texturePaths2[] = {"textures/image.png"};

    struct Player *player2 = Player_CreatePlayer(
        Object_CreateObject(
            "Player2",
            size2,
            pos2,
            texturePaths2,
            1,
            2,
            COLLISION_OVERLAP
            ),
            1,
            5,
            5
        );
    Object_SetTextures(ren, &player2->object);

    World_AddPlayer(&world,player2);
    free(player2);
    player2 = NULL;

    struct Vector2 size3 = {100,100};
    struct Vector2 pos3 = {200,200};
    char *texturePaths3[] = {"textures/image.png"};

    struct Player *player3 = Player_CreatePlayer(
        Object_CreateObject(
            "Player3",
            size3,
            pos3,
            texturePaths3,
            1,
            2,
            COLLISION_OVERLAP
            ),
            1,
            5,
            5
        );
    Object_SetTextures(ren, &player3->object);

    World_AddPlayer(&world,player3);
    free(player3);
    player3 = NULL;

    //_temp_spawnPlayers(&world,ren);

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
                        switch (e.key.keysym.scancode) {
                            case SDL_SCANCODE_O: {
                                World_RemovePlayer(&world, &world.players[1]);
                                World_Print(&world);
                            }break;
                            case SDL_SCANCODE_K: {
                                printf("Vymazani objektu se %d\n\n",World_RemoveObject(&world, &world.objects[1]));
                                World_Print(&world);
                            }break;
                        }
                }break;
            }
        }
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handleInput(&world, keys);

        /* ----------------- END Main loop ----------------- */

        /////RENDER

        if (world.playerCount != 3) {
            printf("");
        }

        SDL_RenderClear(ren);

        for (int i = 0; i < world.objectCount; ++i) {
            Render_StaticObject(ren, &world.objects[i]);
        }


        for (int i = 0; i < world.playerCount; ++i) {
            Render_StaticObject(ren, &world.players[i].object);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16); // ~60 FPS

    }


    for (int i = 0; i < world.objectCount; ++i) {
        for (int j = 0; j < world.objects[i].spriteCount; ++j) {
            SDL_DestroyTexture(world.objects[i].sprites[j].texture);
        }
    }

    for (int i = 0; i < world.playerCount; ++i) {
        for (int j = 0; j < world.players[i].object.spriteCount; ++j) {
            SDL_DestroyTexture(world.players[i].object.sprites[j].texture);
        }
    }

    World_Destroy(&world);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
