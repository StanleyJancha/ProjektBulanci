//
// Created by standa on 12/5/25.
//

#include <stdbool.h>

#include "collisions.h"
#include "object.h"
#include "player.h"


void Ai_BotTick(struct World * world, struct Player *bot,struct Gamerule *gamerule) {
    if (bot->deathStatus.deathAnimationPlaying){return;}
    Object_SetActiveAnimationByName(&bot->object,"idle",ANIMATION_NOT_MIRRORED_FLIPPED);

    struct Vector2 nearestPlayerPos = {-2000};
    for (int i = 1; i < world->playerCount; ++i) {
        struct Player player = world->players[i];
        if (strcmp(player.object.name,bot->object.name) == 0){continue;}


    }


    // enum ObjectFacing newRadnomDirection = rand()%4;

    // Player_OnMove(world,bot,newRadnomDirection);
    // Player_Shoot(world,bot,gamerule);

    // kdyz se nepohne



}
