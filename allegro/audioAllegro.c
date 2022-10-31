#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include "audioAllegro.h"

int playAudio(char * audio);

void * audiot(ALLEGRO_THREAD * thr, void * dataIn) {

    audio_data_t * data = (audio_data_t *) dataIn;

    ALLEGRO_DISPLAY *display = NULL;

    if (!al_install_audio()) {
        fprintf(stderr, "failed to initialize audio!\n");
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "failed to initialize audio codecs!\n");
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "failed to reserve samples!\n");
    }

    display = al_create_display(1, 1);

    if (!display) {
        fprintf(stderr, "failed to create display!\n");
    }

    //Loop the sample until the display closes.
    //playAudio("allegro/audio.wav");
    while (!*data->close_display) {
              

    }

    al_uninstall_audio();
    al_destroy_display(display);
    return 0;
}

int playAudio(char * audio){

    ALLEGRO_SAMPLE * sample;

    sample = al_load_sample(audio);
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

    al_destroy_sample(sample);  
    return 0;
}