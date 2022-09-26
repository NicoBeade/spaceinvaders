#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>


/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/
typedef struct{//Esta estructura define un vector para las coordenadas
    int x;
    int y;
}vector_t;

typedef struct ALIEN{//Cada uno de los aliens sera una estructura de este tipo. Todos los aliens estan en una lista
    vector_t pos;//Posicion del alien
    int type;//Tipo de nave
    int lives;//Cantidad de vidas del alien (Distintos tipos de alien pueden tener mas o menos vidas)
    struct ALIEN * next;//Puntero al siguiente alien de la lista.
}alien_t;

typedef struct BALA{//Cada uno de las balas sera una estructura de este tipo. Todas las balas estan en una lista
    vector_t pos;//Posicion de la bala
    //int type;//Tipo de bala
    struct BALA * next;//Puntero al siguiente alien de la lista.
}bala_t;
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                     ___                     _                   _              
                                    / __|  ___   _ _    ___ | |_   __ _   _ _   | |_   ___   ___
                                   | (__  / _ \ | ' \  (_-< |  _| / _` | | ' \  |  _| / -_) (_-<
                                    \___| \___/ |_||_| /__/  \__| \__,_| |_||_|  \__| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/


//*************DIRECCIONES
#define IZQUIERDA -1        //Constantes utilizadas para indicar la direccion en la que se deben mover los aliens.
#define DERECHA 1
#define ABAJO 2

//*************TIMER
#define U_SEC2M_SEC 1000    //Conversion de micro segundos a milisegundos.

//*************Definidas en INPUT.h

//*************Definidas en DISPLAY.h
#define X_MAX 20              //Ancho maximo de la pantalla
#define Y_MAX 20              //Alto maximo de la pantalla

#define MARGEN_X 1          //Margen horizontal en el display
#define MARGEN_Y 1          //Margen vertical en el display

#define DESPLAZAMIENTO_X 1  //Indica cuanto de debe mover una nave en la coordenada x.
#define DESPLAZAMIENTO_Y 1  //Indica cuanto se debe mover una nave en la coordenada y.

#define ESP_ALIENS_X 1      //Espacio vacio entre los aliens en la coordenada X.
#define TAM_ALIENS_X 3      //Tamano que ocupan los aliens en la coordenada X.
#define ESP_ALIENS_Y 1      //Espacio vacio entre los aliens en la coordenada Y.
#define TAM_ALIENS_Y 3      //Tamano que ocupan los aliens en la coordenada Y.

#define DIST_INICIAL_X 6    //Distancia de los aliens respecto del borde izquierdo de la pantalla al iniciar un nivel, sin offset por nave
#define DIST_INICIAL_Y 4    //Distancia de los aliens respecto del borde superior de la pantalla al iniciar un nivel, sin offset por nave

#define CANT_MAX_ALIENS 12  //Cantidad maxima de aliens, dependiendo del nivel se muestra distinta cantidad, con este tope.
#define CANT_MAX_FILAS 4    //CAntidad maxima de filas, dependiendo del nivel se muestra una cantidad distinta, con este tope.
#include "aliensYBalas.h"
enum alienTypes {NODRIZA, SUPERIOR, MEDIO, INFERIOR};
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                 ___               _           _     _                   
                                | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___
                                |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<
                                |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/
                                                                        |_|                                                            
 * 
 ******************************************************************************************************************************************/
void * timer();     //Funcion encargada de controlar el tiempo del juego.
                                        //Elimina de heap la lista creada.

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                __   __                  ___   _         _             _            
                                \ \ / /  __ _   _ _     / __| | |  ___  | |__   __ _  | |  ___   ___
                                 \ V /  / _` | | '_|   | (_ | | | / _ \ | '_ \ / _` | | | / -_) (_-<
                                  \_/   \__,_| |_|      \___| |_| \___/ |_.__/ \__,_| |_| \___| /__/
                                                                                                                                                      
 * 
 ******************************************************************************************************************************************/
unsigned int timerTick = 1000000;  //Esta variable es un "contador" que se decrementa cada cierto tiempo y con ella se controla cada cuanto tiempo se
                                    //ejecuta cada accion del programa.

int velAliens = 200;  /*Determina que tan rapido se moveran los aliens. La conversion es: si velAliens = 1, entonces moveAlien se ejecuta cada 10mS
                                                                        Para ejecutar velAliens cada 1s velAliens debe valer 100.*/

unsigned int vidas;      //Indica las vidas restantes del usuario puede ser un campo del struct

//int aliensRestantes;    //Indica cuantos aliens quedan en la partida

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

int main(void) {

    srand(time(NULL));

    pthread_t timerT, moveAlienT;//Punteros de los threads
    
    int aliensTotales = 12;
    int filasAliens = 4;
    level_setting_t settings;
    settings.yMax = 15;
    settings.yMin = 0;
    settings.xMax = 15;
    settings.xMin = 0;
    settings.saltoX = 4;
    settings.saltoY = 3;
    settings.initDanielLives = 1;
    settings.initNicolasLives = 2;
    settings.initPabloLives = 3;
    settings.distInicialX = 6;
    settings.distInicialY = 2;
    
    alien_t* listAlien = NULL;//Puntero al primer elemento de la lista de los aliens.
    listAlien = initAliens(listAlien, &settings, "20403", PABLO, DANIEL, NICOLAS);

//************************************* Esta seccion del codigo se usa para probar que funcionen las listas *****************************
    int i = 1;
    alien_t* prueba = listAlien;
    while(prueba != NULL){
        printf("Alien %d: x: %d ; y: %d ; tipo: %d ; vidas: %d\n", i, prueba -> pos.x, prueba -> pos.y, prueba -> type, prueba -> lives);
        i++;
        prueba = prueba -> next;
    }

//****************************************************************************************************************************************
    

    if( pthread_create(&timerT, NULL, timer, NULL) ){//Inicializa el thread timer.
        printf("No se pudo crear el thread timer\n");
    }

    sleep(1);

    if( pthread_create(&moveAlienT, NULL, moveAlien, (void*) listAlien) ){//Inicializa el thread move alien.
        printf("No se pudo crear el thread moveAlien\n");
    }

    pthread_join(timerT, NULL);
    pthread_join(moveAlienT, NULL);

    removeAlienList(listAlien);//Borra del heap la lista de los aliens.

    return 0;
}


/*******************************************************************************************************************************************
 * 
                                                 _____   _                     
                                                |_   _| (_)  _ __    ___   _ _ 
                                                  | |   | | | '  \  / -_) | '_|
                                                  |_|   |_| |_|_|_| \___| |_|  
 * 
 ******************************************************************************************************************************************/
void * timer(){
/* Este thread es el encargado de controlar el tiempo del juego. Cuenta de una variable que se decrementa cada 10mS luego el resto de los
    threads utilizan esta variable para determinar cuando se deben ejecutar.
*/
    printf("Timer set\n");
    while(1){
        usleep(10 * U_SEC2M_SEC); //Sleep 10mS.
        timerTick -= 1;
    }
    pthread_exit(0);
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/
