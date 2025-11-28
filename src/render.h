//
// Created by standa on 10/30/25.
//

#ifndef PROJEKT_RENDER_H
#define PROJEKT_RENDER_H
#include <SDL2/SDL.h>

#include <stdbool.h>
#include "object.h"
#endif //PROJEKT_RENDER_H

bool Render_Object(SDL_Renderer *ren, struct Object *object);
bool Render_UI(SDL_Renderer *ren,struct UI *ui);