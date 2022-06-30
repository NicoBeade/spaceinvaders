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
#include "disdrv.h"
#include "termlib.h"
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

typedef struct{ //argumentos a recibir por el thread del display en juego RPI
    object_t* balas;
    object_t* aliens;
    object_t* naveUser;
    //faltan los punteros a barreras y user
}argDisplayRPI_t;

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

enum OPTIONSPAUSA = {RESUME, VOLUMEN, HOME, RESTART, SCORE};

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
extern sem_t semaforo; 

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

void drawSprite(dcoord_t, sprite_t); //prototipos  de dibujar y limpiar enemigos
void cleanSprite(dcoord_t);
void* displayRPI (void* argDisplayRPI); //prototipo del thread del display del juego en RPI
void clearBuffer(void); //borra los contenidos del buffer del display sin eliminar el del display
void* dispMenu(void* punteroPausa);  //Thread encargado de gestionar el display durante la pausa.
void printLetter(caracteres_t letter); //imprime letra en display
void printFullDisp(fullDisp_t displaySprite); //imprime todo el display

//*****************OPTION HANDLER PAUSA
void changeOptionPausa(int actualOption);//Se encarga de mostrar la opcion indicada en el display cuanto el juego esta pausado
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

#endif