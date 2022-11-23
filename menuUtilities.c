/*******************************************************************************************************************************************
 * 
                                                    _    _   _     _   _   _   _     _                         
                                                   | |  | | | |   (_) | | (_) | |   (_)                        
                 _ __ ___     ___   _ __    _   _  | |  | | | |_   _  | |  _  | |_   _    ___   ___        ___ 
                | '_ ` _ \   / _ \ | '_ \  | | | | | |  | | | __| | | | | | | | __| | |  / _ \ / __|      / __|
                | | | | | | |  __/ | | | | | |_| | | |__| | | |_  | | | | | | | |_  | | |  __/ \__ \  _  | (__ 
                |_| |_| |_|  \___| |_| |_|  \__,_|  \____/   \__| |_| |_| |_|  \__| |_|  \___| |___/ (_)  \___|                                 
 * 
 ******************************************************************************************************************************************/
 //Este archivo contiene todas las funciones y las variables necesarias para el funcionamiento de los menues tanto en allegro como en raspi

#include "menuUtilities.h"

/*******************************************************************************************************************************************
 * 
                                             _ __    ___   _ _    _  _   ___   ___
                                            | '  \  / -_) | ' \  | || | / -_) (_-<
                                            |_|_|_| \___| |_||_|  \_,_| \___| /__/                                                                                                                                                                               
 * 
 ******************************************************************************************************************************************/

#ifdef RASPI
menu_t menuInicio = { &KEYS , {selectPlayInicio, selectLevels, selectLeaderboard, selectSaveScore, selectVolume, selectQuitGame, backMenuAnterior},
                      {"Quick Play    ", "Levels    ", "Leaderboard    ", "Save Score    ", "Volume    ", "Quit Game    "}, 
                      {&halfDispAlienSpaceInvaders, &halfDispSelectLevels, &halfDispTrophy, &halfDispSaveScore, &halfDispVolume, &halfDispQuitGame}, 
                      6 , 1 , changeOption };//Estructura del menu de inicio.

menu_t menuPausa = { &KEYS , {selectResume, selectRestartLevel, selectMainMenu, selectLevels, selectVolume, selectQuitGame, backMenuAnterior},
                      {"Resume    ", "Restart Level    ", "Main menu    ", "Select level    ", "Volume    ", "Quit Game    "}, 
                      {&halfDispResume, &halfDispRestart, &halfDispHome, &halfDispSelectLevels, &halfDispVolume, &halfDispQuitGame}, 
                      6 , 1 , changeOption };//Estructura del menu de pausa.

menu_t menuLostLevel = { &KEYS , {selectNothing, selectRestartLevel, selectMainMenu, selectLevels, selectVolume, selectQuitGame, backMenuAnterior, backMenuAnterior}, 
                      {"Lost Level    ", "Restart Level    ", "Main menu    ", "Select level    ", "Volumen    ", "Quit Game    "}, 
                      {&halfDispLostGame, &halfDispRestart, &halfDispHome, &halfDispSelectLevels, &halfDispVolume, &halfDispQuitGame},
                      6 , 1 , changeOption };//Estructura del menu de cuando se pierde un nivel.

menu_t menuWonLevel = { &KEYS , {selectNothing, selectRestartLevel, selectRestartLevel, selectMainMenu, selectLevels, selectVolume, selectQuitGame, backMenuAnterior}, 
                      {"Won Level    ", "Next Level    ", "Restart Level    ", "Main menu    ", "Select level    ", "Volumen    ", "Quit Game    "}, 
                      {&halfDispAlienSpaceInvaders, &halfDispNextLevel, &halfDispRestart, &halfDispHome, &halfDispSelectLevels, &halfDispVolume, &halfDispQuitGame}, 
                      7 , 1 , changeOption };//Estructura del menu de cuando se gana un nivel.

