//
// Created by standa on 12/5/25.
//

#include <stdbool.h>

#include "collisions.h"
#include "object.h"
#include "player.h"


void Ai_BotTick(struct World * world, struct Player *bot,struct Gamerule *gamerule) {
    if (bot->deathStatus.dead){return;}
    Object_SetActiveAnimationByName(&bot->object,"idle",ANIMATION_NOT_MIRRORED_FLIPPED);

    struct Player *target = NULL;
    int minOffset = 99999;
    bool alignOnX = false;

    for (int i = 0; i < world->playerCount; i++) {
        struct Player *p = &world->players[i];
        if (p == bot || p->deathStatus.dead) continue;

        int diffX = fabs(p->object.position.x - bot->object.position.x);
        int diffY = fabs(p->object.position.y - bot->object.position.y);

        if (diffX < minOffset) {
            minOffset = diffX;
            target = p;
            alignOnX = true;
        }
        if (diffY < minOffset) {
            minOffset = diffY;
            target = p;
            alignOnX = false;
        }

        if (!target) return;

        int threshold = 15;

        if (alignOnX) {
            if (minOffset > threshold) {
                int moveDirX = target->object.position.x - bot->object.position.x;
                Player_OnMove(world, bot, (moveDirX > 0) ? EAST : WEST);
            } else {
                int moveDirY = target->object.position.y - bot->object.position.y;
                Player_SetFacingDirectin(bot, (moveDirY > 0) ? SOUTH : NORTH);
                Player_Shoot(world, bot, gamerule);
            }
        } else {
            if (minOffset > threshold) {
                int moveDirY = target->object.position.y - bot->object.position.y;
                Player_OnMove(world, bot, (moveDirY > 0) ? SOUTH : NORTH);
            } else {
                int moveDirX = target->object.position.x - bot->object.position.x;
                Player_SetFacingDirectin(bot, (moveDirX > 0) ? EAST : WEST);
                Player_Shoot(world, bot, gamerule);
            }
        }
    }


    // enum ObjectFacing newRadnomDirection = rand()%4;

    // Player_OnMove(world,bot,newRadnomDirection);
    // Player_Shoot(world,bot,gamerule);

    // kdyz se nepohne



}
