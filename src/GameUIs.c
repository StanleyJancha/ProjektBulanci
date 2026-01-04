//
// Created by standa on 1/4/26.
//

#include "ui.h"
#include "world.h"
#include "keybinds.h"

void loadMainMenuUIs(struct World *world,struct UI_Manager *ui_manager) {

    struct Vector2 exitGameButtonpos = {500,300};
    struct Vector2 exitGameButtonsize = {200,100};

    struct UI_Events *exitGameButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(exitGameButtonEvents->onClick, "exit_game");

    struct UI *exitGameButton = UI_CreateUI("exit_button",exitGameButtonpos,exitGameButtonsize,"Exit game",exitGameButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,exitGameButton,"basic_button");

    exitGameButton->text.color.r = 0;
    exitGameButton->text.color.g = 0;
    exitGameButton->text.color.b = 0;

    exitGameButton->text.size = 28;

    exitGameButton->text.textTexture = UI_GetTextTexture(world->renderer,exitGameButton->text);
    UI_Text_SetPadding(&exitGameButton->text,10,10);

    UI_Manager_AddUI(ui_manager,exitGameButton);

    free(exitGameButton);


    struct Vector2 startGamepos = {500,100};
    struct Vector2 startGamesize = {200,100};

    struct UI_Events *startGameEvents = malloc(sizeof(struct UI_Events));
    strcpy(startGameEvents->onClick, "start_game");

    struct UI *startGame = UI_CreateUI("start_button",startGamepos,startGamesize,"Start",startGameEvents,false);
    Animation_AddAnimationToUI(world->renderer,startGame,"basic_button");

    startGame->text.color.r = 0;
    startGame->text.color.g = 0;
    startGame->text.color.b = 0;

    startGame->text.size = 28;
    startGame->text.textTexture = UI_GetTextTexture(world->renderer,startGame->text);
    UI_Text_SetPadding(&startGame->text,10,10);

    UI_Manager_AddUI(ui_manager,startGame);

    free(startGame);


    ///
    ///


    struct Vector2 pos2 = {0,0};
    struct Vector2 size2 = {10,10};

    struct UI *scoreBoardUI = UI_CreateUI("score_board_main_menu",pos2,size2,"",NULL,false);
    Animation_AddAnimationToUI(world->renderer,scoreBoardUI,NULL);

    scoreBoardUI->text.color.r = 0;
    scoreBoardUI->text.color.g = 255;
    scoreBoardUI->text.color.b = 255;

    scoreBoardUI->text.size = 40;

    Gamerule_UpdateMainMenuScoreboard(world->renderer,&scoreBoardUI->text);

    UI_Manager_AddUI(ui_manager,scoreBoardUI);
    free(scoreBoardUI);

}

