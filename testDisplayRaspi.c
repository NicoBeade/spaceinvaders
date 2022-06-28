#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>
#include "aliensYBalas.h"
#include "utilidades.h"
#include "displayRaspi.h"


int velAliens = 50;
unsigned int timerTick = 1000000;

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


int main (void){
    disp_init();
    object_t * listAliens = NULL;

    level_setting_t levelSettings = {0, 15, 0, 15, 4, 3, 1, 1, 3, 1, 1, 1};
/*
    typedef struct{
    int xMin;               //-xMax: coordenada maxima en x alcanzable.
    int xMax;               //-xMin: coordenada minima en x alcanzable.
    int yMin;               //-YMax: coordenada maxima en Y alcanzable.
    int yMax;               //-YMin: coordenada minima en Y alcanzable.
    int saltoX;             //-saltoX: distancia entre naves en x
    int saltoY;             //-saltoy: distancia entre naves en y (linea)
    int distInicialX;       //-distInicialX: coordenada en X de la nave del centro de la primera fila
    int distInicialY;       //-distInicialY: coordenada en Y de la nave del centro de la primera fila
    int initUsrLives;       //-initUsrLives: Vidas del usuario en ese nivel
    int initDanielLives;    //-initUsrLives: Vidas de la nave enemiga Daniel en ese nivel
    int initPabloLives;     //-initUsrLives: Vidas de la nave enemiga Pablo en ese nivel
    int initNicolasLives;   //-initUsrLives: Vidas de la nave enemiga Nicolas en ese nivel
    //ESPACIO PARA BARRERAS
    //AAA BARRERAS
    }level_setting_t;
*/
    argMoveAlien_t argMoveAlien = {listAliens, 1, 1, 15, 1, 15, 1, 3};
/*
    typedef struct{//Este es el tipo de dato que recibe el thread de moveAlien
    object_t * alien;//Necesita un puntero al primer elemento de la lista de los aliens.
    int desplazamientoX;//Y cuanto se deben mover las naves en cada tick.
    int desplazamientoY;
    int xMax;//Coordenadas maximas del display.
    int margenX;//Margen maxiom al que pueden llegar las naves respecto del borde del display.
    int yMax;
    int margenY;
    int tamAlienX;//Hitbox del alien en la coordenada X.
    }argMoveAlien_t;
*/

    char filas[] = "444";

    printf("Anashe\n");

    listAliens = initAliens(listAliens, &levelSettings, filas, DANIEL, PABLO, NICOLAS);//Inicializa la lista de los aliens

    pthread_t Ttimer, TmoveAliens, TdisplayRaspi;

    argDisplayRPI_t argumentosDisplayRPI = {0, listAliens};

    printf("Anashe 1\n");

    pthread_create(&Ttimer, NULL, timer, NULL);

    pthread_create(&TdisplayRaspi, NULL, displayRPI, &argumentosDisplayRPI);

    pthread_create(&TmoveAliens, NULL, moveAlien, &argMoveAlien);

    printf("Anashe 2\n");

    pthread_join(Ttimer, NULL);
    pthread_join(TdisplayRaspi, NULL);
    pthread_join(TmoveAliens, NULL);

    removeAlienList(listAliens);

    return 0;
}