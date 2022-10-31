#ifndef AUDIO_H
#define AUDIO_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO

typedef struct 
{
    ALLEGRO_EVENT_QUEUE ** event_queue;

    bool * close_display;

}audio_data_t;

void * audiot(ALLEGRO_THREAD * thr, void * dataIn);

#endif