menu_t menuLeaderboard = { &KEYS , {selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing}, 
                      {"1. 0000    ", "2. 0000    ", "3. 0000    ", "4. 0000    ", "5. 0000    ", "6. 0000    ", "7. 0000    ", "8. 0000    ", "9. 0000    ", "10. 0000    "}, 
                      {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, 
                      10 , 1 , changeOption };//Estructura del menu del leaderboard.

menu_t menuLevels = { &KEYS , {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, 
                      {"Nivel 1    ", "Nivel 2    ", "Nivel 3    ", "Nivel 4    ", "Nivel 5    ", "Nivel 6    ", "Nivel 7    ", "Nivel 8    ", "Nivel 9    ", "Nivel 10    ", "Nivel 11    ", "Nivel 12    ", "Nivel 13    ", "Nivel 14    ", "Nivel 15    ", "Nivel 16    ", "Nivel 17    ", "Nivel 18    ", "Nivel 19    ", "Nivel 20    "}, 
                      {&halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders}, 
                      10 , 1 , changeOption };//Estructura del menu de seleccionar nivel.
#endif


#ifdef ALLEGRO
menu_t menuInicio = { &KEYS , {selectPlayInicio, selectLevels, selectLeaderboard, selectSaveScore, selectVolume, selectQuitGame, backMenuAnterior},
                      {"Quick Play    ", "Levels    ", "Leaderboard    ", "Save Score    ", "Volume    ", "Quit Game    "}, 
                      {"game/spritesAllegro/spriteQP.png", "game/spritesAllegro/spriteLVL.png", "game/spritesAllegro/spriteLB.png", "game/spritesAllegro/spriteSS.png", "game/spritesAllegro/spriteV.png", "game/spritesAllegro/spriteQG.png"},
                      6 , 1 , changeOption };//Estructura del menu de inicio.

menu_t menuPausa    = { &KEYS , {selectResume, selectRestartLevel, selectMainMenu, selectLevels, selectVolume, selectQuitGame, backMenuAnterior},
                      {"Resume ", "Restart Level ", "Main menu ", "Select level ", "Volume ", "Quit Game "},
                      {NULL}, 
                      6, 1, changeOption };//Estructura del menu de pausa.

menu_t menuLostLevel= { &KEYS , {selectRestartLevel, selectMainMenu, selectLevels, selectVolume, selectQuitGame, backMenuAnterior},
                      {"Restart Level ", "Main menu ", "Select level ", "Volumen ", "Quit Game "},
                      {NULL},  
                      5, 1, changeOption };//Estructura del menu de pausa.

menu_t menuWonLevel = { &KEYS , {selectRestartLevel, selectRestartLevel, selectMainMenu, selectLevels, selectVolume, selectQuitGame, backMenuAnterior},
                      {"Next Level ", "Restart Level ", "Main menu ", "Select level ", "Volumen ", "Quit Game "},
                      {NULL}, 
                      6, 1, changeOption };//Estructura del menu de pausa.

menu_t menuLeaderboard = { &KEYS , {selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, backMenuAnterior},
                      {"1. 0000    ", "2. 0000    ", "3. 0000    ", "4. 0000    ", "5. 0000    ", "6. 0000    ", "7. 0000    ", "8. 0000    ", "9. 0000    ", "10. 0000    "},
                      {NULL},  
                      10 , 1 , changeOption };//Estructura del menu de pausa.

menu_t menuLevels   = { &KEYS , {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, backMenuAnterior}, 
                      {"Nivel 1    ", "Nivel 2    ", "Nivel 3    ", "Nivel 4    ", "Nivel 5    ", "Nivel 6    ", "Nivel 7    ", "Nivel 8    ", "Nivel 9    ", "Nivel 10    ", "Nivel 11    ", "Nivel 12    ", "Nivel 13    ", "Nivel 14    ", "Nivel 15    ", "Nivel 16    ", "Nivel 17    ", "Nivel 18    ", "Nivel 19    ", "Nivel 20    "},
                      {NULL}, 
                      20 , 1 , changeOption };//Estructura del menu de pausa.
#endif
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
             ___                      _                                _           __  __                                
            | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___    |  \/  |  ___   _ _    _  _   ___   ___
            | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_)   | |\/| | / -_) | ' \  | || | / -_) (_-<
            |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___|   |_|  |_| \___| |_||_|  \_,_| \___| /__/                                                                                                                                                 
 * 
 ******************************************************************************************************************************************/
