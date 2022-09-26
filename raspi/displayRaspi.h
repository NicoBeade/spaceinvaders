/**********************************************************************************************************************************************************
 * 
                                _   _               _                 ___                      _       _    
                             __| | (_)  ___  _ __  | |  __ _   _  _  | _ \  __ _   ___  _ __  (_)     | |_  
                            / _` | | | (_-< | '_ \ | | / _` | | || | |   / / _` | (_-< | '_ \ | |  _  | ' \ 
                            \__,_| |_| /__/ | .__/ |_| \__,_|  \_, | |_|_\ \__,_| /__/ | .__/ |_| (_) |_||_|
                                            |_|                |__/                    |_|              
                                                                                            
 * 
 ***********************************************************************************************************************************************************
*   Este .h contiene tipos de datos, externs y prototipos para mostrar en el display de la raspi el estado del juego
*
 **********************************************************************************************************************************************************/

#ifndef DISPLAY_RASPI_H
#define DISPLAY_RASPI_H

#include <stdint.h>
#include "drivers/disdrv.h"
#include "drivers/termlib.h"
#include <semaphore.h>
#include "../utilidades.h"

/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/

typedef const uint8_t sprite_t [2][3]; //matriz de 2x3, tamanyo de los enemigos
typedef const uint8_t fullDisp_t [16][16]; //matriz de 16x16, para imprimir sobre todo el display
typedef const uint8_t caracteres_t [8][4]; //matriz de 8x4 para imprimir letras
typedef uint8_t halfDisp_t [8][16]; //matriz de 8x16 para imprimir sobre la mitad del display

typedef struct{ //argumentos a recibir por el thread del display en juego RPI
    object_t** balasEnemigas;
    object_t** balasUsr;
    object_t** aliens;
    object_t** naveUser;
    //faltan los punteros a barreras y user
}argDisplayRPI_t;

typedef struct{//Argumentos que recibe el thread de la animacion de barrido.
    char* msg;//Mensaje a mostrar
    halfDisp_t* lowerDispMenu;//Contenido de la parte inferior del display.
    halfDisp_t* higherDispMenu;//Contenido de la parte superior del display.
    halfDisp_t* drawing;//Dibujo a mostrar en la opcion seleccionada.
    int direccion;//Direccion en la que se debe mostrar el primer barrido.
    int* changeAnimation;//Indica cuando salir del thread.
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
}argChangeOption_t;

typedef struct {//Este struct contiene la informacion necesaria para ejecutar un menu.

	keys_t * keys;
	option_t selectOption[10];//Struct que contiene punteros a funciones que indican que hacer cuando se selecciona una opcion.
    char* textOpciones[10];//Arreglo de punteros a los strings que contienen el texto de cada opcion.
    halfDisp_t* drawingOpciones[10];//Arreglo de punteros a los dibujos que se deben mostrar en cada opcion.
    int cantOpciones;//Cantidad de opciones del menu.
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
    void (*changeOption)(void* argChangeOption);//Callback a la funcion que cambia la opcion seleccionada.
} menu_t;

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

//*************CANTIDAD DE OPCIONES
#define CANT_OPCIONES_PAUSA 3   //Determina la cantidad total de opciones en el menu de pausa

enum OPTIONSPAUSA {RESUME, VOLUMEN, HOME, RESTART, SCORE};

#define VEL_DISP_ANIMATION 3000

#define OFFSETLETRA 0
#define OFFSETALIEN 39
#define OFFSETCHARESP 36
#define OFFSETNUM 26

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

extern menu_t* MENUES[10];//Arreglo que contiene punteros a todos los menues. No tiene por que estar definido aca, solo lo cree para hacer algo de codigo.
extern level_setting_t* LEVELS[10];//Arrego que contiene punteros a la config de todos los niveles.

extern sem_t SEM_GAME;
extern sem_t SEM_MENU; 

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
//*****************FUNCIONES GENERICAS DEL DISPLAY
void drawSprite(dcoord_t, sprite_t); //prototipos  de dibujar y limpiar enemigos
void cleanSprite(dcoord_t);
void clearBuffer(void); //borra los contenidos del buffer del display sin eliminar el del display
void printLetter(caracteres_t letter); //imprime letra en display
void printFullDisp(fullDisp_t displaySprite); //imprime todo el display
void printHalfDisp(halfDisp_t halfDispSprite, char mitad); //Imprime una mitad del display.

//*****************THREAD DISPLAY IN GAME
void* displayRPIThread (void* argDisplayRPI); //prototipo del thread del display del juego en RPI

//*****************THREAD DISPLAY DURANTE MENUES
void* textAnimMenu(void* argTextAnimMenu); //Se encarga de realizar la animacion de barrido de los textos durante la ejecucion de un menu.
void* swipeDrawing(void*);//Muestra un dibujo en la parte superior del display.
void changeOption(void* argChangeOption); //Cambia el texto mostrado en pantalla.


//*****************MENU DE INICIO
int selectPlayInicio(void);
int selectLevelsInicio(void);
int selectVolumeInicio(void);
int selectQuitGameInicio(void);
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

#endif