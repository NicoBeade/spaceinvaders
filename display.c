#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "utilidades.h"
#include "display.h"

#define REFRESHRATE 10

static char * images[]= {"sprites/alien1.png", "sprites/alien2.png", "sprites/alien3.png", "sprites/nave.png", "sprites/escudo.png"};

int timerTick = 1000000;

void * timer(){
/* Este thread es el encargado de controlar el tiempo del juego. Cuenta de una variable que se decrementa cada 10mS luego el resto de los
    threads utilizan esta variable para determinar cuando se deben ejecutar.
*/
    printf("Timer set\n");
    while(1){
        usleep(10 * U_SEC2M_SEC); //Sleep 10mS.
        timerTick -= 1;
    }
    pthread_exit(0);
}

void * thread1(void * arg) {
    
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;

    display_data_t * data = (display_data_t *) arg;
    printf("hola\n");

    bool close_display = false;
    bool fail= false;

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

    while(!close_display){

        ALLEGRO_EVENT ev;

        //Si se 
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

            //printf("aliens");
            showLista(data->aliens);
            //printf("nave");
            showLista(data->nave);
                
            //Se muestra en pantalla

            al_flip_display();

        }
    }    

    /*********************************************************************************************************/

    /**********************************************************************************************************
     * 
     *                                      DESTRUCCION DE OBJETOS 
     * 
     *********************************************************************************************************/

    al_destroy_display(display); 

    al_destroy_event_queue(event_queue);

    al_shutdown_image_addon();

    /*********************************************************************************************************/

    pthread_exit(0);
}

int main (void){

    pthread_t tid1,tid2;

    object_t alien1 = {{100,0}, PABLO, 1, NULL};
    object_t alien0 = {{0,0}, DANIEL, 1, &alien1};

    object_t nave = {{200,200}, NAVE, 1, NULL};

    display_data_t data = {&alien0, &nave, NULL, NULL, NULL};

    pthread_create(&tid1,NULL,timer,NULL);
    printf("t1\n");
    pthread_create(&tid2,NULL,thread1,&data);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    return 0;
}

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
