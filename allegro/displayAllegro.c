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

#include <pthread.h>
#include <stdio.h>
#include "displayAllegro.h"
#include <semaphore.h>

#define SNAVE "sprites/nave.png"
#define SALIEN1 "sprites/alien1.png"
#define SALIEN2 "sprites/alien2.png"
#define SALIEN3 "sprites/alien3.png"
#define SBARRERA "sprites/nave.png"
#define SBALA "sprites/bala.png"

extern sem_t semaforo;

//En esta array se guardan las direcciones a los sprites
    

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

void * displayt (ALLEGRO_THREAD * thr, void * dataIn){

    display_data_t * data = (display_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    ALLEGRO_DISPLAY * display = NULL;

    display = al_create_display(X_MAX,Y_MAX);

    al_init_image_addon();
    

    al_register_event_source(event_queue, al_get_display_event_source(display));

    while(!*data->close_display){

        usleep(10 * U_SEC2M_SEC);    

        if(*data->displayFlag){

            sem_wait(&semaforo);

            al_clear_to_color(al_map_rgb(BGCOLOR));

            showLista(data->punteros->nave);
            showLista(data->punteros->aliens);
            showLista(data->punteros->balasAliens);
            showLista(*data->punteros->balasUsuario);
            showLista(data->punteros->barreras);

            al_flip_display();

            *data->displayFlag= false;

            sem_post(&semaforo);
        }
    }

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

    //Se busca la direccion del sprite
    char * sprite;

    switch (entity->type){
        case NAVE:
            sprite = SNAVE;
            break;

        case DANIEL:
            sprite = SALIEN1;
            break;

        case PABLO:
            sprite = SALIEN2;
            break;

        case NICOLAS:
            sprite = SALIEN3;
            break;    

        case BALA_DANIEL:
        case BALA_NICOLAS:
        case BALA_PABLO:
        case BALA_USUARIO:
            sprite = SBALA;
            break;

        case BARRERA:
            sprite = SBARRERA;
            break;

        default:
            break;
    }
           

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