//
// Created by standa on 12/5/25.
//

#include <stdbool.h>

#include "collisions.h"
#include "object.h"
#include "player.h"


void Ai_BotTick(struct World * world, struct Player *bot) {

    struct Vector2 addToPos = {2,2};


    bool canMove = true;

    struct Object playerObjectCopy = bot->object;
    Object_MoveBy(&playerObjectCopy, addToPos);

    ///// KOLIZE
    for (int j = 0; j < world->objectCount; ++j) {
        if (Collsions_areColliding(&playerObjectCopy,&world->objects[j])){
            if (world->objects[j].collision == COLLISION_BLOCK) {
                canMove = false;
            }
        }
    }


    if (canMove) {
        Player_MoveBy(bot,addToPos);
    }


}