void loadPreGameUIs(struct World *world,struct UI_Manager *ui_manager) {

    struct Vector2 beginMatchButtonPos3 = {780,520};
    struct Vector2 beginMatchButtonSize3 = {200,100};

    struct UI_Events *beginMatchButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(beginMatchButtonEvents->onClick, "begin_match");

    struct UI *beginMatchButton = UI_CreateUI("begin_match_button",beginMatchButtonPos3,beginMatchButtonSize3,"Begin Match",beginMatchButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,beginMatchButton,"basic_button");

    if (strcmp(beginMatchButton->text.textToDisplay, "") != 0) {
        beginMatchButton->text.color.r = 0;
        beginMatchButton->text.color.g = 0;
        beginMatchButton->text.color.b = 0;

        beginMatchButton->text.size = 25;
        beginMatchButton->text.textTexture = UI_GetTextTexture(world->renderer,beginMatchButton->text);
        UI_Text_SetPadding(&beginMatchButton->text,10,10);
    }

    UI_Manager_AddUI(ui_manager,beginMatchButton);

    free(beginMatchButton);

    /////

    SDL_Color colors[4] = {{255,255,255},{0,255,0},{255,0,0},{0,0,255,}};

    for (int i = 0; i < 4; ++i) {

        struct Vector2 pos1 = {200,50 + (i)*150};
        struct Vector2 size1 = {200,100};

        char identifier[64] =  "player_text_field-";
        char iToStringBuffer[20];

        sprintf(iToStringBuffer, "%d", i);
        strcat(identifier,iToStringBuffer);

        struct UI *textInput1 = UI_CreateUI_TextField(world,identifier,pos1,size1,"basic_player_text_field");

        textInput1->text.color.r = 0;
        textInput1->text.color.g = 0;
        textInput1->text.color.b = 0;

        textInput1->text.size = 20;
        textInput1->text.textTexture = UI_GetTextTexture(world->renderer,textInput1->text);
        UI_Text_SetPadding(&textInput1->text,10,10);

        UI_Manager_AddUI(ui_manager,textInput1);

        struct Vector2 pos2 = {450,50 + (i)*150};
        struct Vector2 size2 = {300,100};

        struct UI *slotInfoUI = UI_CreateUI("slotInfoUI",pos2,size2,PlayerKeybindSets[i].info,NULL,false);
        Animation_AddAnimationToUI(world->renderer,slotInfoUI,"basic_text_background");

        slotInfoUI->text.color.r = 0;
        slotInfoUI->text.color.g = 0;
        slotInfoUI->text.color.b = 0;

        slotInfoUI->text.size = 25;
        slotInfoUI->text.textTexture = UI_GetTextTexture(world->renderer,slotInfoUI->text);
        UI_Text_SetPadding(&slotInfoUI->text,10,10);

        UI_Manager_AddUI(ui_manager,slotInfoUI);

        free(slotInfoUI);

        /////


        struct Vector2 pos3 = {80,80 + (i)*150};
        struct Vector2 size3 = {40,40};

        struct UI *playerColor = UI_CreateUI("playerColor",pos3,size3,"",NULL,false);
        SDL_Color *color = malloc(sizeof(SDL_Color));
        color->r = colors[i].r;
        color->g = colors[i].g;
        color->b = colors[i].b;
        color->a = 255;
        playerColor->basicBackgroundColor = color;

        UI_Manager_AddUI(ui_manager,playerColor);

        free(playerColor);
    }

    UI_Manager_PrintAllUIs(ui_manager);


    struct Vector2 pos3 = {780,400};
    struct Vector2 size3 = {200,100};

    struct UI_Events *mainMenuButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(mainMenuButtonEvents->onClick, "exit_to_main_menu");

    struct UI *mainMenuButton = UI_CreateUI("main_menu_button",pos3,size3,"Main Menu",mainMenuButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,mainMenuButton,"basic_button");

    if (strcmp(mainMenuButton->text.textToDisplay, "") != 0) {

        mainMenuButton->text.color.r = 0;
        mainMenuButton->text.color.g = 0;
        mainMenuButton->text.color.b = 0;

        mainMenuButton->text.size = 20;
        mainMenuButton->text.textTexture = UI_GetTextTexture(world->renderer,mainMenuButton->text);
        UI_Text_SetPadding(&mainMenuButton->text,20,10);
    }

    UI_Manager_AddUI(ui_manager,mainMenuButton);

    free(mainMenuButton);
/////
///
    struct Vector2 pos2 = {780,50};
    struct Vector2 size2 = {200,300};

    struct UI *infoPreGameUI = UI_CreateUI("infoPreGame",pos2,size2,"Vsechny texfieldy\n, ktere budou vyplnene\n jsou hraci. \nZbytek jsou boti",NULL,false);
    Animation_AddAnimationToUI(world->renderer,infoPreGameUI,"basic_text_background");

    infoPreGameUI->text.color.r = 0;
    infoPreGameUI->text.color.g = 0;
    infoPreGameUI->text.color.b = 0;

    infoPreGameUI->text.size = 18;

    infoPreGameUI->text.textTexture = UI_GetTextTexture(world->renderer,infoPreGameUI->text);

    UI_Manager_AddUI(ui_manager,infoPreGameUI);
    free(infoPreGameUI);
}



void loadPostGameUIs(struct World *world,struct UI_Manager *ui_manager) {
    SDL_Color color = {0,0,0,255};

    struct Vector2 pos3 = {700,500};
    struct Vector2 size3 = {200,100};

    struct UI_Events *mainMenuButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(mainMenuButtonEvents->onClick, "exit_to_main_menu");

    struct UI *mainMenuButton = UI_CreateUI("main_menu_button",pos3,size3,"Main Menu",mainMenuButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,mainMenuButton,"basic_button");

    if (strcmp(mainMenuButton->text.textToDisplay, "") != 0) {
        mainMenuButton->text.color.r = 0;
        mainMenuButton->text.color.g = 0;
        mainMenuButton->text.color.b = 0;

        mainMenuButton->text.size = 25;
        mainMenuButton->text.textTexture = UI_GetTextTexture(world->renderer,mainMenuButton->text);
        UI_Text_SetPadding(&mainMenuButton->text,10,10);
    }

    UI_Manager_AddUI(ui_manager,mainMenuButton);

    free(mainMenuButton);

    struct Vector2 pos2 = {0,0};
    struct Vector2 size2 = {200,100};

    struct UI *scoreBoard = UI_CreateUI("score_board",pos2,size2,"winner is",NULL,false);
    Animation_AddAnimationToUI(world->renderer,scoreBoard,NULL);

    scoreBoard->text.color.r = 0;
    scoreBoard->text.color.g = 255;
    scoreBoard->text.color.b = 255;

    scoreBoard->text.size = 40;
    scoreBoard->text.textTexture = UI_GetTextTexture(world->renderer,scoreBoard->text);

    UI_Manager_AddUI(ui_manager,scoreBoard);

    free(scoreBoard);

}



