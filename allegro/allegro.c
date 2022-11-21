#include <allegro5/allegro.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "../utilidades.h"
#include "outputAllegro.h"
#include "inputAllegro.h"
#include "allegro.h"
/************************************************************************************************
 * 
 *                              CONSTANTES
 * 
*************************************************************************************************/

#define FPS 2
#define MENUX 150   //Posicion x del menu
#define MENUY 300   //Posicion y del menu
#define ESPACIADOMENU 100   //Espaciado entre opciones del menu
#define SELECTOR 50     //Espacio entre el selector y la opcion seleccionada

#define CANTOP 5
#define SPACELETX 60
#define SPACELETY 50 
#define SCOREX X_MAX/2 - TAMLETRAX*(1.5)
#define SCOREY 100
#define LETRAX X_MAX/2 - TAMLETRAX*(0.5) - SPACELETX
#define LETRAY 200

//Timer tick
extern int timerTick;
extern gameStatus_t GAME_STATUS;

void * eventHandler(ALLEGRO_THREAD * thr, void * dataIn);


typedef struct {

    ALLEGRO_EVENT_QUEUE ** event_queue; 
    ALLEGRO_EVENT * ev;  
    
    bool * keyboardDownFlag;
    bool * keyboardUpFlag;
    int * keycode;
    bool * displayFlag; 
    bool * close_display;

} eventH_data_t;

/*******************************************************/
void * allegroThread (void * dataIn){

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

    output_data_t dataD = {&event_queue, data->punteros, data->textToShow, data->audioToPlay ,&close_display, &displayFlag};
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

audio_t* addAudio(audio_t * firstObj, int audioId){
// Esta funcion se encarga de agregar un nuevo audio a la lista
	audio_t * newAudio = malloc(sizeof(audio_t));//Agrega el nuevo audio

	if(newAudio == NULL){//Si no se puede hacer el malloc indica error.
		printf("Err in gameLib, addObj function: couldnt add node to the list\n");
		return NULL; //error
	}

    if(firstObj != NULL){//Si no es el primero de la lista debe avanzar hasta el ultimo elemento.
        audio_t * lastObj = firstObj;//Se almacena el puntero al primer elemento.
		while(lastObj -> next != NULL){
			lastObj = lastObj -> next;
		}
        lastObj -> next = newAudio;
	}

    else{//Si es el primero de la lista debemos devolver ese puntero.
        firstObj = newAudio;
    }

	newAudio -> audioId = audioId ;

    newAudio -> next = NULL;

	return firstObj;//Devuelve un puntero al primer elemento.
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

	newText -> texto = texto;//Asigna los valores indicados en los distitntos campos del texto.
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
    return first;             //Se devuelve la lista
}

void * eventHandler(ALLEGRO_THREAD * thr, void * dataIn){

    eventH_data_t * data = (eventH_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue; 

    ALLEGRO_EVENT * evp = data->ev;

    while(!*data->close_display){

        usleep(10 * U_SEC2M_SEC);

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

        if (timerTick % FPS == 0){
            *data->displayFlag = true;
        }

    }

    pthread_exit(0);

}

/***********************************************************************************************************************************************************
 * 
 *                                                                      MENUES
 * 
 * ********************************************************************************************************************************************************/

texto_t * allegroMenu(menu_t * data, texto_t * toshow){
//Esta funcion se utiliza para mostrar los menues en pantalla
    
    int i;
    for( i = 0; i<data->cantOpciones; i++){
        //Se agregan los textos de las opciones a la lista de textos
        if(i==0){
            toshow=addText(toshow, data->textOpciones[i], MENUX + SELECTOR, MENUY + i * ESPACIADOMENU);
        }
        else{
            toshow=addText(toshow, data->textOpciones[i], MENUX , MENUY + i * ESPACIADOMENU);
        }
    }
    //se agrega el selector
    toshow = addText(toshow, ">", MENUX, MENUY);
    return toshow;
}

void changeOption(void * dataIn){
//Esta funcion realiza la animacion de cambio de oipcion
    changeOptionData_t * data = (changeOptionData_t *) dataIn;
    texto_t * puntero = *data->toText;
    int i = 0;
    int esc = 0;

    if(data->actualOp == 0 && data->nextOp == ((data->menu)->cantOpciones)-1){

        esc = -(data->menu)->cantOpciones + 1;

    }else if(data->actualOp == ((data->menu)->cantOpciones)-1 && data->nextOp == 0){

        esc = (data->menu)->cantOpciones - 1;

    }else if(data->nextOp > data->actualOp){

        esc = -1;

    }else{

        esc = 1;

    }
    
    for(i = 0; i < (data->menu)->cantOpciones; i++ ){

        puntero->posy += esc * ESPACIADOMENU;

        if(i == data->actualOp){
            puntero->posx-= SELECTOR;
        }
        if(i == data->nextOp){
            puntero->posx += SELECTOR;
        }
        puntero = puntero->next;
    }
}

/***********************************************************************************************************************************************************
 * 
 *                                                                      SCORE
 * 
 * ********************************************************************************************************************************************************/

texto_t * allegroScore(texto_t * toshow, char* scoreActual, char letras[15][2]){
    
    int i, j;

    for(i = 0; i < 3; i++){
    //Se recorren las columnas
        for(j = 0; j < CANTOP; j++){
            //Se recorren las letras que se muestran en cada columna
            //Se añaden los textos que permiten mostrar las letras
            toshow = addText(toshow, letras[i*CANTOP+j], LETRAX + SPACELETX*i, LETRAY + SPACELETY*j);

        }
    }
    toshow = addText(toshow, ">", LETRAX - 25 , LETRAY + SPACELETY* 2); //se añade el cursor
    toshow = addText(toshow, scoreActual, SCOREX, SCOREY); //se añade el score 
    return toshow;
}

void changeLetra(char letras[15][2], int letraActual, int dir){

    int i;
    char (* puntero)[2] = &(letras[0]); //Se crea un puntero temporal
    char str[2]={0,0}; //Se crea un string temporal

    puntero += letraActual*CANTOP; //Se mueve el puntero hacia la primer letra a cambiar

    for(i = 0; i < CANTOP; i++){
        
        strcpy(str, *puntero); //Se guarda la letra a cambiar en el string temporal

        str[0]+=dir; //Se modifica el string temporal

        if(dir<0 && str[0] == 'A'-1){
            str[0] = 'Z';
        }else if(dir>0 && str[0] == 'Z'+1){
            str[0] = 'A';
        }

        strcpy(*puntero,str); //Se modifica el string original
        puntero++;
    }

}

texto_t * changeCol(texto_t * toshow, int nextOp){

    texto_t * puntero = toshow; //Se crea un puntero temporal a la lista de textos
    int i;

    for(i = 0; i < 3*CANTOP; i++ ){
        puntero = puntero->next; //Se llega a el selector
    }

    puntero->posx = LETRAX - 25 + nextOp*SPACELETX; //Se mueve el selector
    return toshow;
}