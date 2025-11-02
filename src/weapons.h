//
// Created by standa on 11/1/25.
//

#ifndef PROJEKT_WEAPONS_H
#define PROJEKT_WEAPONS_H

#include "object.h"

struct Weapon {
    struct Object object;
    int damage;
    int maxAmmo;
    int currAmmo;
};
struct Weapon *Weapon_CreateWeapon(struct Object *object,int maxAmmo, int damage);

void Weapon_SetStatsByName(struct Weapon *weapon);

#endif //PROJEKT_WEAPONS_H