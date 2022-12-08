/*******************************************************************************************************************************************
 * 
                                                      _    _   _     _   _   _   _     _                    _     
                                                     | |  | | | |   (_) | | (_) | |   (_)                  | |    
                    _ __ ___    ___   _ __    _   _  | |  | | | |_   _  | |  _  | |_   _    ___   ___      | |__  
                  | '_ ` _ \   / _ \ | '_ \  | | | | | |  | | | __| | | | | | | | __| | |  / _ \ / __|     | '_ \ 
                  | | | | | | |  __/ | | | | | |_| | | |__| | | |_  | | | | | | | |_  | | |  __/ \__ \  _  | | | |
                  |_| |_| |_|  \___| |_| |_|  \__,_|  \____/   \__| |_| |_| |_|  \__| |_|  \___| |___/ (_) |_| |_|                                
 * 
 ******************************************************************************************************************************************/

#ifndef MENU_UTILITIES_H
#define MENU_UTILITIES_H

#include "spaceLib/spaceLib.h"
#include "levelLoader/levelLoader.h"
#include "spaceLib/score/score.h"
#include <stdio.h>
#include <semaphore.h>
#include <string.h>

#ifdef RASPI
#include "raspi/displayRaspi.h"
#include "raspi/sprites.h"
#endif

#ifdef ALLEGRO
#include "allegro/allegro.h"
#endif

/*******************************************************************************************************************************************
 * 
                                        ___               _           _     _                   
                                        | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___
                                        |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<
                                        |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/
                                                                                |_|                                                            
 * 
 ******************************************************************************************************************************************/
void fillLeaderboardMenu(menu_t * menuLeaderboard);  //Se utiliza para llenar los campos del menu que muestra el leaderBoard.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                     ___                     _                   _              
                                    / __|  ___   _ _    ___ | |_   __ _   _ _   | |_   ___   ___
                                   | (__  / _ \ | ' \  (_-< |  _| / _` | | ' \  |  _| / -_) (_-<
                                    \___| \___/ |_||_| /__/  \__| \__,_| |_||_|  \__| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/

extern keys_t KEYS;

enum MENUES_VALUE { MENU_INICIO , MENU_PAUSA, MENU_WON_LEVEL, MENU_LOST_LEVEL, MENU_LEADERBOARD, MENU_LEVELS, MENU_VOLUME, START_LEVEL_MENU};//Determinan un valor para cada menu

enum PANTALLAS { MENU , SAVE_SCORE, START_LEVEL , IN_GAME, DESTROY_LEVEL, QUIT_GAME};//Determinan un valor para cada pantalla

extern menu_t* MENUES[10];//Arreglo que contiene punteros a todos los menues.

#define U_SEC2M_SEC 1000    //Conversion de micro segundos a milisegundos.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/
typedef void (*audioCallback_t)(int);  //Puntero a la funcion que ejecuta audio
typedef int (*volumeCallback_t)(int);  //Puntero a la funcion que regula el volumen del audio

typedef struct{//Contiene el estado del juego.

    unsigned char pantallaActual;//Indica si el juego se encuentra en partida o en un menu.
    unsigned char pantallaAnterior;//Indica el estado anterior del juego.
    unsigned char nivelActual;//Indica el nivel que esta en juego.
    unsigned char menuActual;//Indica el menu que esta corriendo.
    signed char menuAnterior;//Almacena temporalmente el menu anterior.
    unsigned char inGame;//Indica si se deben correr los threads que ejecutan el juego.
    unsigned char usrLives;//Vidas del usuario.
    unsigned char exitStatus;//Flag utilizado para saber cuando salir del programa. Si es 0 se debe salir del programa.
}gameStatus_t;

typedef struct {//Este struct contiene la informacion necesaria para ejecutar el juego.

	keys_t * keys;
    object_t** naveUsr;
    object_t** balasUsr;
    level_setting_t* levelSettings;
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
    int * scoreInstantaneo;//Esta variable almacena el score constantemente sin necesidad de ganar un nivel.
    audioCallback_t audioCallback;  //Puntero a la funcion que ejecuta audio
} game_t;

typedef struct {

    keys_t * keys;
    char* puntaje;//String que contiene el puntaje del usuario.
    int puntajeNumerico;//Contiene el puntaje numerico.
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
    int cantOpciones;//Cantidad de letras del usuario.
    audioCallback_t audioCallback;

} saveScore_t;

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                             ___         _                           
                                            | __| __ __ | |_   ___   _ _   _ _    ___
                                            | _|  \ \ / |  _| / -_) | '_| | ' \  (_-<
                                            |___| /_\_\  \__| \___| |_|   |_||_| /__/                                                                                                                      
 * 
 ******************************************************************************************************************************************/
extern gameStatus_t GAME_STATUS;

extern game_t menuGame;

extern menu_t menuInicio; 
extern menu_t menuPausa; 
extern menu_t menuWonLevel; 
extern menu_t menuLostLevel; 
extern menu_t menuLeaderboard; 
extern menu_t menuLevels;
extern menu_t menuVolume;

//extern char stringWithScore[20];

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                             __  __                              
                                            |  \/  |  __ _   __   _ _   ___   ___
                                            | |\/| | / _` | / _| | '_| / _ \ (_-<
                                            |_|  |_| \__,_| \__| |_|   \___/ /__/                                                    
 * 
 ******************************************************************************************************************************************/

#ifdef RASPI

#define INPUT_THREAD inputRPIThread             //Thread encargado de leer el input en la RPI
#define DISPLAY_THREAD_GAME displayRPIThread    //Thread encargado de actualizar el display durante la ejecucion del juego en la RPI
#define DISP_ANIM_MENU  textAnimMenu            //Thread encargado de actualizar el display durante un menu en la RPI.
#define SIGUIENTE ((menu->keys)->x == 1)
#define ANTERIOR ((menu->keys)->x == -1)
#define SIGUIENTESCORE ((menu->keys)->x == 1)
#define ANTERIORSCORE ((menu->keys)->x == -1)
#define ARRIBA_INPUT ((menu->keys)->y == 1)
#define ABAJO_INPUT  ((menu->keys)->y == -1)
#define ATRAS   ((menu->keys)->y == -1)

#endif


#ifdef ALLEGRO

#define INPUT_THREAD allegroThread
#define SIGUIENTE ((menu->keys)->y == -1)
#define ANTERIOR ((menu->keys)->y == 1)
#define SIGUIENTESCORE ((menu->keys)->x == 1)
#define ANTERIORSCORE ((menu->keys)->x == -1)
#define ARRIBA_INPUT ((menu->keys)->y == 1)
#define ABAJO_INPUT  ((menu->keys)->y == -1)
#define ATRAS   ((menu->keys)->x == -1)
#endif

#define DERECHA_INPUT ((menu->keys)->x == 1)    //Macros para detectar como se movio el joystick.
#define IZQUIERDA_INPUT  ((menu->keys)->x == -1)

#define PRESS_INPUT     ((menu->keys)->press == 1)    //Macro para detectar cuando se presiona para seleccionar una opcion en un menu.

#endif
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/