//Esta seccion contiene todas las funciones que se llaman mediante callbacks cuando se selecciona una opcion de un menu.

void fillLeaderboardMenu(menu_t * menuLeaderboard){
//Esta funcion lee el archivo del leaderboard y rellena la variable menu_t con los puntajes de cada usuario.
    int i;
    leaderboard_t leaderboard;
    parseScore(leaderboard);  //Se lee el leaderboard de score.txt

    //Primero se agregan los puntajes.
    for(i = 0 ; i<LEADERBOARD ; i++){ //Se cargan en las pantallas del menu cada una de las posiciones del leaderboard
        char copiaLeaderboard [ROWSIZE];
        char puntajeMenu[60];
        
        strcpy(copiaLeaderboard, leaderboard[i]); //Accede al valor numerico de puntaje de cada posicion
        #ifdef RASPI
        strtok(copiaLeaderboard," "); //Corta el texto del leaderboard hasta el espacio, se queda solo con el numero
        #endif

        sprintf(puntajeMenu, "%d.%s    ",i+1,copiaLeaderboard); //Crea un string de la forma en la que se miestra en el display
        strcpy((menuLeaderboard->textOpciones)[i], puntajeMenu); //Guarda el valor de puntaje en cada posicion del menu.
    }
    
}


int selectPlayInicio(void){
    printf("Select Play Inicio\n");
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = START_LEVEL;
    GAME_STATUS.menuAnterior = -1;
    return 0;
}

int selectLevels(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = MENU;
    GAME_STATUS.menuAnterior = GAME_STATUS.menuActual;
    GAME_STATUS.menuActual = MENU_LEVELS;
    return 0;
}

int selectVolume(void){
    GAME_STATUS.menuAnterior = GAME_STATUS.menuActual;
    return 1;
}

int selectQuitGame(void){
    printf("Select Quit Game\n");
    #ifdef RASPI
    velDispAnimation = 1;
    sem_wait(&SEM_DRIVER);
    disp_clear();
    sem_post(&SEM_DRIVER);
    #endif
    GAME_STATUS.pantallaActual = QUIT_GAME;
    return 0;
}

int selectResume(void){
    printf("Select Resume\n");
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.menuAnterior = -1;
    GAME_STATUS.pantallaActual = IN_GAME;
    menuGame.exitStatus = 1;
    return 0;
}

int selectRestartLevel(void){
    printf("Select Restart Level\n");
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = DESTROY_LEVEL;
    GAME_STATUS.menuActual = START_LEVEL_MENU;
    GAME_STATUS.menuAnterior = -1;
    return 0;
}

int selectMainMenu(void){
    printf("Select Main Menu\n");
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = DESTROY_LEVEL;
    GAME_STATUS.menuActual = MENU_INICIO;
    GAME_STATUS.menuAnterior = -1;
    return 0;
}

int selectLeaderboard(void){
    printf("Select Leaderboard\n");
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.menuAnterior = GAME_STATUS.menuActual;
    GAME_STATUS.menuActual = MENU_LEADERBOARD;
    GAME_STATUS.pantallaActual = MENU;
    return 0;
}

int selectSaveScore(void){
    printf("Select Save Score\n");
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = SAVE_SCORE;
    printf("Save score saliendo\n");
    return 0;
}

int backMenuAnterior(void){
    printf("Select Back Menu Anterior\n");
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = MENU;
    GAME_STATUS.menuActual = GAME_STATUS.menuAnterior;
    GAME_STATUS.menuAnterior = -1;
    return 0;
}

int selectNothing(void){
    return 1;
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/