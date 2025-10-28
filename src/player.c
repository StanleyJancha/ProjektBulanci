//
// Created by standa on 10/28/25.
//

#include "player.h"

void Player_HandleInput(struct Player *player, SDL_Keycode key) {

    if (key == PlayerKeybindSets[player->PlayerKeybindSetIndex].move_up) {
        player->object.position.y -= player->speed;
    }else
    if (key == PlayerKeybindSets[player->PlayerKeybindSetIndex].move_left) {
        player->object.position.x -= player->speed;
    }else
    if (key == PlayerKeybindSets[player->PlayerKeybindSetIndex].move_down) {
        player->object.position.y += player->speed;
    }else
    if (key == PlayerKeybindSets[player->PlayerKeybindSetIndex].move_right) {
        player->object.position.x += player->speed;
    }
}

void Player_Print(const struct Player *player) {
    Object_Print(&player->object);
    printf("Speed: %d\n", player->speed);
    printf("PlayerKeybindSetIndex: %d\n", player->PlayerKeybindSetIndex);
}