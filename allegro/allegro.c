#include <allegro5/allegro.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "outputAllegro.h"
#include "inputAllegro.h"
#include "allegro.h"

/************************************************************************************************
 * 
 *                              CONSTANTES
 * 
*************************************************************************************************/

#define FPS 2
#define MENUX 80   //Posicion x del menu
#define MENUY 300   //Posicion y del menu
#define ESPACIADOMENU 120   //Espaciado entre opciones del menu
#define SELECTOR 50     //Espacio entre el selector y la opcion seleccionada

#define CANTOP 5
#define SPACELETX 70
#define SPACELETY 70 
#define SCOREX X_MAX/2
#define SCOREY 70
#define LETRAX X_MAX/2 - TAMLETRAX*(0.5) - SPACELETX
#define LETRAY 200

#define FIRSTX X_MAX/2
#define FIRSTY 70
#define LEFTCOLUMNX 100
#define COLUMNY 220
#define RIGHTCOLUMNX X_MAX-300


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

    output_data_t dataD = {&event_queue, data->punteros, data->textToShow ,&close_display, &displayFlag};
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

TextObj_t * allegroMenu(menu_t * data, TextObj_t * lists){
//Esta funcion se utiliza para mostrar los menues en pantalla
    
    int i;
    TextObj_t salida = {NULL,NULL};

    for( i = 0; i<data->cantOpciones; i++){
        //Se agregan los textos de las opciones a la lista de textos
        if(i==0){
            salida.textoList=addText(salida.textoList, data->textOpciones[i], largeF, MENUX + SELECTOR, MENUY + i * ESPACIADOMENU);
        }
        else{
            salida.textoList=addText(salida.textoList, data->textOpciones[i], mediumF, MENUX , MENUY + i * ESPACIADOMENU);
        }
    }
    //se agrega el selector
    salida.textoList = addText(salida.textoList, ">", largeF, MENUX, MENUY);

    salida.spriteList = addSprite(salida.spriteList, ((data->spritesDir)[0]), X_MAX - 400, Y_MAX/2);
    if(GAME_STATUS.menuActual == MENU_INICIO){
        salida.spriteList = addSprite(salida.spriteList, "game/spritesAllegro/titleMM.png", X_MAX/2, 0);
    };
    lists->spriteList= salida.spriteList;
    lists->textoList= salida.textoList;
    return lists;
}

void changeOption(void * dataIn){
//Esta funcion realiza la animacion de cambio de oipcion
    changeOptionData_t * data = (changeOptionData_t *) dataIn;
    texto_t * puntero = *data->toText;
    sprite_t * sprite = *data->toSprite;
    
    int i = 0;
    int esc = 0; //Esta variable se encarga de saber si hay que subir o bajar las opciones

    if(GAME_STATUS.menuActual != MENU_LEADERBOARD){
        if(data->actualOp == 0 && data->nextOp == ((data->menu)->cantOpciones)-1){

            esc = -(data->menu)->cantOpciones + 1; //Si estoy en la primer opcion tengo que subir todo

        }else if(data->actualOp == ((data->menu)->cantOpciones)-1 && data->nextOp == 0){

            esc = (data->menu)->cantOpciones - 1; //Si estoy en la ultima opcion tengo que bajar todo

        }else if(data->nextOp > data->actualOp){ //En las otras situaciones muevo todo un lugar para arriba o abajo

            esc = -1; 

        }else{

            esc = 1;

        }
        
        for(i = 0; i < (data->menu)->cantOpciones; i++ ){
            //Recorro la lista
            puntero->posy += esc * ESPACIADOMENU; //Muevo todo segun la variable esc

            if(i == data->actualOp){
                puntero->posx-= SELECTOR; //La opcion actual la muevo hacia la izquierda
                puntero->fuente = mediumF; //Reduzco el tamaño de letras
            }
            if(i == data->nextOp){
                puntero->posx += SELECTOR; //La nueva opcion la muevo hacia la derecha
                puntero->fuente = largeF; //Incremento el tamaño de letras
            }
            puntero = puntero->next;
        }

        if(((data->menu)->spritesDir)[0] != NULL){
            sprite->direccion = ((data->menu)->spritesDir)[data->nextOp];
        }
    }
}

/***********************************************************************************************************************************************************
 * 
 *                                                                      SCORE
 * 
 * ********************************************************************************************************************************************************/

