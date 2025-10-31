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
    int HP;
};
struct Player *Player_CreatePlayer(struct Object *object, int PlayerKeybindSetIndex, int speed, int HP);
void Player_HandleInput(struct Player *player, SDL_Keycode key);
void Player_Print(const struct Player *player);
void Player_Destroy(struct Player *player);
int Player_TakeDamage(struct Player *player, int damage);

#endif //PROJEKT_PLAYER_H