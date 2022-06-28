#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "utilidades.h"
#include "display.h"
#define FPS 60

int main(void) {
    
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    bool redraw = false;
    bool close_display = false;
    bool fail= false;

    object_t nave= {{300,50}, 1, 1, NULL};
    object_t alien= {{100,50}, 1, 1, NULL};
    object_t escudo= {{100,300}, 1, 1, NULL};

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

    //Inicializacion del timer

    if (!(timer = al_create_timer(1.0 / FPS)) && !fail) {
        fprintf(stderr, "failed to create timer!\n");
        fail = true;
    }

    //inicializacion del display

    if (!(display = al_create_display(X_MAX, Y_MAX)) && !fail) {
        fprintf(stderr, "failed to create display!\n");     
        fail = true;
    }

    if (fail){
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        al_shutdown_image_addon();
        return -1;
    }

    //Se registra el display en la cola de eventos

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_start_timer(timer);

    /*********************************************************************************************************/

    /**********************************************************************************************************
     * 
     *                                          DIBUJO EN DISPLAY
     * 
     *********************************************************************************************************/

    while(!close_display){

        ALLEGRO_EVENT ev;

        if (al_get_next_event( event_queue, &ev)) 
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                close_display = true;

            else if(ev.type == ALLEGRO_EVENT_TIMER){

                redraw = true;
            }
        }

        if ( redraw && al_is_event_queue_empty(event_queue)){

            redraw = false;

            //Se limpia la pantalla

            al_clear_to_color(al_map_rgb(BGCOLOR)); 

            //Se dibujan las entidades

            showEntity(&nave, NAVEIMG);
            showEntity(&alien, ALIENIMG3);
            showEntity(&escudo, ESCUDOIMG);
                
            //Se muestra en pantalla
            
            al_flip_display(); 

            alien.pos.x+=10;
        }

    }    

    /*********************************************************************************************************/

    /**********************************************************************************************************
     * 
     *                                      DESTRUCCION DE OBJETOS 
     * 
     *********************************************************************************************************/

    al_destroy_display(display); 

    al_destroy_timer(timer);

    al_destroy_event_queue(event_queue);

    al_shutdown_image_addon();

    /*********************************************************************************************************/

    return 0;
}

int showEntity(object_t * entity, char * path){

    ALLEGRO_BITMAP * image = NULL;

    image = al_load_bitmap(path);
    if (!image) {
        fprintf(stderr, "failed to load image !\n");
        return -1;
    }

    al_draw_bitmap(image, entity->pos.x, entity->pos.y, 0);

    al_destroy_bitmap(image);
}
