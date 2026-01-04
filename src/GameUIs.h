//
// Created by standa on 1/4/26.
//

#ifndef PROJEKT_GAMEUIS_H
#define PROJEKT_GAMEUIS_H


struct Game_UIs {
    struct UI_Manager *mainMenu;
    struct UI_Manager *preGame;
    struct UI_Manager *postGame;
    struct UI_Manager *pauseMenu;
    struct UI_Manager *inGame;
};

void Game_UIs_Load(struct World *world,struct Game_UIs *game_UIs);
void Game_UIs_ClearAllTextfieldsInManager(struct World *world,struct UI_Manager *ui_manager);

#endif //PROJEKT_GAMEUIS_H