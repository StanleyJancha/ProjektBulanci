//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_PLAYER_H
#define PROJEKT_PLAYER_H

#include "object.h"
#include "weapons.h"
#include "keybinds.h"

struct Player {
    struct Object object;
    struct Weapon *primaryWeapon;
    struct Weapon *secondaryWeapon;
    int PlayerKeybindSetIndex;
    int speed;
    int HP;
};

struct Player *Player_CreatePlayer(struct Object *object,struct Weapon *primaryWeapon, struct Weapon *secondaryWeapon, int PlayerKeybindSetIndex, int speed, int HP);
void Player_OnOverlapObject(struct World *world,struct Player * player, struct Object *object);
void Player_PickUpWeapon(struct Player *player, struct Object *weaponObject);
bool Player_MoveBy(struct Player *player, struct Vector2 addVector);
bool Player_Shoot(struct Player *player);
bool Player_SetFacingDirectin(struct Player *player, enum ObjectFacing newDir);
void Player_Print(const struct Player *player);
void Player_Destroy(struct Player *player);
int Player_TakeDamage(struct Player *player, int damage);

#endif //PROJEKT_PLAYER_H