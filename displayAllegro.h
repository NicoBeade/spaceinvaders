

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

#include "utilidades.h"

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
    object_t * aliens;
    object_t * nave;
    object_t * barrera;
    object_t * balasUsuario;
    object_t * balasAliens;

} display_data_t;

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

void * display(void * arg);

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

#define X_MAX 640              //Ancho maximo de la pantalla. Vamos a tomar la primer posicion como 0.
#define Y_MAX 480              //Alto maximo de la pantalla

#define MARGEN_X 4          //Margen horizontal en el display
#define MARGEN_Y 1          //Margen vertical en el display

#define DESPLAZAMIENTO_X 1  //Indica cuanto de debe mover una nave en la coordenada x.
#define DESPLAZAMIENTO_Y 1  //Indica cuanto se debe mover una nave en la coordenada y.

#define BULLET_UP -1         //Indica cuanto se debe mover una bala aliada en Y
#define BULLET_DOWN 1      //Indica cuanto se debe mover una bala enemiga en Y

#define ESP_ALIENS_X 1      //Espacio vacio entre los aliens en la coordenada X.
#define TAM_ALIENS_X 3      //Tamano que ocupan los aliens en la coordenada X.
#define ESP_ALIENS_Y 1      //Espacio vacio entre los aliens en la coordenada Y.
#define TAM_ALIENS_Y 3      //Tamano que ocupan los aliens en la coordenada Y.

#define DIST_INICIAL_X 6    //Distancia de los aliens respecto del borde izquierdo de la pantalla al iniciar un nivel, sin offset por nave
#define DIST_INICIAL_Y 4    //Distancia de los aliens respecto del borde superior de la pantalla al iniciar un nivel, sin offset por nave

#define CANT_MAX_ALIENS 12  //Cantidad maxima de aliens, dependiendo del nivel se muestra distinta cantidad, con este tope.
#define CANT_MAX_FILAS 4    //CAntidad maxima de filas, dependiendo del nivel se muestra una cantidad distinta, con este tope.

#define BGCOLOR 0,0,0       //Color del background (negro)

#define REFRESHRATE 2       //Taza de refresco de imagen

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

#endif //display.h