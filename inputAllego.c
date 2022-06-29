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
#include <allegro5/allegro.h>
#include <pthread.h>
#include <stdio.h>

#include "inputAllegro.h"

static enum MYKEYS { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

void * keybord(void * dataIn){
    
    keybord_data_t * data = (keybord_data_t *) dataIn; 

    ALLEGRO_EVENT_QUEUE * event_queue = data->event_queue;

    bool fail = false;
    bool key_pressed[4] = {false, false, false, false};

    /**********************************************************************************************************
     *  
     *                                          INICIALIZACION
     * 
     * *******************************************************************************************************/

    if (!al_install_keyboard() && !fail) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        fail = true;
    }

    //Si falla la inicializacion

    if (fail){
        pthread_exit(NULL);
    }

    //Se registra el display en la cola de eventos

    al_register_event_source(data->event_queue, al_get_keyboard_event_source());

    while (!data->close_display) {

        if (data->keybordFlag){
           
            if (EVENTO.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (EVENTO.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key_pressed[KEY_UP] = true;
                        printf("hola");
                        break;

                    case ALLEGRO_KEY_DOWN:
                        key_pressed[KEY_DOWN] = true;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        key_pressed[KEY_LEFT] = true;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key_pressed[KEY_RIGHT] = true;
                        break;
                }
            }

            else if (EVENTO.type == ALLEGRO_EVENT_KEY_UP) {
                switch (EVENTO.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key_pressed[KEY_UP] = false;
                        break;

                    case ALLEGRO_KEY_DOWN:
                        key_pressed[KEY_DOWN] = false;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        key_pressed[KEY_LEFT] = false;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key_pressed[KEY_RIGHT] = false;
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        *data->close_display = true;
                        break;
                }
            }

            *data->keybordFlag = false;
        }

    }

    pthread_exit(NULL);
}