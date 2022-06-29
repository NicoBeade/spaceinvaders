#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "utilidades.h"
#include <time.h>

static char * images[]= {"sprites/alien1.png", 
                         "sprites/alien2.png", 
                         "sprites/alien3.png", 
                         "sprites/nave.png", 
                         "sprites/escudo.png"};


int showLista(object_t * inicial);
int showEntity(object_t * entity);

typedef struct {

    ALLEGRO_EVENT_QUEUE ** event_queue;  

    object_t * object;                     

    bool * close_display;
    bool * keybordFlag;
    bool * displayFlag; 

} eventH_data_t;


void * displayt (ALLEGRO_THREAD * thr, void * dataIn){

    eventH_data_t * data = (eventH_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    ALLEGRO_DISPLAY * display = NULL;

    display = al_create_display(640,480);

    al_init_image_addon();
    

    al_register_event_source(event_queue, al_get_display_event_source(display));

    while(!*data->close_display){

        if(*data->displayFlag){
            showEntity(data->object);

        }
    }

}

/*******************************************************/



void * eventHandler(ALLEGRO_THREAD * thr, void * dataIn){

    eventH_data_t * data = (eventH_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue; 

    ALLEGRO_EVENT ev;

    while(!*data->close_display){

        if (al_get_next_event(event_queue, &ev)) 
        {    
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                *data->close_display = true;
            }
            else if(ev.type == ALLEGRO_EVENT_TIMER){
                *data->displayFlag=true;
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                data->object->pos.x+=10;
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_UP){
                
            }

        }

    }

}

/*******************************************************/

void * keybordt(ALLEGRO_THREAD * thr, void * dataIn){

    eventH_data_t * data = (eventH_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    al_install_keyboard();
    
}


int main (void){

    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_THREAD * teventH, * tdisplay; 
    ALLEGRO_TIMER * timer = NULL;

    bool fail = false;
    bool close_display = false;
    bool keybordFlag = false;
    bool displayFlag = false; 

    object_t nave = {{100,100}, NAVE, 1, 1, 1, 1, NULL};

    eventH_data_t dataH = {&event_queue, &nave, &close_display, &keybordFlag, &displayFlag};

    al_init();
    timer = al_create_timer(1.0/60.0);
    event_queue = al_create_event_queue();
    
    
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    teventH = al_create_thread(eventHandler, &dataH);
    tdisplay = al_create_thread(displayt, &dataH);
    al_start_thread(teventH);
    al_start_thread(tdisplay);

    
    al_join_thread(teventH, NULL);
    al_join_thread(tdisplay, NULL);
    al_destroy_thread(teventH);
    al_destroy_thread(tdisplay);

}




int showEntity(object_t * entity){
  
    ALLEGRO_BITMAP * image = NULL;      //Se crea un bitmap donde guardar la imagen

    char * sprite = "sprites/nave.png";       //Se busca la direccion del sprite

    image = al_load_bitmap(sprite);    //Se carga en el bitmap

    //Si no se pudo cargar salta error
    if (!image) {
        fprintf(stderr, "failed to load image !\n");
        return -1;
    }

    al_draw_bitmap(image, entity->pos.x, entity->pos.y, 0);     //Se dibuja en el display

    al_flip_display();

    al_destroy_bitmap(image);       //Se eleimina la imagen

    return 0;
}

//      Show Lista

int showLista(object_t * inicial){

    //punter al primer objeto de la lista
    object_t * puntero = inicial;

    //Si se paso un NULL salta error
    if(puntero == NULL){
        return -1;
    }

    else {
        //Se muestra el primer objeto
        showEntity(puntero);

        //Se muestran los siguientes
        while(puntero->next != NULL){
            puntero = puntero->next;
            showEntity(puntero);
        }
    }
    
    return 0;
}