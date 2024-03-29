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
                                        ___               _           _     _                   
                                        | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___
                                        |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<
                                        |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/
                                                                                |_|                                                            
 * 
 ******************************************************************************************************************************************/
static int selectPlayInicio(void);
static int selectLevels(void);
static int selectVolume(void);
static int selectQuitGame(void);
static int selectResume(void);
static int selectRestartLevel(void);
static int selectNextLevel(void);
static int selectMainMenu(void);
static int selectLeaderboard(void);
static int selectSaveScore(void);
static int backMenuAnterior(void);
static int selectNothing(void);
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/
gameStatus_t GAME_STATUS = { .pantallaActual = MENU, .nivelActual = 0 , .menuActual = 0, .menuAnterior = -1, .inGame = 0, .usrLives = MAX_USR_LIVES, .exitStatus = 1};
game_t menuGame = { &KEYS, NULL, NULL, NULL, 0, NULL, NULL}; //Estructura del level handler.

/*******************************************************************************************************************************************
 * 
                                             _ __    ___   _ _    _  _   ___   ___
                                            | '  \  / -_) | ' \  | || | / -_) (_-<
                                            |_|_|_| \___| |_||_|  \_,_| \___| /__/                                                                                                                                                                               
 * 
 ******************************************************************************************************************************************/

#ifdef RASPI
menu_t menuInicio = { &KEYS , {selectPlayInicio, selectLevels, selectLeaderboard, selectVolume, selectQuitGame}, backMenuAnterior,
                      {"Quick Play    ", "Levels    ", "Leaderboard    ", "Volume    ", "Quit Game    "}, 
                      {&halfDispAlienSpaceInvaders, &halfDispSelectLevels, &halfDispTrophy, &halfDispVolume, &halfDispQuitGame}, 
                      5 , 1 , changeOption };//Estructura del menu de inicio.

menu_t menuPausa = { &KEYS , {selectResume, selectRestartLevel, selectVolume, selectMainMenu,  selectQuitGame}, backMenuAnterior,
                      {"Resume    ", "Restart Level    ","Volume    ","Main menu    ", "Quit Game    "}, 
                      {&halfDispResume, &halfDispRestart,  &halfDispVolume, &halfDispHome, &halfDispQuitGame}, 
                      5 , 1 , changeOption };//Estructura del menu de pausa.

menu_t menuLostLevel = { &KEYS , {selectNothing, selectSaveScore, selectRestartLevel, selectMainMenu, selectLevels, selectQuitGame}, backMenuAnterior, 
                      {"Lost Level    ","Save Score   ", "Restart Level    ", "Main menu    ", "Select level    ", "Quit Game    "}, 
                      {&halfDispLostGame, &halfDispSaveScore, &halfDispRestart, &halfDispHome, &halfDispSelectLevels, &halfDispQuitGame},
                      6 , 1 , changeOption };//Estructura del menu de cuando se pierde un nivel.

menu_t menuWonLevel = { &KEYS , {selectNothing, selectNextLevel, selectSaveScore, selectMainMenu, selectQuitGame}, backMenuAnterior, 
                      {"Won Level    ", "Next Level    ", "Save Score    ", "Main menu    ",  "Quit Game    "}, 
                      {&halfDispAlienSpaceInvaders, &halfDispNextLevel, &halfDispSaveScore ,&halfDispHome,  &halfDispQuitGame}, 
                      5 , 1 , changeOption };//Estructura del menu de cuando se gana un nivel.

