#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h> //Manejo de ttfs
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
//#include "displayAllegro.h"
//#include "inputAllegro.h"
//#include "allegro.h"

typedef struct
{
    char* texto;
    int posx;
    int posy;
} texto_t;

int showText(texto_t * data, ALLEGRO_FONT * fuente);

int main()
{

    ALLEGRO_EVENT_QUEUE * event_queue;
    ALLEGRO_EVENT ev;
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_FONT * fuente = NULL;

    texto_t text = {"hola mundo", 100, 100};
    al_init();

    event_queue = al_create_event_queue();
    display = al_create_display(1000,700);
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon

    fuente = al_load_ttf_font("Fuente.ttf", 36, 0);
    
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_clear_to_color(al_map_rgb(255, 255, 255)); //Hace clear del backbuffer del diplay al color RGB 255,255,255 (blanco)

    showText(&text, fuente);

    al_flip_display();

    al_rest(10.0);

    al_destroy_display(display);
    al_destroy_font(fuente);
    al_destroy_event_queue(event_queue);

    return 0;
}

int showText(texto_t * data, ALLEGRO_FONT * fuente){

    al_draw_text(fuente, al_map_rgb(255,0,0) , data->posx, data->posy, ALLEGRO_ALIGN_LEFT, data->texto);
}