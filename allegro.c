#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "utilidades.h"
#include "displayAllegro.h"
#include "inputAllegro.h"


typedef struct {

    ALLEGRO_EVENT_QUEUE ** event_queue; 
    ALLEGRO_EVENT * ev; 

    object_t * object;    
    
    bool * keyboardDownFlag;
    bool * keyboardUpFlag;
    int * keycode;
    bool * displayFlag; 
    bool * close_display;

} eventH_data_t;

/*******************************************************/

void * eventHandler(ALLEGRO_THREAD * thr, void * dataIn){

    eventH_data_t * data = (eventH_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue; 

    ALLEGRO_EVENT * evp = data->ev;

    while(!*data->close_display){

        if (al_get_next_event(event_queue, evp)) 
        {    
            if (evp->type  == ALLEGRO_EVENT_DISPLAY_CLOSE){
                *data->close_display = true;
            }
            else if(evp->type  == ALLEGRO_EVENT_TIMER){
                *data->displayFlag=true;
            }
            else if (evp->type == ALLEGRO_EVENT_KEY_DOWN){
                *data->keycode = evp->keyboard.keycode;
                *data->keyboardDownFlag=true;
            }
            else if (evp->type == ALLEGRO_EVENT_KEY_UP){
                *data->keycode = evp->keyboard.keycode;
                *data->keyboardUpFlag=true;
            }

        }

    }

}

/*******************************************************/

int main (void){

    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_THREAD * teventH, * tdisplay, * tkeyboard; 
    ALLEGRO_TIMER * timer = NULL;

    bool fail = false;
    bool close_display = false;
    bool keybordDownFlag = false;
    bool keybordUpFlag = false;
    int keycode = 0;
    bool displayFlag = false; 

    object_t alien2 = {{400,100}, DANIEL, 1, 1,  NULL};
    object_t alien = {{300,100}, DANIEL, 1, 1,  &alien2};
    object_t nave = {{X_MAX/2 - NAVEX, Y_MAX - NAVEY - 10}, NAVE, 1, 1,  &alien};

    eventH_data_t dataH = {&event_queue, &ev, &nave, &keybordDownFlag, &keybordUpFlag, &keycode, &displayFlag, &close_display};
    display_data_t dataD = {&event_queue, &nave, &close_display, &displayFlag};
    keyboard_data_t dataK = {&event_queue, &ev, &nave, &close_display, &keybordDownFlag, &keybordUpFlag, &keycode};

    al_init();
    timer = al_create_timer(1.0/60.0);
    event_queue = al_create_event_queue();
    
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    teventH = al_create_thread(eventHandler, &dataH);
    tdisplay = al_create_thread(displayt, &dataD);
    tkeyboard = al_create_thread(keyboardt, &dataK);
    al_start_thread(teventH);
    al_start_thread(tdisplay);
    al_start_thread(tkeyboard);

    
    al_join_thread(teventH, NULL);
    al_join_thread(tdisplay, NULL);
    al_join_thread(tkeyboard, NULL);
    al_destroy_thread(teventH);
    al_destroy_thread(tdisplay);
    al_destroy_thread(tkeyboard);

}
