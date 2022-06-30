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
*   Este archivo contiene las funciones y threads encargadas de todas las acciones relacionadas a recibir input del usuario cuando el juego se ejecuta en
*   la raspberry.
 **********************************************************************************************************************************************************/

 #ifndef INPUT_RASPI_H
 #define INPUT_RASPI_H

#include "utilidades.h"
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
typedef struct{//Este es el tipo de dato que recibe el thread de updateInputGame
    object_t * naveUsuario;//Necesita un puntero a la lista de la nave del usuario.
    //object_t * (*fireUsuario)(object_t *);//Puntero a la funcion que dispara una bala del usuario.
    void (*moveNaveUsuario)(object_t * naveUsuario, int desplazamiento, int xMax, int tamAliensX);//Puntero a la funcion que se encarga de modificar la posicion del usuario.
    int xMax;//Limites de la pantalla.
    int tamXNave;//Tamano en x de la nave.
    int desplazamiento;//India de a cuantos saltos en la coordenada X se mueve la nave.
    void * (*threadDisplayPausa)(void*);//Puntero al thread que gestiona la pausa en el display.
}argUpdateInputGame_t;

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
void* updateInputGame(void* argUpdateInputGame);  //Se encarga de leer el input durante la ejecucion del juego.

void* inputMenu(void* punteroOpciones);          //Se encarga de leer el input durante la pausa.
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
extern sem_t semaforo;
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

#define X_MAX ((argUpdateInputGame_t*)argUpdateInputGame) -> xMax   //Aceceso a la coordenada maxima de x.

#define TAM_X_NAVE ((argUpdateInputGame_t*)argUpdateInputGame) -> tamXNave  //Acceso al tamano de la nave en la coordenada X.

#define DESPLAZAMIENTO ((argUpdateInputGame_t*)argUpdateInputGame) -> desplazamiento //Acceso al desplazamiento.

#define MOOVE_NAVE_USUARIO ((argUpdateInputGame_t*)argUpdateInputGame) -> moveNaveUsuario  //Acceso al callback que mueve la nave del usuario.

#define THREAD_DISPLAY_PAUSA ((argUpdateInputGame_t*)argUpdateInputGame) -> threadDisplayPausa  //Acceso al callback que mueve la nave del usuario.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

 #endif//inputRaspi.h
