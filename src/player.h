//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_PLAYER_H
#define PROJEKT_PLAYER_H

#include "object.h"
#include "weapons.h"
#include "keybinds.h"
#include "ui.h"

struct PlayerStats {
    struct UI ui;
    int kills;
    int deaths;
};

struct Player {
    char displayName[64];
    bool isBot;
    bool canMove;
    struct Object object;
    struct Weapon *primaryWeapon;
    struct Weapon *secondaryWeapon;
    Uint32 lastBulletShotTime;
    int PlayerKeybindSetIndex;
    int speed;
    int HP;
    struct PlayerStats stats;
};

struct Player *Player_CreatePlayer(struct Object *object,struct Weapon *primaryWeapon, struct Weapon *secondaryWeapon, int PlayerKeybindSetIndex, int speed, int HP, bool isBot);
void Player_OnOverlapObject(struct World *world,struct Player * player, struct Object *object);
void Player_PickUpWeapon(struct Player *player, struct Object *weaponObject);
bool Player_MoveBy(struct Player *player, struct Vector2 addVector);
bool Player_Shoot(struct World *world,struct Player *player);
bool Player_SetFacingDirectin(struct Player *player, enum ObjectFacing newDir);
struct Player *Player_GetByName(struct World *world,char *name);
void Player_Print(const struct Player *player);
void Player_Destroy(struct Player *player);
int Player_TakeDamage(struct Player *player, int damage);

void Player_UpdateStatsUITexture(SDL_Renderer *renderer,struct Player *player);
#endif //PROJEKT_PLAYER_H