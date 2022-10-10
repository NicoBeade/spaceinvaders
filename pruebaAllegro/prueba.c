#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h> //Manejo de ttfs
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/************************************************************************************************/
typedef struct TEXTO
{
    char* texto;
    int posx;
    int posy;

    struct TEXTO * next;
} texto_t;


typedef struct {//Este struct se utiliza para obtener la entrada del usuario.

	int x;
	int y;
	uint8_t press;
} keys_t;

typedef struct{//Esta estructura define un vector para las coordenadas
    int x;
    int y;
}vector_t;

typedef struct OBJECT{//Cada alien, barrera, bala y la nave del usuario es un struct de este tipo y se los organizara en listas en funcion de cual de estos es.
    vector_t pos;//Posicion en x e y
    int type;//Tipo de objeto y categoria dentro del tipo
    int lives;//Cantidad de vidas del objeto, cada objeto podria tener distinta cantidad de vidas
    char animationStatus;//Estado de la animacion de cada nave, puede ser distinto para cada nave
    struct OBJECT * next;//Puntero al siguiente objeto de la lista.
}object_t;


 typedef struct {

    ALLEGRO_EVENT_QUEUE ** event_queue;
    ALLEGRO_EVENT * ev; 

    keys_t * keys;

    bool * close_display;
    bool * keyboardDownFlag;
    bool * keyboardUpFlag;
    int * keycode;

} keyboard_data_t;

typedef struct 
{
    ALLEGRO_EVENT_QUEUE ** event_queue;

    object_t ** objects;
    texto_t ** text;

    bool * close_display;
    bool * displayFlag;

} display_data_t;

typedef struct ALLEGRO {

    object_t ** objectsToShow;
    texto_t ** textToShow;
    keys_t * keys;

} data_allegro_t;

typedef struct {

    ALLEGRO_EVENT_QUEUE ** event_queue; 
    ALLEGRO_EVENT * ev;  
    
    bool * keyboardDownFlag;
    bool * keyboardUpFlag;
    int * keycode;
    bool * displayFlag; 
    bool * close_display;

} eventH_data_t;

enum keycodes {DOWN, UP, RIGHT, LEFT, SPACE};
#define KEYCODE ALLEGRO_KEY_DOWN-*data->keycode

/************************************************************************************************/
/************************************************************************************************/

void * displayt (ALLEGRO_THREAD * thr, void * dataIn);
void * keyboardt(ALLEGRO_THREAD * thr, void * dataIn);
void * allegroThread (ALLEGRO_THREAD * thr, void * arg);
void * eventHandler(ALLEGRO_THREAD * thr, void * dataIn);
int showEntity(object_t * entity);
int showObjects(object_t * inicial);
int showText(texto_t * data, ALLEGRO_FONT * fuente);
int showTexts(texto_t * inicial, ALLEGRO_FONT * fuente);

/************************************************************************************************/

int main()
{

    ALLEGRO_EVENT_QUEUE * event_queue;
    ALLEGRO_EVENT ev;
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_FONT * fuente = NULL;
    ALLEGRO_THREAD * principal;

    keys_t KEYS = { .x =0, .y = 0, .press = 0 };
    texto_t texto = {"hola mundo", 100, 100};

    object_t * toObject;
    texto_t * toText = &texto;
    keys_t * toKeys = &KEYS;

    data_allegro_t data = {&toObject, &toText, toKeys};

    principal = al_create_thread(allegroThread, &data);

    al_start_thread(principal);

    sleep(5.0);
    texto.texto="chau mundo";
    al_join_thread(principal, NULL);
    al_destroy_thread(principal);
    

    return 0;
}


void * allegroThread (ALLEGRO_THREAD * thr, void * dataIn){

    /*************************************************************************************************************
     * 
     *                                          MANEJO DE DATOS
     * 
     * ***********************************************************************************************************/

    data_allegro_t * data = (data_allegro_t*) dataIn;


    /**************************************************************
     * 
     *              PUNTEROS DE ALLEGRO
     * 
     * ***********************************************************/

    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_THREAD * teventH, * tdisplay, * tkeyboard; 

    /**************************************************************
     * 
     *              FLAGS Y VARIABLES
     * 
     * ***********************************************************/

    bool close_display = false;
    bool keybordDownFlag = false;
    bool keybordUpFlag = false;
    bool displayFlag = false; 

    int keycode = 0;
    
    /**************************************************************
     * 
     *              DATOS DE LOS THREADS
     * 
     * ***********************************************************/


    eventH_data_t dataH = {&event_queue, &ev, &keybordDownFlag, &keybordUpFlag, &keycode, &displayFlag, &close_display};

    display_data_t dataD = {&event_queue, data->objectsToShow, data->textToShow, &close_display, &displayFlag};
    keyboard_data_t dataK = {&event_queue, &ev, data->keys, &close_display, &keybordDownFlag, &keybordUpFlag, &keycode};

    /*************************************************************************************************************
     * 
     *                                          THREADS
     * 
     * ***********************************************************************************************************/

    al_init();

    event_queue = al_create_event_queue();

    teventH = al_create_thread(eventHandler, &dataH);
    tdisplay = al_create_thread(displayt, &dataD);
    tkeyboard = al_create_thread(keyboardt, &dataK);
    al_start_thread(teventH);
    al_start_thread(tdisplay);
    al_start_thread(tkeyboard);

    
    al_join_thread(teventH, NULL);
    al_join_thread(tdisplay, NULL);
    al_join_thread(tkeyboard, NULL);
    al_destroy_thread(teventH);
    al_destroy_thread(tdisplay);
    al_destroy_thread(tkeyboard);

    pthread_exit(0);
    
}

