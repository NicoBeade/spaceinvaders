/**********************************************************************************************************************************************************
 * 
                                _   _               _                 ___                      _       _    
                             __| | (_)  ___  _ __  | |  __ _   _  _  | _ \  __ _   ___  _ __  (_)     | |_  
                            / _` | | | (_-< | '_ \ | | / _` | | || | |   / / _` | (_-< | '_ \ | |  _  | ' \ 
                            \__,_| |_| /__/ | .__/ |_| \__,_|  \_, | |_|_\ \__,_| /__/ | .__/ |_| (_) |_||_|
                                            |_|                |__/                    |_|              
                                                                                            
 * 
 ***********************************************************************************************************************************************************
*   Este .h contiene tipos de datos, externs y prototipos para mostrar en el display de la raspi el estado del juego.
*
 **********************************************************************************************************************************************************/

#ifndef DISPLAY_RASPI_H
#define DISPLAY_RASPI_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "drivers/disdrv.h"
#include "drivers/termlib.h"
#include "sprites.h"
#include "../spaceLib/spaceLib.h"
#include "../spaceLib/score/score.h"
#include <semaphore.h>
#include <string.h>

/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/

typedef struct{ //argumentos a recibir por el thread del display en juego RPI
    object_t** balasEnemigas; //puntero a las distintas listas
    object_t** balasUsr;
    object_t** aliens;
    object_t** naveUser;
    object_t** barriers;
    object_t** mothership;
}argDisplayRPI_t;

typedef struct{//Argumentos que recibe el thread de la animacion de barrido.
    char* msg;//Mensaje a mostrar
    halfDisp_t* lowerDispMenu;//Contenido de la parte inferior del display.
    halfDisp_t* higherDispMenu;//Contenido de la parte superior del display.
    halfDisp_t* drawing;//Dibujo a mostrar en la opcion seleccionada.
    int direccion;//Direccion en la que se debe mostrar el primer barrido.
    int* changeAnimation;//Indica cuando salir del thread.
    char* menuActual;//Indica que menu se esta ejecutando.
}argTextAnimMenu_t;

typedef struct{//Argumentos que recibe el thread que muestra el dibujo de cada opcion.
    halfDisp_t* higerDispMenu;//parte superior del display.
    int direccion;
    halfDisp_t* drawing;//Dibujo a mostrar.
}argSwipeDrawing_t;

typedef struct{//Argumentos que recibe la funcion changeOption.
    pthread_t* threadMenu;//Puntero al thread
    int* animStatus;//Indicador de si se debe cambiar de texto
    halfDisp_t* lowerDispMenu;//Puntero a la parte inferior del display durante la ejecucion del menu.
    halfDisp_t* higherDispMenu;//Contenido de la parte superior del display.
    halfDisp_t* drawing;//Dibujo a mostrar en la opcion seleccionada.
    char* nuevoTexto;//Nuevo texto a mostrar en pantalla.
    int direccion;//Direccion de la animacion.
    char* menuActual;//Indica que menu se esta ejecutando.
}argChangeOption_t;

typedef int (*option_t)(void);//Punteros a funcion utilizadas en los menues. Se utilizan para realizar las acciones necesarias al seleccionar una opcion en un menu.

typedef struct {//Este struct contiene la informacion necesaria para ejecutar un menu.

	keys_t * keys;
	option_t selectOption[20];//Struct que contiene punteros a funciones que indican que hacer cuando se selecciona una opcion.
    option_t backMenuAnterior;//Puntero a la funcion que vuelve un menu hacia atras.
    char textOpciones[20][50];//Arreglo de punteros a los strings que contienen el texto de cada opcion.
    halfDisp_t* drawingOpciones[20];//Arreglo de punteros a los dibujos que se deben mostrar en cada opcion.
    int cantOpciones;//Cantidad de opciones del menu.
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
    void (*changeOption)(void* argChangeOption);//Callback a la funcion que cambia la opcion seleccionada.
    void (*audioCallback)(int);//Callback a la funcion que ejecuta los sonidos.
} menu_t;

typedef struct {

    char* letra;//Apunta a la letra que se debe mostrar.
    halfDisp_t* display;//Apunta a medio display donde se mostrara la letra.
    vector_t* pos;//Posicion de donde se hara titilar la letra.
    char* exitStatus;//Flag que indica cuando salir del thread.
} letterFlash_t;

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

#define VEL_DISP_ANIMATION 3000

#define OFFSETLETRA 0
#define OFFSETALIEN 39
#define OFFSETCHARESP 36
#define OFFSETNUM 26

#define VEL_TITILEO 200

#define MAX_SPRITES 200

#define IZQUIERDA -1        //Constantes utilizadas para indicar la direccion en la que se deben mover los aliens.
#define DERECHA 1
#define ABAJO 2

#define U_SEC2M_SEC 1000    //Conversion de micro segundos a milisegundos.
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

extern unsigned int timerTick;   //Variables utilizadas para saber cuando se deben ejecutar los threads.

extern keys_t KEYS;
extern gameStatus_t GAME_STATUS;

extern sem_t SEM_GAME;
extern sem_t SEM_MENU; 
extern sem_t SEM_DRIVER;

extern int velDispAnimation;

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

//*****************THREAD DISPLAY IN GAME
void* displayRPIThread (void* argDisplayRPI); //prototipo del thread del display del juego en RPI

//*****************THREAD DISPLAY DURANTE MENUES
void* textAnimMenu(void* argTextAnimMenu); //Se encarga de realizar la animacion de barrido de los textos durante la ejecucion de un menu.
void changeOption(void* argChangeOption); //Cambia el texto mostrado en pantalla.

//*****************SCORE
halfDisp_t* getLeaderBoardName(halfDisp_t* nameDispMenu, int select);  //Se utiliza para mostrar el nombre del usuario del leaderBoard en el menu.
halfDisp_t* strToHalfDisplay(halfDisp_t * nombre, char* nombreStr);  //Adapte el nombre del usuario a un halfdisplay_t.
void* letterFlashThread(void*); //Thread encargado de hacer titilar una letra en el display.
void barridoLetra(char, char, int sentido, dcoord_t); //Barrido vertical de letra

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

#endif