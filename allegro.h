
typedef struct ALLEGRO {

    object_t * nave;
    object_t * aliens;
    object_t * balasAliens;
    object_t ** balasUsuario;
    object_t * barreras;
    //menu_t * menu;
    level_setting_t * levelSettings;

} data_allegro_t;

void * allegroThread (void * arg);