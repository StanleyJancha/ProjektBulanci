//
// Created by standa on 10/28/25.
//

#include "player.h"

struct Player *Player_CreatePlayer(struct Object *object,struct Weapon *primaryWeapon, struct Weapon *secondaryWeapon, int PlayerKeybindSetIndex, int speed, int HP) {
    struct Player *player = malloc(sizeof(struct Player));
    if (!player) return NULL;

    if (!object) {
        free(player);
        return NULL;
    };

    player->object = *object;
    free(object);
    object = NULL;

    player->primaryWeapon = primaryWeapon;
    player->secondaryWeapon = secondaryWeapon;

    player->PlayerKeybindSetIndex = PlayerKeybindSetIndex;
    player->speed = speed;
    player->HP = HP;

    return player;
}

void Player_OnOverlapObject(struct World *world,struct Player * player, struct Object *object) {
    if (object->objectType == OBJECT_PICKUP_WEAPON) {
        if (player->secondaryWeapon == NULL) {
            object->collision = COLLISION_NONE;
            Player_PickUpWeapon(player,object);
            World_RemoveObject(world,object,false);
        }
    }
}

bool Player_Shoot(struct World *world,struct Player *player) {
    struct Vector2 size1 = {50,50};

    struct Object *object1 = Object_CreateObject("bullet",size1,player->primaryWeapon->object.position,0,COLLISION_NONE,OBJECT_DYNAMIC);
    Animation_AddAnimationsToObject(world->renderer,object1,ANIMATIONS_SINGLE,0);

    World_AddObject(world,object1);
    free(object1);
    object1 = NULL;
}

bool Player_SetFacingDirectin(struct Player *player, enum ObjectFacing newDir) {
    player->object.objectDir = newDir;
    struct Vector2 playerCenter = {
        (player->object.position.x + player->object.size.x / 2) ,
        player->object.position.y + player->object.size.y / 2
    };


    if (player->primaryWeapon != NULL) {
        struct Vector2 newWeaponLocation = playerCenter;

        switch (newDir) {
            case NORTH:
                newWeaponLocation.x -= player->primaryWeapon->object.size.x / 2;
                newWeaponLocation.y -= player->primaryWeapon->object.size.y / 2 + player->object.size.y / 2;
                break;

            case EAST:
                newWeaponLocation.x -= player->primaryWeapon->object.size.x / 2 - player->object.size.x / 2;
                newWeaponLocation.y -= player->primaryWeapon->object.size.y / 2;
                break;

            case SOUTH:
                newWeaponLocation.x -= player->primaryWeapon->object.size.x / 2;
                newWeaponLocation.y -= player->primaryWeapon->object.size.y / 2 - player->object.size.y / 2;
                break;

            case WEST:
                newWeaponLocation.x -= player->primaryWeapon->object.size.x / 2 + player->object.size.x / 2;
                newWeaponLocation.y -= player->primaryWeapon->object.size.y / 2;
                break;
        }

        player->primaryWeapon->object.objectDir = newDir;
        player->primaryWeapon->object.position = newWeaponLocation;
    }

    if (player->secondaryWeapon != NULL) {
        struct Vector2 newWeaponLocation = playerCenter;

        switch (newDir) {
            case NORTH:
                newWeaponLocation.x -= player->secondaryWeapon->object.size.x / 2;
                newWeaponLocation.y -= player->secondaryWeapon->object.size.y / 2 + player->object.size.y / 2;
                break;

            case EAST:
                newWeaponLocation.x -= player->secondaryWeapon->object.size.x / 2 - player->object.size.x / 2;
                newWeaponLocation.y -= player->secondaryWeapon->object.size.y / 2;
                break;

            case SOUTH:
                newWeaponLocation.x -= player->secondaryWeapon->object.size.x / 2;
                newWeaponLocation.y -= player->secondaryWeapon->object.size.y / 2 - player->object.size.y / 2;
                break;

            case WEST:
                newWeaponLocation.x -= player->secondaryWeapon->object.size.x / 2 + player->object.size.x / 2;
                newWeaponLocation.y -= player->secondaryWeapon->object.size.y / 2;
                break;
        }

        player->secondaryWeapon->object.objectDir = newDir;
        player->secondaryWeapon->object.position = newWeaponLocation;
    }
}

bool Player_MoveBy(struct Player *player, struct Vector2 addVector) {

    Object_MoveBy(&player->object,addVector);

    // if (player->primaryWeapon != NULL) {
    //     Object_MoveBy(&player->primaryWeapon->object,addVector);
    // }
    // if (player->secondaryWeapon != NULL) {
    //     Object_MoveBy(&player->secondaryWeapon->object,addVector);
    // }

}

void Player_PickUpWeapon(struct Player *player, struct Object *weaponObject) {
    player->secondaryWeapon = Weapon_CreateWeapon(weaponObject,1,1);
    Weapon_SetStatsByName(player->secondaryWeapon);
}


int Player_TakeDamage(struct Player *player, int damage) {

}

void Player_Destroy(struct Player *player) {
    Object_Destroy(&player->object);
    if (player->primaryWeapon != NULL) {
        Object_Destroy(&player->primaryWeapon->object);
        //free(player->primaryWeapon->object);
        free(player->primaryWeapon);
        player->primaryWeapon = NULL;
    }
    if (player->secondaryWeapon != NULL) {
        Object_Destroy(&player->secondaryWeapon->object);
        //free(player->secondaryWeapon->object);
        free(player->secondaryWeapon);
        player->secondaryWeapon = NULL;
    }
}


void Player_Print(const struct Player *player) {
    Object_Print(&player->object);
    printf("\t\tWeapon prim: %d\n", (player->primaryWeapon != NULL)?1:0);
    printf("\t\tWeapon sec: %d\n", (player->secondaryWeapon != NULL)?1:0);
    printf("\t\tSpeed: %d\n", player->speed);
    printf("\t\tPlayerKeybindSetIndex: %d\n", player->PlayerKeybindSetIndex);
    printf("\t\tHP: %d\n", player->HP);
}