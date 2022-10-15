/**********************************************************************************************************************************************************
 *                          _                           _                _   _                                     _     
                        (_)                         | |       /\     | | | |                                   | |    
                         _   _ __    _ __    _   _  | |_     /  \    | | | |   ___    __ _   _ __    ___       | |__  
                        | | | '_ \  | '_ \  | | | | | __|   / /\ \   | | | |  / _ \  / _` | | '__|  / _ \      | '_ \ 
                        | | | | | | | |_) | | |_| | | |_   / ____ \  | | | | |  __/ | (_| | | |    | (_) |  _  | | | |
                        |_| |_| |_| | .__/   \__,_|  \__| /_/    \_\ |_| |_|  \___|  \__, | |_|     \___/  (_) |_| |_|
                                    | |                                               __/ |                           
                                    |_|                                              |___/ 
 * 
 ***********************************************************************************************************************************************************
*   Este archivo contiene las funciones y threads encargadas de todas las acciones relacionadas a recibir input del usuario cuando el juego se ejecuta a.
*
 **********************************************************************************************************************************************************/

 #ifndef INPUT_ALLEGRO_H
 #define INPUT_ALLEGRO_H

 #include <allegro5/allegro.h>
 #include <allegro5/allegro_image.h>
 #include "utilidades.h"
 #include "displayAllegro.h"

 #define EVENTO (*(data->ev))


 typedef struct {

    ALLEGRO_EVENT_QUEUE ** event_queue;
    ALLEGRO_EVENT * ev; 

    keys_t * keys;

    bool * close_display;
    bool * keyboardDownFlag;
    bool * keyboardUpFlag;
    int * keycode;

} keyboard_data_t;


/*******************************************************************************************************************************************
 * 
                                 ___               _           _     _                   
                                | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___
                                |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<
                                |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/
                                                                        |_|                                                            
 * 
 ******************************************************************************************************************************************/

void * keyboardt(ALLEGRO_THREAD * thr, void * dataIn);

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


 #endif//inputAllegro.h