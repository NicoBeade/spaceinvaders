/**********************************************************************************************************************************************************
 * 
                             _                           _                _   _                                          
                            (_)                         | |       /\     | | | |                                         
                             _   _ __    _ __    _   _  | |_     /  \    | | | |   ___    __ _   _ __    ___         ___ 
                            | | | '_ \  | '_ \  | | | | | __|   / /\ \   | | | |  / _ \  / _` | | '__|  / _ \       / __|
                            | | | | | | | |_) | | |_| | | |_   / ____ \  | | | | |  __/ | (_| | | |    | (_) |  _  | (__ 
                            |_| |_| |_| | .__/   \__,_|  \__| /_/    \_\ |_| |_|  \___|  \__, | |_|     \___/  (_)  \___|
                                        | |                                               __/ |                          
                                        |_|                                              |___/    
 * 
 ***********************************************************************************************************************************************************
*   Este archivo contiene las funciones y threads encargadas de todas las acciones relacionadas a recibir input del usuario cuando el juego se ejecuta a.
*
 **********************************************************************************************************************************************************/

#include <stdio.h>
#include <time.h>
#include "aliensYBalas.h"
#include "inputAllegro.h"
#include "utilidades.h"
#include "displayAllegro.h"

/*
    LEFT 82
    RIGTH 83
    UP 84
    DOWN 85
    SPACE 75

*/

#define KEYCODE ALLEGRO_KEY_DOWN-*data->keycode

enum keycodes {DOWN, UP, RIGHT, LEFT, SPACE};

void moveNaveUsuario(object_t * naveUsuario, int desplazamiento, int xMax, int tamNaveX){
/* Esta funcion se llama como callback por los threads que manejan el input tanto en allegro como en la raspberry. Se encarga de actualizar
    la posicion de la nave del usuario.
*/

    if( !((naveUsuario -> pos.x == 0 && desplazamiento < 0) || ((naveUsuario -> pos.x == xMax - tamNaveX + 1) && desplazamiento > 0)) ){//Chequea que no este en los bordes.
        naveUsuario -> pos.x += desplazamiento;//Desplaza la nave
    }   
}

void * keyboardt(ALLEGRO_THREAD * thr, void * dataIn){

    keyboard_data_t * data = (keyboard_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    ALLEGRO_EVENT * evp = data->ev;

    bool key_pressed[5] = {false, false, false, false, false};

    al_install_keyboard();

    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while(!*data->close_display){

        if(*data->keyboardDownFlag){

            if(*data->keycode == ALLEGRO_KEY_SPACE){
                key_pressed[SPACE]= true;
            }     
            else{
                key_pressed[KEYCODE]= true;
            }

            *data->keyboardDownFlag= false;       
        }
        else if(*data->keyboardUpFlag){

            if(*data->keycode == ALLEGRO_KEY_SPACE){
                key_pressed[SPACE]= false;
            }     
            else{
                key_pressed[KEYCODE]= false;
            }

            *data->keyboardUpFlag= false;       
        }

        usleep(10000);

        if(key_pressed[UP]){


        }
        if(key_pressed[DOWN]){

            
        }
        if(key_pressed[RIGHT]){
            moveNaveUsuario(data->object, DESPLAZAMIENTO_X, X_MAX, NAVEX);
            
        }
        if(key_pressed[LEFT]){
            moveNaveUsuario(data->object, -DESPLAZAMIENTO_X, X_MAX, NAVEX);
        }
    } 
}