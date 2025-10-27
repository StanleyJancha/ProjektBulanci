#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *win = SDL_CreateWindow("Moving Image",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       800, 600,
                                       0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* surface = IMG_Load("image.png");
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);

    bool running = true;
    SDL_Event e;
    int x = 0;

    while (running) {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                running = false;

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        SDL_Rect dst = {x, 200, 100, 100};
        SDL_RenderCopy(ren, tex, NULL, &dst);
        SDL_RenderPresent(ren);

        x += 2; // move image
        if (x > 800) x = -100; // wrap around
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
