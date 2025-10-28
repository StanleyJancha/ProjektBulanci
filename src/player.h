//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_PLAYER_H
#define PROJEKT_PLAYER_H

#include "object.h"
#include "keybinds.h"

struct Player {
    struct Object object;
    int PlayerKeybindSetIndex;
    int speed;
};

void Player_HandleInput(struct Player *player, SDL_Event e);
void Player_Print(const struct Player *player);
#endif //PROJEKT_PLAYER_H