menu_t menuLeaderboard = { &KEYS , {selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing}, backMenuAnterior, 
                      {"1. 0000    ", "2. 0000    ", "3. 0000    ", "4. 0000    ", "5. 0000    ", "6. 0000    ", "7. 0000    ", "8. 0000    ", "9. 0000    ", "10. 0000    "}, 
                      {&halfDispVolume0, &halfDispVolume0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, 
                      10 , 1 , changeOption };//Estructura del menu del leaderboard.

menu_t menuLevels = { &KEYS , {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, backMenuAnterior, 
                      {"Nivel 1    ", "Nivel 2    ", "Nivel 3    ", "Nivel 4    ", "Nivel 5    ", "Nivel 6    ", "Nivel 7    ", "Nivel 8    ", "Nivel 9    ", "Nivel 10    ", "Nivel 11    ", "Nivel 12    ", "Nivel 13    ", "Nivel 14    ", "Nivel 15    ", "Nivel 16    ", "Nivel 17    ", "Nivel 18    ", "Nivel 19    ", "Nivel 20    "}, 
                      {&halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders, &halfDispAlienSpaceInvaders}, 
                      8 , 1 , changeOption };//Estructura del menu de seleccionar nivel.

menu_t menuVolume = { &KEYS , {selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing}, backMenuAnterior, 
                      {" 00 ", " 01 ", " 02 ", " 03 ", " 04 ", " 05 ", " 06 ", " 07 ", " 08 "}, 
                      {&halfDispVolume0, &halfDispVolume1, &halfDispVolume2, &halfDispVolume3, &halfDispVolume4, &halfDispVolume5, &halfDispVolume6, &halfDispVolume7, &halfDispVolume8}, 
                      9 , 1 , changeOption };//Estructura del menu de seleccionar volumen.
#endif


#ifdef ALLEGRO
menu_t menuInicio = { &KEYS , {selectPlayInicio, selectLevels, selectLeaderboard, selectVolume, selectQuitGame}, backMenuAnterior,
                      {"Quick Play    ", "Levels    ", "Leaderboard    ", "Volume    ", "Quit Game    "}, 
                      {"game/spritesAllegro/spriteQP.png", "game/spritesAllegro/spriteLVL.png", "game/spritesAllegro/spriteLB.png", "game/spritesAllegro/spriteV.png", "game/spritesAllegro/spriteQG.png"},
                      NULL,
                      5 , 1 , changeOption };//Estructura del menu de inicio.

menu_t menuPausa    = { &KEYS , {selectResume, selectRestartLevel, selectMainMenu, selectVolume, selectQuitGame}, backMenuAnterior,
                      {"Resume", "Restart", "Main menu", "Volume", "Quit Game"},
                      {},
                      "Pause", 
                      5, 1, changeOption};//Estructura del menu de pausa.

menu_t menuLostLevel= { &KEYS , {selectSaveScore, selectRestartLevel, selectMainMenu, selectLevels, selectQuitGame}, backMenuAnterior, 
                      {"Save Score", "Restart", "Main menu", "Select level", "Quit Game"}, 
                      {},
                      "You Lose",  
                      5, 1, changeOption };//Estructura del menu de pausa.

menu_t menuWonLevel = { &KEYS , {selectNextLevel, selectSaveScore, selectMainMenu, selectQuitGame}, backMenuAnterior, 
                      {"Next Level", "Save Score", "Main menu", "Quit Game"}, 
                      {{0}},
                      "You Won", 
                      4, 1, changeOption };//Estructura del menu de pausa.

menu_t menuLeaderboard = { &KEYS , {selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing, selectNothing}, backMenuAnterior,
                      {"1. 00000   ", "2. 00000   ", "3. 00000   ", "4. 00000   ", "5. 00000   ", "6. 00000   ", "7. 00000   ", "8. 00000   ", "9. 00000   ", "10. 00000   "},
                      {{0}},
                      "",  
                      10 , 1 , NULL };//Estructura del menu de pausa.

menu_t menuLevels   = { &KEYS , {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, backMenuAnterior, 
                      {"Nivel 1", "Nivel 2", "Nivel 3", "Nivel 4", "Nivel 5", "Nivel 6", "Nivel 7", "Nivel 8", "Nivel 9", "Nivel 10", "Nivel 11", "Nivel 12", "Nivel 13", "Nivel 14", "Nivel 15", "Nivel 16", "Nivel 17", "Nivel 18", "Nivel 19", "Nivel 20"},
                      {},
                      "Levels", 
                      5 , 1 , changeOption};//Estructura del menu de pausa.

menu_t menuVolume = { &KEYS , {backMenuAnterior, backMenuAnterior, backMenuAnterior, backMenuAnterior, backMenuAnterior, backMenuAnterior, backMenuAnterior, backMenuAnterior, backMenuAnterior, backMenuAnterior, backMenuAnterior}, backMenuAnterior, 
                      {"00 ", "01 ", "02 ", "03 ", "04 ", "05 ", "06 ", "07 ", "08 ", "09 ", "10 "}, 
                      {{0}},
                      "Volume", 
                      11 , 1 , changeVolume };//Estructura del menu de seleccionar volumen.
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

        sprintf(puntajeMenu, "%d. %s    ",i+1,copiaLeaderboard); //Crea un string de la forma en la que se miestra en el display
        strcpy((menuLeaderboard->textOpciones)[i], puntajeMenu); //Guarda el valor de puntaje en cada posicion del menu.
    }
        
}


static int selectPlayInicio(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = DESTROY_LEVEL;
    GAME_STATUS.menuActual = START_LEVEL_MENU;
    GAME_STATUS.menuAnterior = -1;
    GAME_STATUS.nivelActual = 1;
    return 0;
}

static int selectLevels(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = MENU;
    GAME_STATUS.menuAnterior = GAME_STATUS.menuActual;
    GAME_STATUS.menuActual = MENU_LEVELS;
    return 0;
}

static int selectVolume(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = MENU;
    GAME_STATUS.menuAnterior = GAME_STATUS.menuActual;
    GAME_STATUS.menuActual = MENU_VOLUME;
    return 0;
}

static int selectQuitGame(void){
    #ifdef RASPI
    velDispAnimation = 1;
    clearDisplay();
    #endif
    GAME_STATUS.pantallaActual = QUIT_GAME;
    return 0;
}

static int selectResume(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.menuAnterior = -1;
    GAME_STATUS.pantallaActual = IN_GAME;
    menuGame.exitStatus = 1;
    return 0;
}

static int selectRestartLevel(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = DESTROY_LEVEL;
    GAME_STATUS.menuActual = START_LEVEL_MENU;
    GAME_STATUS.menuAnterior = -1;
    return 0;
}

static int selectNextLevel(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = DESTROY_LEVEL;
    GAME_STATUS.menuActual = START_LEVEL_MENU;
    GAME_STATUS.menuAnterior = -1;
    GAME_STATUS.nivelActual += 1;
    #ifdef RASPI
    char platform[4] = "rpi";
    #endif

    #ifdef ALLEGRO
    char platform[4] = "lnx";
    #endif

    directory_t carpetaNiveles = {};
    level_t levelArray[MAX_LEVEL];
    indexAllLevels(platform,LEVELSDIR, LEVELPREFIX, &carpetaNiveles, levelArray);
    
    if(GAME_STATUS.nivelActual >= levelArrayLen(levelArray)){//Si se llego al ultimo nivel.
        GAME_STATUS.nivelActual -= 1;
    }
    return 0;
}

static int selectMainMenu(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = DESTROY_LEVEL;
    GAME_STATUS.menuActual = MENU_INICIO;
    GAME_STATUS.menuAnterior = -1;
    GAME_STATUS.inGame = 0;
    return 0;
}

static int selectLeaderboard(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.menuAnterior = GAME_STATUS.menuActual;
    GAME_STATUS.menuActual = MENU_LEADERBOARD;
    GAME_STATUS.pantallaActual = MENU;
    return 0;
}

static int selectSaveScore(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = SAVE_SCORE;
    return 0;
}

static int backMenuAnterior(void){
    #ifdef RASPI
    velDispAnimation = 1;
    #endif
    GAME_STATUS.pantallaActual = MENU;
    GAME_STATUS.menuActual = GAME_STATUS.menuAnterior;
    GAME_STATUS.menuAnterior = -1;
    return 0;
}

static int selectNothing(void){
    return 1;
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/