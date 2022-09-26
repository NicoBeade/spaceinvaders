
typedef struct ALLEGRO {

    object_t * toShow;
    keys_t * keys;
    level_setting_t * levelSettings;

} data_allegro_t;

void * allegroThread (void * arg);