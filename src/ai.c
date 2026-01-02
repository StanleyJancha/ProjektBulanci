//
// Created by standa on 12/5/25.
//

#include <stdbool.h>

#include "collisions.h"
#include "object.h"
#include "player.h"


void Ai_BotTick(struct World * world, struct Player *bot,struct Gamerule *gamerule) {

    // enum ObjectFacing newRadnomDirection = rand()%4;

    // Player_OnMove(world,bot,newRadnomDirection);
    // Player_Shoot(world,bot,gamerule);

    // kdyz se nepohne
    Object_SetActiveAnimationByName(&bot->object,"idle",ANIMATION_NOT_MIRRORED_FLIPPED);

}
