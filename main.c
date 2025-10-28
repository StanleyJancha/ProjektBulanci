#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

#include "src/world.h"
#include "src/object.h"
#include "src/keybinds.h"
#include "src/player.h"

#include "src/WorkingWithFiles.h"

void STARTMAP(struct World *world, SDL_Renderer *renderer) {

    int w,h;

    SDL_GetRendererOutputSize(renderer,&w,&h);

    struct Object background = {
        "background",
        {w,h},
        {0,0},
        NULL,
        1,
        0,
        COLLISION_BLOCK
    };
    background.sprites = (struct Sprite*)malloc(sizeof(struct Sprite) * background.spriteCount);
    strcpy(background.sprites[0].spritePath , "textures/background.jpeg");
    Object_SetTexture(renderer, &background);
    World_AddObject(world, &background);

    struct Object tree = {
        "tree",
        {50,50},
        {200,200},
        NULL,
        1
    };
    tree.sprites = (struct Sprite*)malloc(sizeof(struct Sprite) * tree.spriteCount);
    strcpy(tree.sprites[0].spritePath , "textures/tree.png");
    Object_SetTexture(renderer, &tree);

    World_AddObject(world, &tree);

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
            Object_MoveBy(&player->object,addToPos);
        }

    }
}



int main() {
    // struct Object *objekty = NULL;
    // int *length = 0;
    //
    // GetMapObjects(objekty,length);

    // setup
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *win = SDL_CreateWindow("Bulanci",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       800, 600,
                                       0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

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
        2
    };
    Object_SetTexture(ren, &object);


    struct World world = World_Create();
    STARTMAP(&world, ren);

    World_AddPlayer(&world, &player);

    World_Print(&world);


    bool running = true;

    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: // ZAVRIT OKNO
                    running = false;
                    break;
            }
        }
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handleInput(&world, keys);



        SDL_RenderClear(ren);

        for (int i = 0; i < world.objectCount; ++i) {
            Render_StaticObject(ren, &world.objects[i]);
        }


        for (int i = 0; i < world.playerCount; ++i) {
            Render_StaticObject(ren, &world.players[0].object);
        }


        SDL_RenderPresent(ren);


        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyTexture(object.sprites[0].texture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
