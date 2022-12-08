#include "allegroUtilities.h"

void * eventHandler(ALLEGRO_THREAD * thr, void * dataIn);


typedef struct {

    ALLEGRO_EVENT_QUEUE ** event_queue; 
    ALLEGRO_EVENT * ev;  
    
    bool * keyboardDownFlag;
    bool * keyboardUpFlag;
    int * keycode;
    bool * refreshInput;
    bool * close_display;
    int * closeGameFlag;

} eventH_data_t;

/***********************************************************************************************************************************************************
 * 
 *                                                                      THREAD PRINCIPAL
 * 
 * ********************************************************************************************************************************************************/

void * allegroThread (void * dataIn){

    /*************************************************************************************************************
     * 
     *                                          MANEJO DE DATOS
     * 
     * ***********************************************************************************************************/

    data_allegro_t * data = (data_allegro_t*) dataIn;

    /*****************PUNTEROS DE ALLEGRO*******************/

    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_THREAD * teventH, * tdisplay, * tkeyboard; 

    /*****************FLAGS Y VARIABLES*********************/

    bool refreshInput = false;
    bool close_display = false;
    bool keybordDownFlag = false;
    bool keybordUpFlag = false;

    int keycode = 0;
    
    /*****************DATOS DE LOS THREADS*******************/

    eventH_data_t dataH = {&event_queue, &ev, &keybordDownFlag, &keybordUpFlag, &keycode, &refreshInput ,&close_display, data->flagCloseGame};

    output_data_t dataD = {&event_queue, data->punteros, data->textToShow , &close_display, data->comPointer};
    keyboard_data_t dataK = {&event_queue, &ev, data->keys, &refreshInput, &close_display, &keybordDownFlag, &keybordUpFlag, &keycode};

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

/***********************************************************************************************************************************************************
 * 
 *                                                                      EVENT HANDLER
 * 
 * ********************************************************************************************************************************************************/

void * eventHandler(ALLEGRO_THREAD * thr, void * dataIn){

    eventH_data_t * data = (eventH_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue; 

    ALLEGRO_EVENT * evp = data->ev;
        
    ALLEGRO_TIMER * timer = NULL;

    timer = al_create_timer(1.0 / 60.0);                         //Se crea el timer
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    
    al_start_timer(timer); //Empieza el timer

    while(!*data->close_display){

        usleep(10 * U_SEC2M_SEC);

        if(*data->closeGameFlag == 1){
            *data->close_display = true;
        }

        if (al_get_next_event(event_queue, evp)) 
        {    
            if (evp->type  == ALLEGRO_EVENT_DISPLAY_CLOSE){
                *data->closeGameFlag = 1;
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
            else if (evp->type == ALLEGRO_EVENT_TIMER){
                *data->refreshInput = true;
            }

        }

    }

    al_destroy_timer(timer);
    pthread_exit(0);

}

/***********************************************************************************************************************************************************
 * 
 *                                                                      DISPLAY STATUS
 * 
 * ********************************************************************************************************************************************************/

int displayHandler(argDisplay_t * dataIn){

    *dataIn->comPointer = dataIn->pantalla + 1;
    
    while(*dataIn->comPointer);

    return 0;

}

/***********************************************************************************************************************************************************
 * 
 *                                                                      MANEJO DE LISTAS
 * 
 * ********************************************************************************************************************************************************/

texto_t* addText(texto_t * firstObj, char * texto, int fuente, int posx, int posy){
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

	newText -> texto = texto;//Asigna los valores indicados en los distitntos campos del texto.
	newText -> fuente = fuente;
    newText -> posx = posx;
	newText -> posy = posy;

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
    return NULL;             //Se devuelve la lista
}

sprite_t* addSprite(sprite_t * firstObj, char * direccion, int posx, int posy){
// Esta funcion se encarga de agregar un nuevo sprite a la lista
	sprite_t * newText = malloc(sizeof(sprite_t));//Agrega el nuevo sprite

	if(newText == NULL){//Si no se puede hacer el malloc indica error.
		printf("Err in gameLib, addObj function: couldnt add node to the list\n");
		return NULL; //error
	}

    if(firstObj != NULL){//Si no es el primero de la lista debe avanzar hasta el ultimo elemento.
        sprite_t * lastObj = firstObj;//Se almacena el puntero al primer elemento.
		while(lastObj -> next != NULL){
			lastObj = lastObj -> next;
		}
        lastObj -> next = newText;
	}

    else{//Si es el primero de la lista debemos devolver ese puntero.
        firstObj = newText;
    }

	newText -> direccion = direccion;//Asigna los valores indicados en los distitntos campos del sprite.
    newText -> posx = posx;
	newText -> posy = posy;

    newText -> next = NULL;

	return firstObj;//Devuelve un puntero al primer elemento.
}

sprite_t * emptySprite(sprite_t * firstSprite){
//Esta funcion se encarga de limpiar la lista de sprites a escribir
    sprite_t * first = firstSprite;
    //Recorre la lista y libera el espacio
    if(first != NULL){
        if(first->next == NULL){
            free(first);
        }
        else{
            sprite_t * sig = first->next;
            do{
                sig = first->next;
                free(first);
                first = sig;
            }while(sig!=NULL);
        }
    }
    return NULL;             //Se devuelve la lista
}
