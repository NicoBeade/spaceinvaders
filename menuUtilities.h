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
int selectPlayInicio(void);
int selectLevels(void);
int selectVolume(void);
int selectQuitGame(void);
int selectResume(void);
int selectRestartLevel(void);
int selectMainMenu(void);
int selectDificulty(void);
int selectLeaderboard(void);
int selectSaveScore(void);
int backMenuAnterior(void);
int selectNothing(void);
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

#ifdef RASPI
extern halfDisp_t halfDispTrophy;
extern halfDisp_t halfDispAlienSpaceInvaders;
extern halfDisp_t halfDispVolume;
extern halfDisp_t halfDispResume;
extern halfDisp_t halfDispRestart;
extern halfDisp_t halfDispAAA;
#endif

#define VEL_SHOOT_USR 15
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

extern int velDispAnimation;

extern sem_t SEM_DRIVER;

extern menu_t menuInicio; 
extern menu_t menuPausa; 
extern menu_t menuWonLevel; 
extern menu_t menuLostLevel; 
extern menu_t menuLeaderboard; 
extern menu_t menuLevels;

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

typedef struct {

    keys_t * keys;
    char* puntaje;//String que contiene el puntaje del usuario.
    int puntajeNumerico;//Contiene el puntaje numerico.
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
    int cantOpciones;//Cantidad de letras del usuario.
} saveScore_t;

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
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

#endif
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/