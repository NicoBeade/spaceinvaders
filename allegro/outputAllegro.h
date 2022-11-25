/**********************************************************************************************************************************************************
 * 
                         _   _                 _                              _   _                                     _     
                        | | (_)               | |                     /\     | | | |                                   | |    
                      __| |  _   ___   _ __   | |   __ _   _   _     /  \    | | | |   ___    __ _   _ __    ___       | |__  
                     / _` | | | / __| | '_ \  | |  / _` | | | | |   / /\ \   | | | |  / _ \  / _` | | '__|  / _ \      | '_ \ 
                    | (_| | | | \__ \ | |_) | | | | (_| | | |_| |  / ____ \  | | | | |  __/ | (_| | | |    | (_) |  _  | | | |
                     \__,_| |_| |___/ | .__/  |_|  \__,_|  \__, | /_/    \_\ |_| |_|  \___|  \__, | |_|     \___/  (_) |_| |_|
                                      | |                   __/ |                             __/ |                           
                                      |_|                  |___/                             |___/                

 *                              
 ***********************************************************************************************************************************************************
*   Este .h contiene tipos de datos y constantes que son utilizados para el manejo del display 
*
 **********************************************************************************************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../utilidades.h"
#include "allegro.h"

/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/

    /**************************************************************************
     Datos que recibe el thread de display
        Recibe 5 punteros: uno a la lista de aliens
                        uno al usuario
                        uno a la barrera
                        uno a la lista de las balas del usuario
                        uno a la lista de las balas de los aliens
    ***************************************************************************/

typedef struct 
{
    ALLEGRO_EVENT_QUEUE ** event_queue;

    punteros_t punteros;
    texto_t ** text;

    bool * close_display;
    bool * displayFlag;

}output_data_t;
    
enum {smallF, mediumF, largeF, bigF, FONTMAX};

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

    /**************************************************************************
     THREAD DISPLAY
        Este thread de encarga de mostrar en pantalla las entidades del juego
        Recibe como parametro un puntero a los datos guardados en el struct
        display_data_t (explicado arriba)
        
    ***************************************************************************/

void * displayt (ALLEGRO_THREAD * thr, void * dataIn);

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

#define BGHEIGHT 768
#define BGVEL 5
#define TAMLETRAX 24
#define TAMLETRAY 36

#define X_MAX 900              //Ancho maximo de la pantalla. Vamos a tomar la primer posicion como 0.
#define Y_MAX 760              //Alto maximo de la pantalla

#define BGCOLOR 0,0,0       //Color del background (negro)


/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

#endif //display.h