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

/***********************************************************************************************************************************************************
 * 
 *                                                                      THREAD PRINCIPAL
 * 
 * ********************************************************************************************************************************************************/

void * keyboardt (ALLEGRO_THREAD * thr, void * dataIn){

    keyboard_data_t * data = (keyboard_data_t *) dataIn;        //Se obtiene la informacion de entrada

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;    //Se toma la cola de eventos de la entrada

    bool key_pressed[5] = {false, false, false, false, false};  //Array que almacena las teclas precionadas

    al_install_keyboard();                                      //Se intala lo necesario para el teclado

    al_register_event_source(event_queue, al_get_keyboard_event_source());  //Se agrega el teclado a la cola de eventos

    while(!*data->close_display){   //Mientras no se haya cerrado el display 

        if(*data->keyboardDownFlag || *data->keyboardUpFlag){       //Si se detecto una tecla

            switch (*data->keycode)                                 //Se almacena la tecla presionada
                {
                case ALLEGRO_KEY_SPACE:
                    key_pressed[SPACE]= *data->keyboardDownFlag;    //Espacio
                    break;
                case ALLEGRO_KEY_DOWN:
                    key_pressed[DOWN]= *data->keyboardDownFlag;     //Tecla hacia abajo
                    break;
                case ALLEGRO_KEY_UP:
                    key_pressed[UP]= *data->keyboardDownFlag;       //Tecla hacia arriba
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key_pressed[RIGHT]= *data->keyboardDownFlag;    //Tecla hacia la derecha
                    break;
                case ALLEGRO_KEY_LEFT:
                    key_pressed[LEFT]= *data->keyboardDownFlag;     //Tecla hacia la izquierda
                    break;
                default:
                    break;
                }

            *data->keyboardDownFlag= false;    //Se reinician los flags
            *data->keyboardUpFlag= false;       
        }

        if(timerTick % FPS == 0 ){             //Cada cierto tiempo se envian las teclas precionadas al main
                                               //A traves del puntero keys
            if(key_pressed[UP]){
                (*data->keys).y = 1;
            }
            else if(key_pressed[DOWN]){
                (*data->keys).y = -1;
            }
            else{
                (*data->keys).y = 0;
            }

            if(key_pressed[RIGHT]){
                (*data->keys).x = 1; 
            }
            else if(key_pressed[LEFT]){
                (*data->keys).x = -1;
            }
            else{
                (*data->keys).x = 0;
            }
            
            if(key_pressed[SPACE]){
                (*data->keys).press = 1;
                key_pressed[SPACE] = false;
            }

        }
    }

    pthread_exit(0); 
}