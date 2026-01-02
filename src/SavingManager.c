// //
// // Created by standa on 10/28/25.
// //
// #include "SavingManager.h"
//
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
//
// void SavingManagaer_SaveMatch(struct World *world,struct Gamerule *gamerule){
//     struct MatchSave match;
//
//     match.gameLength = gamerule->gameTimes.gameLengthMinutes;
//
//     for (int i = 0; i < world->playerCount; ++i) {
//         struct Player player = world->players[i];
//
//         strcpy(match.playerSave[i].name,player.displayName);
//         memcpy(&match.playerSave[i].playerStats,&player.stats,sizeof(struct PlayerStats));
//     }
//
//     struct MatchSave *matches = NULL;
//     SavingManagaer_GetMatchHistory(matches);
//
//     free(matches);
//
// }
//
// void SavingManager_GetMatchHistory(struct MatchSave *matchSaves) {
//
//     FILE *fp = fopen("matchHistory", "rb");
//
//     if (fp == NULL) {
//         return;
//     }
//
//     fseek(fp, 0, SEEK_END);
//     long fileSize = ftell(fp);
//     rewind(fp);
//
//     int count = fileSize / sizeof(struct MatchSave);
//     matchSaves = malloc(sizeof(struct MatchSave) * count);
//
//     fread(matchSaves, sizeof(struct MatchSave), count, fp);
//
//     fclose(fp);
// }
