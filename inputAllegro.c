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
#include <semaphore.h>

/*
    LEFT 82
    RIGTH 83
    UP 84
    DOWN 85
    SPACE 75

*/

#define KEYCODE ALLEGRO_KEY_DOWN-*data->keycode

enum keycodes {DOWN, UP, RIGHT, LEFT, SPACE};

extern sem_t semaforo;

extern unsigned int timerTick;

#define FPS 5

void * keyboardt(ALLEGRO_THREAD * thr, void * dataIn){

    keyboard_data_t * data = (keyboard_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    bool key_pressed[6] = {false, false, false, false, false, false};

    al_install_keyboard();

    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while(!*data->close_display){

        usleep(10 * U_SEC2M_SEC);

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

        if(timerTick % FPS == 0 ){

            if(key_pressed[UP]){
                sem_wait(&semaforo);
                *data->punteros->balasUsuario = shootBala(data->punteros->nave, *data->punteros->balasUsuario, data->levelSettings);
                key_pressed[UP]=false;
                sem_post(&semaforo);
            }
            if(key_pressed[DOWN]){
                
            }
            if(key_pressed[RIGHT]){
                moveNaveUsuario(data->punteros->nave, DESPLAZAMIENTO_X, X_MAX, NAVEX);
                
            }
            if(key_pressed[LEFT]){
                moveNaveUsuario(data->punteros->nave, -DESPLAZAMIENTO_X, X_MAX, NAVEX);
            }

        }
    }

    pthread_exit(0); 
}