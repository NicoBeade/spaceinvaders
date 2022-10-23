/**********************************************************************************************************************************************************
 * 
                             _                           _     _____                          _       _     
                            (_)                         | |   |  __ \                        (_)     | |    
                             _   _ __    _ __    _   _  | |_  | |__) |   __ _   ___   _ __    _      | |__  
                            | | | '_ \  | '_ \  | | | | | __| |  _  /   / _` | / __| | '_ \  | |     | '_ \ 
                            | | | | | | | |_) | | |_| | | |_  | | \ \  | (_| | \__ \ | |_) | | |  _  | | | |
                            |_| |_| |_| | .__/   \__,_|  \__| |_|  \_\  \__,_| |___/ | .__/  |_| (_) |_| |_|
                                        | |                                          | |                    
                                        |_|                                          |_|
 * 
 ***********************************************************************************************************************************************************
 **********************************************************************************************************************************************************/

 #ifndef INPUT_RASPI_H
 #define INPUT_RASPI_H

#include "../utilidades.h"
#include "../spaceLib/spaceLib.h"
#include <semaphore.h>
/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/


/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                 ___               _           _     _                   
                                | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___
                                |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<
                                |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/
                                                                        |_|                                                            
 * 
 ******************************************************************************************************************************************/

void* inputRPIThread(void* argInputRPI);  //Se encarga de leer el input durante la ejecucion del juego.
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

//*************LIMITES DEL JOYSTICK
#define JOY_ACTIVE_NEG -50      //Coordenada en X a partir de la cual se detecta que se activo el joystick.
#define JOY_ACTIVE_POS  50

#define VEL_USR 5   //Determina que tan rapido podra mover la nave del usuario. La conversion es: si velUsuario = 1, entonces la nave se podra mover 
                    //cada 10mS. Para ejecutar que la nave se pueda mover cada 1s, velUsuario debe valer 100. Por defecto se mueve cada medio segundo.

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

extern unsigned int timerTick;   //Variable del timer utilizada para saber cuando se deben ejecutar los threads.
extern int velInput;    /*Determina que tan rapido se leera el input. La conversion es: si velInput = 1, entonces updateInputGame se ejecuta 
                                                                cada 10mS. Para ejecutar updateInputGame cada 1s velInput debe valer 100.*/

extern gameStatus_t GAME_STATUS;

extern sem_t SEM_GAME;
extern sem_t SEM_MENU;
extern sem_t SEM_DRIVER;
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

#define NAVE_USUARIO ((argUpdateInputGame_t*)argUpdateInputGame) -> naveUsuario //Acceso a la nave del usuario.

#define MOOVE_NAVE_USUARIO ((argUpdateInputGame_t*)argUpdateInputGame) -> moveNaveUsuario  //Acceso al callback que mueve la nave del usuario.

#define THREAD_DISPLAY_PAUSA ((argUpdateInputGame_t*)argUpdateInputGame) -> threadDisplayPausa  //Acceso al callback que mueve la nave del usuario.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

 #endif//inputRaspi.h
