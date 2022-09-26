#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>
#include "aliensYBalas.h"
#include "utilidades.h"
#include <semaphore.h>
#include "allegro.h"
#include "displayAllegro.h"
#include "inputAllegro.h"
#include <time.h>

sem_t semaforo;

int velAliens = 100;
int velBalas = 1;
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

    srand(time(NULL));

    object_t * listAliens = NULL;

    object_t * naveUsuario = (object_t*) malloc(sizeof(object_t));//Crea la nave del usuario

    object_t * listaBalasEnemigas = NULL;

    object_t * listaBalasUsuario = NULL;
    
    if(naveUsuario == NULL){\
        printf("no se pudo crear al usuario por malloc \n");
    }

    naveUsuario -> pos.x = 6;   //Inicializa la nave del usuario.
    naveUsuario -> pos.y = Y_MAX - NAVEY - 10;
    naveUsuario -> type = NAVE;
    naveUsuario -> lives = 1;
    naveUsuario -> next = NULL;

/*typedef struct{//Datos necesarios para iniciar un nivel.
    int xMin;                   //-xMax: coordenada maxima en x alcanzable.
    int xMax;                   //-xMin: coordenada minima en x alcanzable.
    int yMin;                   //-YMax: coordenada maxima en Y alcanzable.
    int yMax;                   //-YMin: coordenada minima en Y alcanzable.
    int saltoX;                 //-saltoX: distancia entre naves en x
    int saltoY;                 //-saltoy: distancia entre naves en y (linea)
    int maxUsrBullets;          //-maxUsrBullets: cantidad maxima de balas de la nave del usuario concurrentes
    int maxEnemyBullets;        //-maxEnemyBullets: cantidad maxima de balas enemigas concurrentes
    int shootProbDani;          //-shootProbDani: probabilidad de disparo de naves Daniel en cada tick en procentaje
    int shootProbPablo;         //-shootProbPablo: probabilidad de disparo de naves Pablo en cada tick en procentaje
    int shootProbNico;          //-shootProbNico: probabilidad de disparo de naves Nicolas en cada tick en procentaje
    int distInicialX;           //-distInicialX: coordenada en X de la nave del centro de la primera fila
    int distInicialY;           //-distInicialY: coordenada en Y de la nave del centro de la primera fila
    int barreraInicialX;        //-barreraInicialX: coordenada en X de la primera barrera
    int barreraInicialY;        //-barreraInicialY: coordenada en Y de la primera barrera
    int initUsrLives;           //-initUsrLives: Vidas del usuario en ese nivel
    int initDanielLives;        //-initUsrLives: Vidas de la nave enemiga Daniel en ese nivel
    int initPabloLives;         //-initUsrLives: Vidas de la nave enemiga Pablo en ese nivel
    int initNicolasLives;       //-initUsrLives: Vidas de la nave enemiga Nicolas en ese nivel
    int anchoUsr;               //-anchoUsr: Ancho de la nave del usuario
    int altoUsr;                //-altoUsr: Alto de la nave del usuario
    int anchoNave;              //-anchoNave: Ancho de las naves enemigas
    int altoNave;               //-altoNave: Alto de las naves enemigas
    int anchoMiniBarrera;       //-anchoMiniBarrera: Ancho de las minibarreras
    int altoMiniBarrera;        //-altoMiniBarrera: Alto de las minibarreras
    int miniBarreraLives;       //-miniBarreraLives: Vidas de cada minibarrera
}level_setting_t;
*/

    level_setting_t levelSettings; //{0, X_MAX, 0, Y_MAX, 49 + 30 , 49 + 10, 10, 50, 50, 50, 6, X_MAX/2, 20, 1, 1, 1, {1,1}};
    levelSettings.yMin = 0;
    levelSettings.xMax = X_MAX;
    levelSettings.yMin = 0;
    levelSettings.yMax = Y_MAX;
    levelSettings.saltoX = 49+30;
    levelSettings.saltoY = 49+10;
    levelSettings.maxUsrBullets = 3;
    levelSettings.shootProbDani = 50;
    levelSettings.shootProbNico = 70;
    levelSettings.shootProbPablo = 30;
    levelSettings.distInicialX = X_MAX/2;
    levelSettings.distInicialY = 20;
    levelSettings.initDanielLives = 2;
    levelSettings.initDanielLives = 2;
    levelSettings.initDanielLives = 2;
    levelSettings.anchoUsr = NAVEX;
    levelSettings.altoUsr = NAVEY;
    levelSettings.altoNave = NAVEY;
    levelSettings.anchoNave = NAVEX;
    levelSettings.anchoMiniBarrera = 10; 
    levelSettings.altoMiniBarrera = 10;
    levelSettings.miniBarreraLives = 2;

