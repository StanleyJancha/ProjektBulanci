//
// Created by standa on 10/28/25.
//

#include "player.h"

#include "collisions.h"

struct Player *Player_CreatePlayer(struct Object *object,struct Weapon *primaryWeapon, struct Weapon *secondaryWeapon, int PlayerKeybindSetIndex, int speed, int HP, bool isBot) {
    struct Player *player = malloc(sizeof(struct Player));
    if (!player) return NULL;

    if (!object) {
        free(player);
        return NULL;
    };

    player->object = *object;
    free(object);
    object = NULL;

    player->isBot = isBot;
    player->canMove = true;

    player->primaryWeapon = primaryWeapon;
    player->secondaryWeapon = secondaryWeapon;

    player->PlayerKeybindSetIndex = PlayerKeybindSetIndex;
    player->speed = speed;
    player->HP = HP;
    player->lastBulletShotTime = 0;

    player->stats.kills = 0;
    player->stats.deaths = 0;

    player->deathStatus.dead = false;
    player->deathStatus.lastDeathTime = 0;

    return player;
}

void Player_OnOverlapObject(struct World *world,struct Player * player, struct Object *object) {
    printf("OVERLAP DYNAMIC %s %s\n",player->object.name,object->name);
    if (object->objectType == OBJECT_PICKUP_WEAPON) {

    }
    else if (object->objectType == OBJECT_DYNAMIC) {
        char name[32];
        strcpy(name,object->name);
        char *token = strtok(name,"_");
        if (strcmp(token,"bullet") == 0) {
            token = strtok(NULL,"_");
            if (token == NULL){return;}
            if (strcmp(token,player->object.name) != 0) {
                printf("shoot player");
            }
        }
    }
}

bool Player_Shoot(struct World *world,struct Player *player,struct Gamerule *gamerule) {
    int weaponCooldown = (player->secondaryWeapon == NULL)?500:200;

    if (SDL_GetTicks() - gamerule->gamestates.gamePaused - player->lastBulletShotTime < weaponCooldown) {
        return false;
    }
    player->lastBulletShotTime = SDL_GetTicks();

    struct Vector2 size1 = {50,50};

    char bulletName[32];
    char playerName[32];
    strcpy(bulletName,"bullet_");
    strcpy(playerName,player->object.name);
    strcat(bulletName,playerName);

    struct Object *object1 = Object_CreateObject(bulletName,size1,player->primaryWeapon->object.position,0,COLLISION_OVERLAP,OBJECT_DYNAMIC,player->object.objectDir);
    Animation_AddAnimationsToObject(world->renderer,object1,ANIMATIONS_SINGLE,0);

    World_AddObject(world,object1);
    free(object1);
    object1 = NULL;



    if (player->secondaryWeapon != NULL) {
        player->secondaryWeapon->currAmmo -= 1;
        if (player->secondaryWeapon->currAmmo <= 0) {
            free(player->secondaryWeapon);
            player->secondaryWeapon = NULL;
        }
    }
}

