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

#include "inputAllegro.h"
#include "utilidades.h"

void * keyboardt(ALLEGRO_THREAD * thr, void * dataIn){

    keyboard_data_t * data = (keyboard_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    ALLEGRO_EVENT * evp = data->ev;

    bool key_pressed[4] = {false, false, false, false};

    al_install_keyboard();

    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while(!*data->close_display){

        if(*data->keyboardFlag){
            
            switch ((*evp).keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    data->object->type = NAVE;
                    break;

                case ALLEGRO_KEY_DOWN:
                    data->object->type = DANIEL;
                    break;

                case ALLEGRO_KEY_LEFT:
                    data->object->pos.x -=10;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    data->object->pos.x +=10;
                    break;
            }     

            *data->keyboardFlag= false;       
        }
    } 
}