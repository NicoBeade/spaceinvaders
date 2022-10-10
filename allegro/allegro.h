
typedef struct ALLEGRO {

    object_t ** objectsToShow;
    texto_t ** textToShow;
    keys_t * keys;

} data_allegro_t;

void * allegroThread (void * arg);