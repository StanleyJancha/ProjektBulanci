//
// Created by standa on 11/15/25.
//
#include <SDL_ttf.h>
#include "ui.h"

#include "world.h"

void UI_Manager_Destroy(struct UI_Manager *manager) {
    if (manager == NULL) {
        return;
    }

    for (int i = 0; i < manager->count; ++i) {
        Animation_RemoveAnimation(&manager->UIs[i].animation);
        if (manager->UIs[i].text.textTexture != NULL) {
            SDL_DestroyTexture(manager->UIs[i].text.textTexture);
        }
        if (manager->UIs[i].events != NULL) {
            free(manager->UIs[i].events);
            manager->UIs[i].events = NULL;
        }
    }
}


struct UI_Manager *UI_Manager_Create() {
    struct UI_Manager *ui_manager = malloc(sizeof(struct UI_Manager));
    ui_manager->UIs = NULL;
    ui_manager->count = 0;
    return ui_manager;
}

struct UI *UI_CreateUI(char identifier[64], struct Vector2 position, struct Vector2 size,char text[64],struct UI_Events *events, bool isTextInput) {
    struct UI *ui = malloc(sizeof(struct UI));
    if (ui == NULL) return NULL;
    strcpy(ui->identifier, identifier);
    ui->size = size;
    ui->position = position;
    ui->visibility = VISIBLE;
    ui->child = NULL;

    strcpy(ui->text.textToDisplay,text);
    ui->text.isInput = isTextInput;

    ui->events = events;

    return ui;
}

struct UI *UI_CreateUI_TextField(struct World *world,char identifier[64], struct Vector2 position, struct Vector2 size, char *specialAnimationName) {
    SDL_Color color = {255,255,255,255};

    struct UI_Events *textInputFieldPlayer1Events = malloc(sizeof(struct UI_Events));
    strcpy(textInputFieldPlayer1Events->onClick, "text_field");

    struct UI *textInputFieldPlayer = UI_CreateUI(identifier,position,size,"",textInputFieldPlayer1Events,true);
    Animation_AddAnimationToUI(world->renderer,textInputFieldPlayer,specialAnimationName);

    textInputFieldPlayer->text.textTexture = UI_GetTextTexture(world->renderer,textInputFieldPlayer->text.textToDisplay,color,25);

    return textInputFieldPlayer;
}

bool UI_SetChild(struct UI *parent, struct UI *child) {
    if (parent == NULL) {printf("UI_SetChild - parent not valid\n") ;return false;}
    if (child == NULL) {printf("UI_SetChild - child for \"%s\" not valid\n",parent->identifier) ;return false;}

    parent->child = child;
    return true;

}

bool UI_Manager_AddUI(struct UI_Manager *uiManager, struct UI *ui) {
    if (uiManager->count == 0) {
        uiManager->UIs = malloc(sizeof(struct UI) * 1);
        if (uiManager->UIs == NULL){printf("UI_Manager_Add - nepodarilo se nacist pamet");return false;}
        uiManager->UIs[0] = *ui;
        uiManager->count = 1;
        return true;
    }

    struct UI *temp = malloc(sizeof(struct UI) * (uiManager->count + 1));;
    if (temp == NULL) {printf("UI_Manager_Add - nepodarilo se nacist pamet");return false;}
    for (int i = 0; i < uiManager->count; ++i) {
        temp[i] = uiManager->UIs[i];
    }
    temp[uiManager->count] = *ui;
    uiManager->UIs = temp;
    uiManager->count++;
    return true;
}



SDL_Texture *UI_GetTextTexture(SDL_Renderer *renderer, char *text, SDL_Color color, int ptsize) {
    TTF_Font *font = TTF_OpenFont("fonts/Playwrite.ttf", ptsize);
    if (!font) {printf("Font error: %s\n", TTF_GetError());return NULL;}

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);

    return texture;
}


void UI_Print(struct UI *ui) {
    printf("id: %s\nanimation name:\n",ui->identifier);
}

void UI_Manager_PrintAllUIs(struct UI_Manager *ui_manager) {
    for (int i = 0; i < ui_manager->count; ++i) {
        UI_Print(&ui_manager->UIs[i]);
    }
}

