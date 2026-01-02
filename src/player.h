//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_PLAYER_H
#define PROJEKT_PLAYER_H

#include "object.h"
#include "weapons.h"
#include "keybinds.h"
#include "ui.h"


#define PLAYER_STATS_UI_TEXT_SIZE 15
#define PLAYER_STATS_UI_TEXT_COLOR 0,0,0

struct PlayerStats {
    struct UI ui;
    int kills;
    int deaths;
};


struct PlayerDeathStatus {
    bool dead;
    Uint32 lastDeathTime;
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
    struct PlayerDeathStatus deathStatus;
};

struct Player *Player_CreatePlayer(struct Object *object,struct Weapon *primaryWeapon, struct Weapon *secondaryWeapon, int PlayerKeybindSetIndex, int speed, int HP, bool isBot);
void Player_OnOverlapObject(struct World *world,struct Player * player, struct Object *object);
void Player_PickUpWeapon(struct Player *player, struct Object *weaponObject);
bool Player_OnMove(struct World *world,struct Player *player, enum ObjectFacing newDir);
bool Player_MoveBy(struct Player *player, struct Vector2 addVector);
bool Player_Shoot(struct World *world,struct Player *player,struct Gamerule *gamerule);
bool Player_SetFacingDirectin(struct Player *player, enum ObjectFacing newDir);
struct Player *Player_GetByName(struct World *world,char *name);
void Player_Print(const struct Player *player);
void Player_Destroy(struct Player *player);
int Player_TakeDamage(struct Player *player, int damage);
void Player_Respawn(struct World *world,struct Player *player);

void Player_UpdateStatsUITexture(SDL_Renderer *renderer,struct Player *player);
#endif //PROJEKT_PLAYER_H