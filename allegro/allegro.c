#include <allegro5/allegro.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "../utilidades.h"
#include "displayAllegro.h"
#include "inputAllegro.h"
#include "allegro.h"

/************************************************************************************************
 * 
 *                              CONSTANTES
 * 
*************************************************************************************************/

#define FPS 2
#define MENUX 150   //Posicion x del menu
#define MENUY 100   //Posicion y del menu
#define ESPACIADOMENU 100   //Espaciado entre opciones del menu
#define SELECTOR 50     //Espacio entre el selector y la opcion seleccionada

//Timer tick
extern int timerTick;
extern gameStatus_t GAME_STATUS;

void * eventHandler(ALLEGRO_THREAD * thr, void * dataIn);
texto_t* addText(texto_t * firstObj, char * texto, int posx, int posy);

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

    display_data_t dataD = {&event_queue, data->punteros, data->textToShow, &close_display, &displayFlag};
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
    int i = 0, j= 0;

    
    //Busco la opcion seleccionada
    for(i = 0; i<data->actualOp; i++){
        puntero = puntero->next;
    }
    //la muevo
    puntero->posx-= SELECTOR;
    
    //busco la nueva opcion seleccionada
    puntero = *data->toText;
    for(i = 0; i<data->nextOp; i++){
        puntero = puntero->next;
    }
    
    //la muevo
    puntero->posx += SELECTOR;

    //busco el selector
    puntero = *data->toText;
    for(j = 0; j< (data->menu)->cantOpciones; j++){
        puntero = puntero->next;
    }
    
    puntero->posx= MENUX;
    puntero->posy= MENUY + data->nextOp * ESPACIADOMENU;

}

int selectPlayInicio(void){
    printf("Select Play Inicio\n");
    GAME_STATUS.pantallaActual = START_LEVEL;
    GAME_STATUS.menuActual = START_LEVEL_MENU;
    return 0;
}

int selectLevels(void){
    return 1;
}

int selectVolume(void){
    return 1;
}

int selectQuitGame(void){
    printf("Select Quit Game\n");
    GAME_STATUS.pantallaActual = QUIT_GAME;
    return 0;
}

int selectResume(void){
    printf("Select Resume\n");
    GAME_STATUS.pantallaActual = IN_GAME;
    menuGame.exitStatus = 1;
    return 0;
}

int selectRestartLevel(void){
    printf("Select Restart Level\n");
    GAME_STATUS.pantallaActual = DESTROY_LEVEL;
    GAME_STATUS.menuActual = START_LEVEL_MENU;
    return 0;
}

int selectMainMenu(void){
    printf("Select Main Menu\n");
    GAME_STATUS.pantallaActual = DESTROY_LEVEL;
    GAME_STATUS.menuActual = MENU_INICIO;
    return 0;
}

int selectDificulty(void){
    return 0;
}