TextObj_t * allegroScore(TextObj_t * lists, char* scoreActual, char letras[15][2]){
    
    int i, j;
    float scoreLen = strlen(scoreActual);
    TextObj_t salida = {NULL,NULL};

    for(i = 0; i < 3; i++){
    //Se recorren las columnas
        for(j = 0; j < CANTOP; j++){
            //Se recorren las letras que se muestran en cada columna
            //Se añaden los textos que permiten mostrar las letras

            if(j == (CANTOP/2)){
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], largeF, LETRAX + SPACELETX*i - 5, LETRAY + SPACELETY*j);
            }else if(j == 0){
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], smallF, LETRAX + SPACELETX*i + 3, LETRAY + SPACELETY*j+40);
            }else if(j == CANTOP-1){
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], smallF, LETRAX + SPACELETX*i + 3, LETRAY + SPACELETY*j-15);
            }else if(j == 1){
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], mediumF, LETRAX + SPACELETX*i, LETRAY + SPACELETY*j+5);
            }else{
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], mediumF, LETRAX + SPACELETX*i, LETRAY + SPACELETY*j+5);  
            }
        }
    }
    //salida.textoList = addText(salida.textoList, ">", largeF, LETRAX - 35 , LETRAY + SPACELETY* 2); //se añade el cursor
    salida.textoList = addText(salida.textoList, scoreActual, bigF, SCOREX - (scoreLen/2.0)*58, SCOREY); //se añade el score 
    
    salida.spriteList = addSprite(salida.spriteList, "game/spritesAllegro/Selector1.png", LETRAX -15, LETRAY + 123);
    lists->textoList= salida.textoList;
    lists->spriteList=salida.spriteList;
    return lists;
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

sprite_t * changeCol(sprite_t * toshow, int nextOp){

    sprite_t * puntero = toshow; //Se crea un puntero temporal a la lista de textos

    puntero->posx = LETRAX - 15 + nextOp*SPACELETX; //Se mueve el selector
    return toshow;
}

/***********************************************************************************************************************************************************
 * 
 *                                                                      LIDERBOARD
 * 
 * ********************************************************************************************************************************************************/

TextObj_t * allegroLiderboard(menu_t * data, TextObj_t * lists){
    
    int i;
    TextObj_t salida = {NULL,NULL};

    float firstLen = strlen(data->textOpciones[0]);
    float secondLen = strlen(data->textOpciones[1]);

    salida.textoList=addText(salida.textoList, data->textOpciones[0], bigF, FIRSTX -(firstLen/2.0)*40 , FIRSTY);
    salida.textoList=addText(salida.textoList, data->textOpciones[1], largeF, FIRSTX -(secondLen/2.0)*25 , FIRSTY + 120);

    for(i=2; i < data->cantOpciones; i++){
        if(i%2 == 0){
            salida.textoList=addText(salida.textoList, data->textOpciones[i], mediumF, LEFTCOLUMNX , COLUMNY + 60*i/2);
        }else{
            salida.textoList=addText(salida.textoList, data->textOpciones[i], mediumF, RIGHTCOLUMNX , COLUMNY + 60* (i-1)/2);
        }
    }

    lists->spriteList= salida.spriteList;
    lists->textoList= salida.textoList;
    return lists;
}

/***********************************************************************************************************************************************************
 * 
 *                                                                      LEVELS
 * 
 * ********************************************************************************************************************************************************/

texto_t * levelAllegro(texto_t * toText, char * score, char * vidas ){

    texto_t * temp = toText;

    temp = addText(temp, vidas, mediumF, X_MAX - 50, Y_MAX - 50);
    temp = addText(temp, "Lives :", mediumF, X_MAX - 200, Y_MAX - 50);
    temp = addText(temp, score, mediumF, 20, Y_MAX - 50);

    return temp;
}

int expo(int num, int exp){

    int temp = num;
    if (exp == 0){
        return 1;
    }
    else{
        for (int i = 0; i < exp - 1; i++){
            num = num * temp;
        }
    }

    return num;
}

int techo(float num){

    int i;
    for(i=0; i<10; i++){
        if(num < i+1 && num >= i){
            return i;
        }
    }
    return 0;
}

void refreshDatos( char * toScore, char * toVidas, int score, int vidas){

    int  i; 
    float temp;
    toVidas[0] = '0' + vidas;

    for( i = 0; i < 6; i++){

        temp = (float)((score % (expo(10, i)*10)) - (score % expo(10, i)));

        toScore[5 - i] = '0' + techo(temp / expo(10, i));
        
    }

}