/*******************************************************/

void * eventHandler(ALLEGRO_THREAD * thr, void * dataIn){

    eventH_data_t * data = (eventH_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue; 

    ALLEGRO_EVENT * evp = data->ev;

    while(!*data->close_display){

        usleep(10 * 1000);

        if (al_get_next_event(event_queue, evp)) 
        {    
            if (evp->type  == ALLEGRO_EVENT_DISPLAY_CLOSE){
                *data->close_display = true;
            }
            else if (evp->type == ALLEGRO_EVENT_KEY_DOWN){
                *data->keycode = evp->keyboard.keycode;
                *data->keyboardDownFlag=true;
            }
            else if (evp->type == ALLEGRO_EVENT_KEY_UP){
                *data->keycode = evp->keyboard.keycode;
                *data->keyboardUpFlag=true;
            }

        }

        usleep(10 * 1000);

        *data->displayFlag = true;

    }

    pthread_exit(0);

}



void * displayt (ALLEGRO_THREAD * thr, void * dataIn){

    display_data_t * data = (display_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    ALLEGRO_DISPLAY * display = NULL;

    ALLEGRO_FONT * fuente = NULL;

    display = al_create_display(1000,700);

    al_init_image_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon

    fuente = al_load_ttf_font("space_invaders.ttf", 36, 0);


    al_register_event_source(event_queue, al_get_display_event_source(display));

    while(!*data->close_display){

        usleep(10 * 1000);    

        if(*data->displayFlag){

            al_clear_to_color(al_map_rgb(0,0,0));

            //showObjects(data->objects);
            showText(*data->text, fuente);

            al_flip_display();

            *data->displayFlag= false;
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

int showText(texto_t * data, ALLEGRO_FONT * fuente){

    al_draw_text(fuente, al_map_rgb(255,255,255) , data->posx, data->posy, ALLEGRO_ALIGN_LEFT, data->texto);
}

//      Show Entity

int showEntity(object_t * entity){
  
    ALLEGRO_BITMAP * image = NULL;      //Se crea un bitmap donde guardar la imagen

    //Se busca la direccion del sprite
    char * sprite;

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

int showObjects(object_t * inicial){

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

/*int showTexts(texto_t * inicial, ALLEGRO_FONT * fuente){

    //punter al primer objeto de la lista
    texto_t * puntero = inicial;

    //Si se paso un NULL salta error
    if(puntero == NULL){
        return -1;
    }

    else {
        //Se muestra el primer objeto
        showText(puntero, fuente);

        //Se muestran los siguientes
        while(puntero->next != NULL){
            puntero = puntero->next;
            showText(puntero, fuente);
        }
    }
    
    return 0;

}*/

void * keyboardt(ALLEGRO_THREAD * thr, void * dataIn){

    keyboard_data_t * data = (keyboard_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    int timer = 0;

    bool key_pressed[6] = {false, false, false, false, false, false};

    al_install_keyboard();

    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while(!*data->close_display){

        usleep(10 * 1000);

        timer++;
        if(*data->keyboardDownFlag){

            if(*data->keycode == ALLEGRO_KEY_SPACE){
                key_pressed[SPACE]= true;
            }     
            else{
                key_pressed[KEYCODE]= true;
            }

            *data->keyboardDownFlag= false;       
        }
        else if(*data->keyboardUpFlag){

            if(*data->keycode == ALLEGRO_KEY_SPACE){
                key_pressed[SPACE]= false;
            }     
            else{
                key_pressed[KEYCODE]= false;
            }

            *data->keyboardUpFlag= false;       
        }

        if(timer % 20 == 0 ){

            if(key_pressed[UP]){
                (*data->keys).y = 1;
            }
            if(key_pressed[DOWN]){
                (*data->keys).y = -1;
            }
            if(key_pressed[RIGHT]){
                (*data->keys).x = 1; 
            }
            if(key_pressed[LEFT]){
                (*data->keys).x = -1;
            }
            if(key_pressed[SPACE]){
                (*data->keys).press = 1;
            }
            timer = 0;
        }
    }

    pthread_exit(0); 
}