void loadPauseMenuUIs(struct World *world,struct UI_Manager *ui_manager) {
    SDL_Color color = {0,0,0,255};

    struct Vector2 pos = {500,300};
    struct Vector2 size = {200,100};

    struct UI_Events *pauseButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(pauseButtonEvents->onClick, "unpause_game");

    struct UI *pauseButton = UI_CreateUI("button_unpause",pos,size,"Unpause",pauseButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,pauseButton,"basic_button");


    pauseButton->text.color.r = 0;
    pauseButton->text.color.g = 0;
    pauseButton->text.color.b = 0;

    pauseButton->text.size = 25;
    pauseButton->text.textTexture = UI_GetTextTexture(world->renderer,pauseButton->text);
    UI_Text_SetPadding(&pauseButton->text,20,10);


    UI_Manager_AddUI(ui_manager,pauseButton);

    free(pauseButton);

    /////

    struct Vector2 pos3 = {500,500};
    struct Vector2 size3 = {200,100};

    struct UI_Events *mainMenuButtonEvents = malloc(sizeof(struct UI_Events));
    strcpy(mainMenuButtonEvents->onClick, "exit_to_main_menu");

    struct UI *mainMenuButton = UI_CreateUI("main_menu_button",pos3,size3,"Main Menu",mainMenuButtonEvents,false);
    Animation_AddAnimationToUI(world->renderer,mainMenuButton,"basic_button");

    if (strcmp(mainMenuButton->text.textToDisplay, "") != 0) {

        mainMenuButton->text.color.r = 0;
        mainMenuButton->text.color.g = 0;
        mainMenuButton->text.color.b = 0;

        mainMenuButton->text.size = 20;
        mainMenuButton->text.textTexture = UI_GetTextTexture(world->renderer,mainMenuButton->text);
        UI_Text_SetPadding(&mainMenuButton->text,20,10);


    }

    UI_Manager_AddUI(ui_manager,mainMenuButton);

    free(mainMenuButton);
}
void loadInGameUIs(struct World *world,struct UI_Manager *ui_manager) {

    struct Vector2 pos2 = {0,0};
    struct Vector2 size2 = {200,100};

    struct UI *gameTimerUI = UI_CreateUI("game_timer",pos2,size2,"00:00",NULL,false);
    Animation_AddAnimationToUI(world->renderer,gameTimerUI,NULL);

    gameTimerUI->text.color.r = 0;
    gameTimerUI->text.color.g = 0;
    gameTimerUI->text.color.b = 0;

    gameTimerUI->text.size = 40;
    gameTimerUI->text.textTexture = UI_GetTextTexture(world->renderer,gameTimerUI->text);

    UI_Text_SetPadding(&gameTimerUI->text,20,10);

    UI_Manager_AddUI(ui_manager,gameTimerUI);

    free(gameTimerUI);

}


void Game_UIs_Load(struct World *world,struct Game_UIs *game_UIs) {
    game_UIs->mainMenu = UI_Manager_Create();
    loadMainMenuUIs(world,game_UIs->mainMenu);

    game_UIs->pauseMenu = UI_Manager_Create();
    loadPauseMenuUIs(world,game_UIs->pauseMenu);

    game_UIs->inGame = UI_Manager_Create();
    loadInGameUIs(world,game_UIs->inGame);

    game_UIs->postGame = UI_Manager_Create();
    loadPostGameUIs(world,game_UIs->postGame);

    game_UIs->preGame = UI_Manager_Create();
    loadPreGameUIs(world,game_UIs->preGame);
}


void Game_UIs_ClearAllTextfieldsInManager(struct World *world,struct UI_Manager *ui_manager) {
    for (int i = 0; i < ui_manager->count; ++i) {
        struct UI *ui = &ui_manager->UIs[i];

        if (!ui->text.isInput) {
            continue;
        }
        strcpy(ui->text.textToDisplay,"");
        ui->text.textTexture = UI_GetTextTexture(world->renderer,ui->text);
    }

}