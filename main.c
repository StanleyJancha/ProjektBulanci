#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

#include "src/world.h"
#include "src/object.h"
#include "src/keybinds.h"
#include "src/liveInput.h"
#include "src/player.h"





bool Render_StaticObject(SDL_Renderer *ren, struct Object *object) {

    SDL_Rect dst = {object->position.x, object->position.y, object->size.x, object->size.y};
    SDL_RenderCopy(ren, object->sprites[0].texture, NULL, &dst);
}



int main() {

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
        1
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

    World_AddPlayer(&world, &player);

    World_Print(&world);


    bool running = true;

    SDL_Event e;
    struct LiveInputKeyManager liveInputKeyManager = {
        NULL,
        0
    };

    while (running) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: // ZAVRIT OKNO
                    running = false;
                    break;
                case SDL_KEYDOWN: {
                    if (!LiveInput_KeyInArray(&liveInputKeyManager,e.key.keysym.sym)){LiveInput_AddKey(&liveInputKeyManager,e.type,e.key.keysym.sym);}
                }break;
                case SDL_KEYUP: {
                    if (LiveInput_KeyInArray(&liveInputKeyManager,e.key.keysym.sym)){LiveInput_RemoveKey(&liveInputKeyManager,e.type,e.key.keysym.sym);}
                }break;
            }
        }

        for (int i = 0; i < liveInputKeyManager.activeKeysCount; ++i) {
            int playerIndex = KeyBinds_GetPlayerIndexByInput(&world, liveInputKeyManager.activeKeys[i]);
            if (playerIndex != -1) {
                struct Player *playerWithInput = World_GetPlayerByIndex(&world, playerIndex);
                Player_HandleInput(playerWithInput, liveInputKeyManager.activeKeys[i]);
            }
        }


        SDL_RenderClear(ren);

        Render_StaticObject(ren, &world.players[0].object);

        SDL_RenderPresent(ren);


        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyTexture(object.sprites[0].texture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
