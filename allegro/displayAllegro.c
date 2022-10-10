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

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
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

//Structs
    
typedef int (*option_t)(void);

typedef struct {//Este struct contiene la informacion necesaria para ejecutar un menu.

	keys_t * keys;
	option_t selectOption[10];//Struct que contiene punteros a funciones que indican que hacer cuando se selecciona una opcion.
    char* textOpciones[10];//Arreglo de punteros a los strings que contienen el texto de cada opcion.
    int cantOpciones;//Cantidad de opciones del menu.
    void (*changeOption)(void* argChangeOption);//Callback a la funcion que cambia la opcion seleccionada.
} menu_t;



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

int showObjects(object_t * inicial);
int showText(texto_t * data, ALLEGRO_FONT * fuente);
int showTexts(texto_t * inicial, ALLEGRO_FONT * fuente);
texto_t* addText(texto_t * firstObj, char * texto, int posx, int posy);
texto_t * emptyText(texto_t * firstText);
texto_t * allegroMenu(menu_t * data, texto_t * toshow);
void changeOption(void * data);

/***********************************************************************************************************************************************************
 * 
 *                                                                      THREAD PRINCIPAL
 * 
 * ********************************************************************************************************************************************************/

void * displayt (ALLEGRO_THREAD * thr, void * dataIn){

    display_data_t * data = (display_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    ALLEGRO_DISPLAY * display = NULL;

    ALLEGRO_FONT * fuente = NULL;

    display = al_create_display(X_MAX,Y_MAX);

    al_init_image_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon

    fuente = al_load_ttf_font("Fuente.ttf", 36, 0);
    

    al_register_event_source(event_queue, al_get_display_event_source(display));

    while(!*data->close_display){

        usleep(10 * U_SEC2M_SEC);    

        if(*data->displayFlag){

            sem_wait(&semaforo);

            al_clear_to_color(al_map_rgb(BGCOLOR));

            showObjects(*data->objects);
            showTexts(*data->text, fuente);

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

int showText(texto_t * data, ALLEGRO_FONT * fuente){

    al_draw_text(fuente, al_map_rgb(255,255,255) , data->posx, data->posy, ALLEGRO_ALIGN_LEFT, data->texto);
}

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

int showTexts(texto_t * inicial, ALLEGRO_FONT * fuente){

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

}

texto_t* addText(texto_t * firstObj, char * texto, int posx, int posy){
/* Esta funcion se encarga de agregar un nuevo alien a la lista, inicializando su posicion, tipo y cantidad de vidas.
    Devuelve un puntero al primer elemento de la lista.
*/	
	texto_t * newText = malloc(sizeof(texto_t));//Agrega el nuevo alien

	if(newText == NULL){//Si no se puede hacer el malloc indica error.
		printf("Err in gameLib, addObj function: couldnt add node to the list\n");
		return NULL; //error
	}

    if(firstObj != NULL){//Si no es el primero de la lista debe avanzar hasta el ultimo elemento.
        texto_t * lastObj = firstObj;//Se almacena el puntero al primer elemento.

		while(lastObj -> next != NULL){
			lastObj = lastObj -> next;
		}
        lastObj -> next = newText;
	}
    else{//Si es el primero de la lista debemos devolver ese puntero.
        firstObj = newText;
    }

	newText -> texto = texto;//Asigna los valores indicados en los distitntos campos del alien.
	newText -> posx = posx;
	newText -> posy = posy;
    newText -> next = NULL;

	return firstObj;//Devuelve un puntero al primer elemento.
}

texto_t * emptyText(texto_t * firstText){

    texto_t * first = firstText;

    if(first != NULL){
        if(first->next == NULL){
            free(first);
        }
        else{
            texto_t * sig = first->next;
            do{
                sig = first->next;
                free(first);
                first = sig;
            }while(sig!=NULL);
        }

    }
    return first;             //Se devuelve la lista
}

texto_t * allegroMenu(menu_t * data, texto_t * toshow){

    int i;
    for( i = 0; i<data->cantOpciones; i++){

        if(i==0){
            toshow=addText(toshow, data->textOpciones[i], 130, (i+1)*100);
        }
        else{
            toshow=addText(toshow, data->textOpciones[i], 100, (i+1)*100);
        }
    }
    toshow = addText(toshow, "> ", 100, 100);
    return toshow;
}

void changeOption(void * dataIn){

    changeOptionData_t * data = (changeOptionData_t *) dataIn;
    texto_t * puntero = *data->toText;
    int i = 0, j= 0;

    
    //Busco la opcion seleccionada
    for(i = 0; i<data->actualOp; i++){
        puntero = puntero->next;
    }
    //la muevo
    puntero->posx-=30;
    
    //busco la nueva opcion seleccionada
    puntero = *data->toText;
    for(i = 0; i<data->nextOp; i++){
        puntero = puntero->next;
    }
    
    //la muevo
    puntero->posx +=30;

    //busco el selector
    puntero = *data->toText;
    for(j = 0; j< (data->menu)->cantOpciones; j++){
        puntero = puntero->next;
    }
    
    puntero->posx=100;
    puntero->posy=(data->nextOp+1)*100;

}

/**********************************************************************************************************************************************************/