/*
    typedef struct{
    int xMin;                   //-xMax: coordenada maxima en x alcanzable.
    int xMax;                   //-xMin: coordenada minima en x alcanzable.
    int yMin;                   //-YMax: coordenada maxima en Y alcanzable.
    int yMax;                   //-YMin: coordenada minima en Y alcanzable.
    int saltoX;                 //-saltoX: distancia entre naves en x
    int saltoY;                 //-saltoy: distancia entre naves en y (linea)
    int maxEnemyBullets;        //-maxEnemyBullets: cantidad maxima de balas enemigas concurrentes
    int shootProbDani;          //-shootProbDani: probabilidad de disparo de naves Daniel en cada tick en procentaje
    int shootProbPablo;         //-shootProbPablo: probabilidad de disparo de naves Pablo en cada tick en procentaje
    int shootProbNico;          //-shootProbNico: probabilidad de disparo de naves Nicolas en cada tick en procentaje
    int distInicialX;           //-distInicialX: coordenada en X de la nave del centro de la primera fila
    int distInicialY;           //-distInicialY: coordenada en Y de la nave del centro de la primera fila
    int initUsrLives;           //-initUsrLives: Vidas del usuario en ese nivel
    int initDanielLives;        //-initUsrLives: Vidas de la nave enemiga Daniel en ese nivel
    int initPabloLives;         //-initUsrLives: Vidas de la nave enemiga Pablo en ese nivel
    int initNicolasLives;       //-initUsrLives: Vidas de la nave enemiga Nicolas en ese nivel
    vector_t centerNaveOffset;  //-centerOffset: distancia de offset desde la esquina superior izq de la nave hasta el centro
    //ESPACIO PARA BARRERAS
    //AAA BARRERAS
    }level_setting_t;
*/

    char filas[] = "666";

    printf("Anashe\n");

    listAliens = initAliens(listAliens, &levelSettings, filas, DANIEL, PABLO, NICOLAS);//Inicializa la lista de los aliens

    argMoveAlien_t argMoveAlien = {&listAliens, 15, 15, X_MAX , 20, Y_MAX, 20, 49, &listaBalasEnemigas, &levelSettings};
    argMoveBala_t argMoveBala = {&listaBalasUsuario, &listaBalasEnemigas, 10,10,10,-10,Y_MAX, 0};
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
    /*
    typedef struct{
	object_t * balasUsr;
	object_t * balasEnemigas;
	int velocidadPablo;
	int velocidadDaniel;
	int velocidadNicolas;
	int velocidadUsr;
	int yMax;
	int yMin;
}argMoveBala_t;

*/ 

    data_allegro_t dataAllegro = {naveUsuario, listAliens, listaBalasEnemigas, &listaBalasUsuario, NULL, &levelSettings};

    pthread_t Ttimer, TmoveAliens, TdisplayRaspi, TupdateInputGame, Tallegro, TmoveBalas;


    printf("Anashe 1\n");

    sem_init(&semaforo, 0, 1);

    pthread_create(&Ttimer, NULL, timer, NULL);
    pthread_create(&Tallegro, NULL, allegroThread, &dataAllegro);
    pthread_create(&TmoveAliens, NULL, moveAlien, &argMoveAlien);
    pthread_create(&TmoveBalas, NULL, moveBalaThread, &argMoveBala);

    printf("Anashe 2\n");

    pthread_join(Ttimer, NULL);
    pthread_join(Tallegro, NULL);
    pthread_join(TmoveAliens, NULL);
    pthread_join(TmoveBalas, NULL);
    
    sem_destroy(&semaforo);

    removeAlienList(listAliens);

    return 0;
}