/**********************************************************************************************************************************************************
 *
                          _   _                 _                                _   _                               
                         | | (_)               | |                       /\     | | | |                              
                       __| |  _   ___   _ __   | |   __ _   _   _       /  \    | | | |   ___    __ _   _ __    ___  
                      / _` | | | / __| | '_ \  | |  / _` | | | | |     / /\ \   | | | |  / _ \  / _` | | '__|  / _ \ 
                     | (_| | | | \__ \ | |_) | | | | (_| | | |_| |    / ____ \  | | | | |  __/ | (_| | | |    | (_) |
                      \__,_| |_| |___/ | .__/  |_|  \__,_|  \__, |   /_/    \_\ |_| |_|  \___|  \__, | |_|     \___/ 
                                       | |                   __/ |                               __/ |               
                                       |_|                  |___/                               |___/                                                       
 * 
 ***********************************************************************************************************************************************************
*   Este archivo contiene las funciones y threads encargadas de mostrar en pantalla el juego a traves de la libreria Allegro
*
 **********************************************************************************************************************************************************/

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <pthread.h>
#include <stdio.h>
#include "displayAllegro.h"

//En esta array se guardan las direcciones a los sprites
static char * images[]= {"sprites/alien1.png", 
                         "sprites/alien2.png", 
                         "sprites/alien3.png", 
                         "sprites/nave.png", 
                         "sprites/escudo.png"};

//Timer tick
extern int timerTick = 1000000;

/***********************************************************************************************************************************************************
 * 
 *                                                                      PROTOTIPOS DE FUNCIONES LOCALES
 * 
 * ********************************************************************************************************************************************************/


    /******************************************************************************
     *  SHOW ENTITY
     *      Esta funcion recibe como unico parametro un puntero a un object_t
     *      y se encarga de mostrarlo en pantalla
     * 
     * ***************************************************************************/

int showEntity(object_t * entity);

    /******************************************************************************
     *  SHOW LISTA
     *      Esta funcion recibe como unico parametro un puntero al primer elemento
     *      de una lista de objetos (object_t) y los imprime en pantalla 
     * 
     * ***************************************************************************/

int showLista(object_t * inicial);

/***********************************************************************************************************************************************************
 * 
 *                                                                      THREAD PRINCIPAL
 * 
 * ********************************************************************************************************************************************************/

void * display(void * arg) {
    
    //      OBJETOS DE ALLEGRO

    ALLEGRO_DISPLAY * display = NULL;           //Display
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;   //Cola de eventos
    ALLEGRO_EVENT ev;                           //Evento

    //Se castea el puntero de datos y se almacena en data
    display_data_t * data = (display_data_t *) arg;

    //      FLAGS

    bool close_display = false;         //Cuando el display se cierra se prende
    bool fail= false;                   //Cuando la inicializacion de allegro falla se prende

    /**********************************************************************************************************
     *  
     *                                          INICIALIZACION
     * 
     * *******************************************************************************************************/

    //inicilizacion de allegro

    if (!al_init()) { 
        fprintf(stderr, "failed to initialize allegro!\n");
        fail= true;
    }
 
    //inicializacion de la cola de eventos
    
    if (!(event_queue = al_create_event_queue()) && !fail) {
        fprintf(stderr, "failed to create event_queue!\n");
        fail= true;
    }

    //inicializacion para las imagenes

    if (!al_init_image_addon() && !fail) {  
        fprintf(stderr, "failed to initialize image addon !\n");
        fail = true;
    }

    //inicializacion del display

    if (!(display = al_create_display(X_MAX, Y_MAX)) && !fail) {
        fprintf(stderr, "failed to create display!\n");     
        fail = true;
    }

    //Si falla la inicializacion

    if (fail){
        al_destroy_event_queue(event_queue);
        al_shutdown_image_addon();
        pthread_exit(NULL);
    }

    //Se registra el display en la cola de eventos

    al_register_event_source(event_queue, al_get_display_event_source(display));

    /*********************************************************************************************************/

    /**********************************************************************************************************
     * 
     *                                          DIBUJO EN DISPLAY
     * 
     *********************************************************************************************************/

    //Mientras el usuario no cierre el display

    while(!close_display){

        //Si se cierra la pestaña se prende el flag

        if (al_get_next_event( event_queue, &ev)) 
        {    
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                close_display = true;
        }

        //Cada cierto tiempo deteminado por el refresh rate , se actualiza la imagen

        if (!(timerTick % REFRESHRATE) && al_is_event_queue_empty(event_queue) ){

            //Se limpia la pantalla

            al_clear_to_color(al_map_rgb(BGCOLOR)); 

            //Se dibujan las entidades

            showLista(data->aliens);
            showLista(data->nave);
            showLista(data->balasAliens);
            showLista(data->balasUsuario);
            showLista(data->barrera);
                
            //Se muestra en pantalla

            al_flip_display();

        }
    }    

    /*********************************************************************************************************/

    //      DESTRUCCION DE OBJETOS

    al_destroy_display(display); 
    al_destroy_event_queue(event_queue);
    al_shutdown_image_addon();

    /*********************************************************************************************************/

    pthread_exit(0);
}

/**********************************************************************************************************************************************************/

/***********************************************************************************************************************************************************
 * 
 *                                                                      FUNCIONES SECUNDARIAS
 * 
 * ********************************************************************************************************************************************************/

//      Show Entity

int showEntity(object_t * entity){
  
    ALLEGRO_BITMAP * image = NULL;      //Se crea un bitmap donde guardar la imagen

    char * sprite = images[entity->type];       //Se busca la direccion del sprite

    image = al_load_bitmap(sprite);    //Se carga en el bitmap

    //Si no se pudo cargar salta error
    if (!image) {
        fprintf(stderr, "failed to load image !\n");
        return -1;
    }

    al_draw_bitmap(image, entity->pos.x, entity->pos.y, 0);     //Se dibuja en el display

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

/**********************************************************************************************************************************************************/