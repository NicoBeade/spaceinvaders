/**********************************************************************************************************************************************************
 * 
                                         _     _   _   _       _               _                    _     
                                        | |   (_) | | (_)     | |             | |                  | |    
                                 _   _  | |_   _  | |  _    __| |   __ _    __| |   ___   ___      | |__  
                                | | | | | __| | | | | | |  / _` |  / _` |  / _` |  / _ \ / __|     | '_ \ 
                                | |_| | | |_  | | | | | | | (_| | | (_| | | (_| | |  __/ \__ \  _  | | | |
                                 \__,_|  \__| |_| |_| |_|  \__,_|  \__,_|  \__,_|  \___| |___/ (_) |_| |_|
                                                                                            
 * 
 ***********************************************************************************************************************************************************
*   Este .h contiene tipos de datos y constantes que son utilizados en varios archivos del programa.
*
 **********************************************************************************************************************************************************/

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdint.h>
#include <pthread.h>
#include "spaceLib/spaceLib.h"

/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/
//typedef enum objectTypes {DANIEL, PABLO, NICOLAS, NAVE, BARRERA, BALA_DANIEL, BALA_PABLO, BALA_NICOLAS, BALA_USUARIO, BARRERA_ESQUINA_SUP_IZQ, BARRERA_ESQUINA_SUP_DER, BARRERA_INTERNO, BARRERA_ESQUINA_INF_IZQ, BARRERA_ESQUINA_INF_DER, NONE} types_t;

typedef struct{//Contiene el estado del juego.

    unsigned char pantallaActual;//Indica si el juego se encuentra en partida o en un menu.
    unsigned char nivelActual;//Indica el nivel que esta en juego.
    unsigned char menuActual;//Indica el menu que esta corriendo.
    unsigned char exitStatus;//Flag utilizado para saber cuando salir del programa. Si es 0 se debe salir del programa.
}gameStatus_t;

typedef struct {//Este struct se utiliza para obtener la entrada del usuario.

	int x;
	int y;
	uint8_t press;
} keys_t;

typedef int (*option_t)(void);//Punteros a funcion utilizadas en los menues. Se utilizan para realizar las acciones necesarias al seleccionar
                                //una opcion en un menu.

typedef struct {//Este struct contiene la informacion necesaria para ejecutar el juego.

	keys_t * keys;
    object_t** naveUsr;
    level_setting_t* levelSettings;
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
} game_t;

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

//*************DIRECCIONES
#define IZQUIERDA -1        //Constantes utilizadas para indicar la direccion en la que se deben mover los aliens.
#define DERECHA 1
#define ABAJO 2

//*************TIMER
#define U_SEC2M_SEC 1000    //Conversion de micro segundos a milisegundos.

//*************VELOCIDAD DE EJECUCION
#define USER_SPEED 5    //Determina que tan rapido se leera el input. La conversion es: si velInput = 1, entonces updateInputGame se ejecuta
                        //cada 10mS. Para ejecutar updateInputGame cada 1s velInput debe valer 100.

//*************PANTALLAS
enum PANTALLAS { MENU , START_LEVEL , IN_GAME, DESTROY_LEVEL};//Determinan un valor para cada pantalla

//*************MENUES
enum MENUES_VALUE { MENU_INICIO , MENU_PAUSA};//Determinan un valor para cada pantalla
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


/******************************************************************************************************************************************
*******************************************************************************************************************************************/


 #endif//utilidades.h