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
#include <allegro5/allegro_ttf.h> //Manejo de ttfs
#include <allegro5/allegro_primitives.h>
#include <pthread.h>
#include <stdio.h>
#include "displayAllegro.h"
#include "allegro.h"
#include "../spaceLib/spaceLib.h"
#include <semaphore.h>


extern sem_t SEM_GAME;

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
void showText(texto_t * data, ALLEGRO_FONT * fuente);
int showTexts(texto_t * inicial, ALLEGRO_FONT * fuente);
texto_t* addText(texto_t * firstObj, char * texto, int posx, int posy);
texto_t * emptyText(texto_t * firstText);


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
    al_init_primitives_addon();

    fuente = al_load_ttf_font("allegro/spaceInv.ttf", 36, 0);
    
    al_register_event_source(event_queue, al_get_display_event_source(display));

   
    while(!*data->close_display){

        usleep(10 * U_SEC2M_SEC);    

        if(*data->displayFlag){
        
            //sem_wait(&SEM_GAME);
            //Se limpia la pantalla
            al_clear_to_color(al_map_rgb(BGCOLOR));
            //Se dibujan los elementos y textos en el buffer
            showObjects( *((*data).punteros.alienList) );
            showObjects( *((*data).punteros.UsrList) );
            showObjects( *((*data).punteros.barrerasList) );
            showObjects( *((*data).punteros.balasUsr) );
            showObjects( *((*data).punteros.balasAlien) );
            showObjects( *((*data).punteros.mothershipList) );
            showTexts(*data->text, fuente);
            //Se muestra en pantalla
            al_flip_display();

            *data->displayFlag= false;

            //sem_post(&SEM_GAME);
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

void showText(texto_t * data, ALLEGRO_FONT * fuente){

    //Comando para escribir un texto en el buffer
    
    al_draw_rectangle(data->posx - 10, data->posy - 5, 
    data->posx + data->lenght*TAMLETRAX +10 , data->posy + TAMLETRAY + 5, al_map_rgb(0,0,255), 3);

    al_draw_rectangle(data->posx - 5, data->posy -10, 
    data->posx + data->lenght*TAMLETRAX + 5, data->posy + TAMLETRAY +10, al_map_rgb(0,0,255), 3);

    al_draw_filled_rectangle(data->posx - 7, data->posy - 2, 
    data->posx + data->lenght*TAMLETRAX + 10, data->posy + TAMLETRAY +2, al_map_rgb(0,0,0));

    al_draw_filled_rectangle(data->posx - 2, data->posy -7, 
    data->posx + data->lenght*TAMLETRAX + 2, data->posy + TAMLETRAY + 7, al_map_rgb(0,0,0));

    al_draw_text(fuente, al_map_rgb(80,80,80) , data->posx, data->posy, ALLEGRO_ALIGN_LEFT, data->texto);
}

int showEntity(object_t * entity){
  
    ALLEGRO_BITMAP * image = NULL;      //Se crea un bitmap donde guardar la imagen

    //Se busca la direccion del sprite
    objectType_t * asset = getObjType(entity->type);
    
    char * sprite = asset->sprite1;

    if (sprite == NULL){
        return -1;
    }
    else{
        image = al_load_bitmap(sprite);    //Se carga en el bitmap
    }
    

    //Si no se pudo cargar salta error
    if (!image) {
        //fprintf(stderr, "failed to load image ! %s\n", sprite);
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
// Esta funcion se encarga de agregar un nuevo texto a la lista

	texto_t * newText = malloc(sizeof(texto_t));//Agrega el nuevo texto

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
    newText -> lenght = strlen(texto);

    newText -> next = NULL;

	return firstObj;//Devuelve un puntero al primer elemento.
}

texto_t * emptyText(texto_t * firstText){
//Esta funcion se encarga de limpiar la lista de textos a escribir
    texto_t * first = firstText;

    //Recorre la lista y libera el espacio
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


/**********************************************************************************************************************************************************/