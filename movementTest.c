#include <SDL_image.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

int main() {
    // 1️⃣ Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Move with Arrow Keys",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 2️⃣ Load an image (you can use any BMP file)
    SDL_Surface* surface = IMG_Load("textures/image.png");
    if (!surface) {
        printf("Image Load Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // 3️⃣ Sprite position and speed
    SDL_Rect dest = { 100, 100, 100, 100 }; // x, y, w, h
    int speed = 5;

    bool running = true;
    SDL_Event event;

    // 4️⃣ Main loop
    while (running) {
        // 5️⃣ Handle input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_UP])    dest.y -= speed;
        if (keystate[SDL_SCANCODE_DOWN])  dest.y += speed;
        if (keystate[SDL_SCANCODE_LEFT])  dest.x -= speed;
        if (keystate[SDL_SCANCODE_RIGHT]) dest.x += speed;

        // Keep sprite inside window
        if (dest.x < 0) dest.x = 0;
        if (dest.y < 0) dest.y = 0;
        if (dest.x + dest.w > 800) dest.x = 800 - dest.w;
        if (dest.y + dest.h > 600) dest.y = 600 - dest.h;

        // 6️⃣ Render
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, &dest);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    // 7️⃣ Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}