bool UI_MouseInside(struct UI *ui, struct Vector2 mouse) {
    return  mouse.x >= ui->position.x &&
            mouse.x <= ui->position.x + ui->size.x &&
            mouse.y >= ui->position.y &&
            mouse.y <= ui->position.y + ui->size.y;
}


struct UI *UI_MouseOnUI(struct UI_Manager *uiManager,struct Vector2 mousePos) {
    for (int i = uiManager->count - 1; i >= 0; --i) {
        if (uiManager->UIs[i].events != NULL){
            if (strcmp(uiManager->UIs[i].events->onClick, "") != 0) {
                if (UI_MouseInside(&uiManager->UIs[i],mousePos)) {
                    return &uiManager->UIs[i];
                }
            }
        }
    }
    return NULL;
}

bool UI_ButtonCallEvent(struct World *world,struct Gamerule *gamerule,struct UI_Manager *ui_manager,struct UI *ui) {
    char onClickEventName[68];
    strcpy(onClickEventName,ui->events->onClick);
    printf("akce pro %s\n",onClickEventName);

    if (strcmp(onClickEventName,"unpause_game") == 0) {
        Gamerule_UnpauseGame(gamerule);
        return true;
    }
    if (strcmp(onClickEventName,"start_game") == 0) {
        gamerule->gamestates.gamestate = GAME_PRE_PLAY;
        return true;
    }
    if (strcmp(onClickEventName,"begin_match") == 0) {
        char playerNames[4][64] = {0};
        bool atleastOnePlayer = false;
        int j = 0;
        for (int i = 0; i < ui_manager->count; ++i) {
            char tmp[64];
            strcpy(tmp,ui_manager->UIs[i].identifier);
            char *name = strtok(tmp,"-");
            if (strcmp(name,"player_text_field") == 0) {
                if (strcmp(ui_manager->UIs[i].text.textToDisplay,"") != 0) {
                    atleastOnePlayer = true;
                }
                strcpy(playerNames[j],ui_manager->UIs[i].text.textToDisplay);
                j++;
            }
        }

        if (!atleastOnePlayer) {
            return false;
        }



        Gamerule_StartGame(world,gamerule,playerNames);
        gamerule->gamestates.gamestate = GAME_IN_GAME;
        return true;
    }

    if (strcmp(onClickEventName,"exit_game") == 0) {
        gamerule->gamestates.appRunning = false;
        return true;
    }
    if (strcmp(onClickEventName,"exit_to_main_menu") == 0) {
        Gamerule_EndGame(world,gamerule,false);
        gamerule->gamestates.gamestate = GAME_IN_MAIN_MENU;
        return true;
    }
    if (strcmp(onClickEventName,"text_field") == 0) {
        if (ui->text.isInput == true){
            if (gamerule->inputUI == NULL || strcmp(gamerule->inputUI->identifier,ui->identifier) != 0) {
                gamerule->inputUI = ui;
                SDL_StartTextInput();
            }else {
                SDL_StopTextInput();
                gamerule->inputUI = NULL;
                printf("Unclick\n");
            }
            printf("name: %s\n",gamerule->inputUI->identifier);



            // bool newUi = (gamerule->inputUI)?(strcmp(gamerule->inputUI->identifier,ui->identifier) == 0):false; // jestli nove kliknuty UI je uz inputUI nebo jesti jsme klikli na jiny
            // gamerule->inputUI = ui;
            // if(SDL_IsTextInputActive() && !newUi) {
            //     SDL_StopTextInput();
            //     gamerule->inputUI = NULL;
            //     printf("inut stoppedn\n");
            // }else {
            //     SDL_StartTextInput();
            //     printf("inut started\n");
            // }
        }
        return true;

    }
    return false;
}

struct UI *UI_Manager_GetUIByIdentifier(struct UI_Manager *uiManager,char *identifier) {
    for (int i = 0; i < uiManager->count; ++i) {
        if (strcmp(uiManager->UIs[i].identifier,identifier) == 0) {
            return &uiManager->UIs[i];
        }
    }
    return NULL;
}
