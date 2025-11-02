//
// Created by standa on 11/1/25.
//
#include "weapons.h"

struct Weapon *Weapon_CreateWeapon(struct Object *object,int maxAmmo, int damage) {
    struct Weapon *weapon = malloc(sizeof(struct Weapon));
    if (!weapon) return NULL;

    if (!object) {
        free(weapon);
        return NULL;
    };

    weapon->object = *object;
    //free(object);
    //object = NULL;

    weapon->maxAmmo = maxAmmo;
    weapon->currAmmo = maxAmmo;
    weapon->damage = damage;

    return weapon;
}

void Weapon_SetStatsByName(struct Weapon *weapon) {
    if (strcmp(weapon->object.name, "gun") == 0) {
        weapon->maxAmmo = 5;
        weapon->currAmmo = weapon->maxAmmo;
        weapon->damage = 5;
    }
}