bool Player_OnMove(struct World *world,struct Player *player, enum ObjectFacing newDir) {

        struct Vector2 addToPos = {0,0};

        char *playerAnim = NULL;
        char *weaponAnim = NULL;
        enum AnimationMirrorFlip animFlipPlayer = ANIMATION_NOT_MIRRORED_FLIPPED;
        enum AnimationMirrorFlip animFlipWeapon = ANIMATION_NOT_MIRRORED_FLIPPED;

        if (newDir == NORTH) {
            newDir = NORTH;
            addToPos.y = -player->speed;
            playerAnim = "moveUp";
            animFlipPlayer = ANIMATION_NOT_MIRRORED_FLIPPED;
            weaponAnim = "up";
            animFlipWeapon = ANIMATION_NOT_MIRRORED_FLIPPED;
        }
        else if (newDir == EAST) {
            newDir = EAST;
            addToPos.x = player->speed;
            playerAnim = "moveRight";
            animFlipPlayer = ANIMATION_NOT_MIRRORED_FLIPPED;
            weaponAnim = "right";
            animFlipWeapon = ANIMATION_NOT_MIRRORED_FLIPPED;
        }

        else if (newDir == SOUTH) {
            newDir = SOUTH;
            addToPos.y = player->speed;
            playerAnim = "moveDown";
            animFlipPlayer = ANIMATION_NOT_MIRRORED_FLIPPED;
            weaponAnim = "down";
            animFlipWeapon = ANIMATION_NOT_MIRRORED_FLIPPED;
        }
        else if (newDir == WEST) {
            newDir = WEST;
            addToPos.x = -player->speed;
            playerAnim = "moveRight";
            animFlipPlayer = ANIMATION_FLIP;
            weaponAnim = "right";
            animFlipWeapon = ANIMATION_FLIP;
        }

        if (newDir != -1) {
            Player_SetFacingDirectin(player, newDir);

            Object_SetActiveAnimationByName(&player->object, playerAnim, animFlipPlayer);

            if (player->primaryWeapon)
                Object_SetActiveAnimationByName(&player->primaryWeapon->object, weaponAnim, animFlipWeapon);
            if (player->secondaryWeapon)
                Object_SetActiveAnimationByName(&player->secondaryWeapon->object, weaponAnim, animFlipWeapon);

        }
        player->object.objectAnimationsType = (newDir != -1)?ANIMATIONS_PLAYER:ANIMATIONS_SINGLE;
        if (player->primaryWeapon)
            player->primaryWeapon->object.objectAnimationsType = (newDir != -1)?ANIMATIONS_OBJECT:ANIMATIONS_SINGLE;
        if (player->secondaryWeapon)
            player->secondaryWeapon->object.objectAnimationsType = (newDir != -1)?ANIMATIONS_OBJECT:ANIMATIONS_SINGLE;


        if ((addToPos.x != 0 || addToPos.y != 0)) {
            bool canMove = true;

            struct Object playerObjectCopy = player->object;
            Object_MoveBy(&playerObjectCopy, addToPos);

            if (
                (playerObjectCopy.position.x + playerObjectCopy.size.y >= 1080 || playerObjectCopy.position.x <= 0) ||
                (playerObjectCopy.position.y + playerObjectCopy.size.y >= 720 || playerObjectCopy.position.y <= 0)
                ) {
                return false;
            }

            ///// KOLIZE
            for (int j = 0; j < world->objectCount; ++j) {
                if (Collsions_areColliding(&playerObjectCopy,&world->objects[j])){
                    if (world->objects[j].collision == COLLISION_BLOCK) {
                        canMove = false;
                    }
                }
            }


            if (canMove) {
                Player_MoveBy(player,addToPos);
            }

        }

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

    player->stats.ui.position.x = player->object.position.x+player->object.size.x/2 -player->stats.ui.size.x/2;
    player->stats.ui.position.y = player->object.position.y+player->object.size.y/2 -player->stats.ui.size.y/2 - 50;

    if (player->primaryWeapon != NULL) {
        Object_MoveBy(&player->primaryWeapon->object,addVector);
    }
    if (player->secondaryWeapon != NULL) {
        Object_MoveBy(&player->secondaryWeapon->object,addVector);
    }

}

void Player_PickUpWeapon(struct Player *player, struct Object *weaponObject) {
    player->secondaryWeapon = Weapon_CreateWeapon(weaponObject,1,1);
    Weapon_SetStatsByName(player->secondaryWeapon);
}

void Player_Die(struct Player *player) {
    printf("Player %s died\n",player->displayName);
    player->deathStatus.dead = true;
    player->deathStatus.lastDeathTime = SDL_GetTicks();
    player->object.collision = COLLISION_NONE;
}

void Player_Respawn(struct World *world,struct Player *player) {
    printf("Player %s respaned\n",player->displayName);
    player->object.collision = COLLISION_OVERLAP;
    player->HP = 2;
    player->deathStatus.dead = false;

    Object_SetRandomPosition(world,&player->object,100,900,100,600);

    Player_OnMove(world,player,NORTH);
}

int Player_TakeDamage(struct Player *player, int damage) {
    if (player == NULL){printf("Player je NULL pri dostani damage\n");return 0;}

    player->HP -= damage;
    if (player->HP <= 0) {
        printf("%s dostal %d damage\n",player->object.name,damage);
        player->stats.deaths++;
        Player_Die(player);
        return 1;
    }
    return 0;
}

struct Player *Player_GetByName(struct World *world,char *name) {
    for (int i = 0; i < world->playerCount; ++i) {
        if (strcmp(world->players[i].object.name, name) == 0) {
            return &world->players[i];
        }
    }
    return NULL;
}

void Player_UpdateStatsUITexture(SDL_Renderer *renderer,struct Player *player) {
    SDL_Color color = {PLAYER_STATS_UI_TEXT_COLOR};

    sprintf(player->stats.ui.text.textToDisplay,"K:%dD:%d",player->stats.kills,player->stats.deaths);

    player->stats.ui.text.textTexture = UI_GetTextTexture(renderer,player->stats.ui.text.textToDisplay,color,PLAYER_STATS_UI_TEXT_SIZE);
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