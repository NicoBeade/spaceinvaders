#include "allegro.h"
#include <allegro5/allegro.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>

#define X_MAX 900               //Ancho maximo de la pantalla. Vamos a tomar la primer posicion como 0.
#define Y_MAX 760               //Alto maximo de la pantalla
#define U_SEC2M_SEC 1000        //Conversion de micro segundos a milisegundos.
#define TAMLETRAX 24
#define TAMLETRAY 36

enum {smallF, mediumF, largeF, bigF, FONTMAX};

/*******************************************************************************************************************************************
 *
 *                                                      OUTPUT UTILITIES 
 * 
 ******************************************************************************************************************************************/

    /**************************************************************************
     Datos que recibe el thread del output
    ***************************************************************************/

typedef struct 
{
    ALLEGRO_EVENT_QUEUE ** event_queue;

    punteros_t punteros;
    texto_t ** text;

    bool * close_display;
    bool * displayFlag;

}output_data_t;

void * displayt (ALLEGRO_THREAD * thr, void * dataIn);

/*******************************************************************************************************************************************
 *
 *                                                      INPUT UTILITIES 
 * 
 ******************************************************************************************************************************************/


typedef struct {

    ALLEGRO_EVENT_QUEUE ** event_queue;
    ALLEGRO_EVENT * ev; 

    keys_t * keys;

    bool * close_display;
    bool * keyboardDownFlag;
    bool * keyboardUpFlag;
    int * keycode;

} keyboard_data_t;

void * keyboardt(ALLEGRO_THREAD * thr, void * dataIn);