/*******************************************************************************************************************************************
 * 
             ___                      _                                _           __  __                                
            | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___    |  \/  |  ___   _ _    _  _   ___   ___
            | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_)   | |\/| | / -_) | ' \  | || | / -_) (_-<
            |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___|   |_|  |_| \___| |_||_|  \_,_| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/
//Este archivo contiene todas las funciones que se llaman mediante callbacks cuando se selecciona una opcion de un menu.

#include "selectOptions.h"


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

int selectDificulty(void){
    GAME_STATUS.menuAnterior = GAME_STATUS.menuActual;
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

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/