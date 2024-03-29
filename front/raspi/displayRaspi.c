/**********************************************************************************************************************************************************
 * 
                                    _   _               _                 ___                      _          
                                 __| | (_)  ___  _ __  | |  __ _   _  _  | _ \  __ _   ___  _ __  (_)      __ 
                                / _` | | | (_-< | '_ \ | | / _` | | || | |   / / _` | (_-< | '_ \ | |  _  / _|
                                \__,_| |_| /__/ | .__/ |_| \__,_|  \_, | |_|_\ \__,_| /__/ | .__/ |_| (_) \__|
                                                |_|                |__/                    |_|                
* 
 ***********************************************************************************************************************************************************
*   
*   Este archivo contiene las funciones y threads encargadas de todas las acciones relacionadas al manejo del display RPI.
*   Tiene las siguientes funcionalidades:
*       -Funciones para agregar informacion al buffer e imprimir cosas en el display.
*       -Thread que se encarga de actualizar el display durante la ejecucion del juego en la raspberry.
*       -Thread que se encarga del barrido de texto en el display durante la ejecucion de un menu (menu de inicio, pausa, etc.).
*       -Funciones que indican que hacer cuando se selecciona una opcion en un menu.
*
 **********************************************************************************************************************************************************/


#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "displayRaspi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "drivers/termlib.h"
#include "drivers/disdrv.h"
#include "../../back/spaceLib/score/score.h"
#include <string.h>


typedef struct{//Argumentos que recibe el thread que muestra el dibujo de cada opcion.
    halfDisp_t* higerDispMenu;      //parte superior del display.
    int direccion;
    halfDisp_t* drawing;            //Dibujo a mostrar.
}argSwipeDrawing_t;


/*******************************************************************************************************************************************
 * 
                                     ___                     _                   _              
                                    / __|  ___   _ _    ___ | |_   __ _   _ _   | |_   ___   ___
                                   | (__  / _ \ | ' \  (_-< |  _| / _` | | ' \  |  _| / -_) (_-<
                                    \___| \___/ |_||_| /__/  \__| \__,_| |_||_|  \__| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/

#define DISP_MAX 15                 //Tamanyos
#define DISP_MIN 0
#define ALTO_SPRITE 2
#define ANCHO_SPRITE 3
#define ALTO_LETRA 8
#define ANCHO_LETRA 4

#define OFFSETLETRA 0               //Constantes para desreferenciar letras
#define OFFSETALIEN 39
#define OFFSETCHARESP 36
#define OFFSETNUM 26

#define VEL_DISP_ANIMATION 3000     //Velocidades de animaciones
#define VEL_TITILEO 200
#define BARRIDO_LETRA 1000  
#define WAIT_SEEP_MENU 900

#define MAX_SPRITES 200


sem_t SEM_DRIVER;                   //Semaforo que regula que no se pisen los drivers

int velDispAnimation = 2;           //Velocidad a la que se realiza el barrido del display durante un menu
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/



/*******************************************************************************************************************************************
 * 
             ___               _           _     _                          _           ___   _            _     _           
            | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___    __| |  ___    / __| | |_   __ _  | |_  (_)  __   ___
            |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<   / _` | / -_)   \__ \ |  _| / _` | |  _| | | / _| (_-<
            |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__| \__,_|  \__| |_| \__| /__/
                                                    |_|                                                                                                                                                                                                               
 * 
 ******************************************************************************************************************************************/
//*****************FUNCIONES GENERICAS DEL DISPLAY
static void drawSprite(dcoord_t, sprite_t); //prototipos  de dibujar y limpiar enemigos
static void clearBuffer(void); //borra los contenidos del buffer del display sin eliminar el del display
static void printLetter(caracteres_t letter, dcoord_t coordenada); //imprime letra en display en la coordenada correspondiente
static void printHalfDisp(halfDisp_t halfDispSprite, char mitad); //Imprime una mitad del display.

//*****************THREAD DISPLAY DURANTE MENUES
static int offsetAlfabeto(char caracter); //Se utiliza para obtener el offset necesario para acceder al string "alfabeto".
static void swipeCharacter(halfDisp_t* lowerDispMenu, caracteres_t caracter, int direccion); //Agrega un caracter completo al buffer.
static void copyMatrixLetter(char,uint8_t [8][4]); //Copia de matrices de letra
static void sweepMatrix(uint8_t [8][4], int ); //Barrido de letra x1 linea 
static void addRow(uint8_t [8][4], uint8_t [4], int ); //Agrega una linea en la matriz con las celdas correspondientes
static void* swipeDrawing(void*);//Muestra un dibujo en la parte superior del display.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/******************************************************************************************************************************************
 * 
  ___                      _                                _         _        _   _               _                   ___   ___   ___ 
 | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___  | |    __| | (_)  ___  _ __  | |  __ _   _  _    | _ \ | _ \ |_ _|
 | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_) | |   / _` | | | (_-< | '_ \ | | / _` | | || |   |   / |  _/  | | 
 |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___| |_|   \__,_| |_| /__/ | .__/ |_| \__,_|  \_, |   |_|_\ |_|   |___|
                                                                                           | |
 * 
 ******************************************************************************************************************************************/

//IMPORTANTE: AL DIBUJAR O LIMPIAR EL DISPLAY HAY QUE LLAMAR A disp_update() Y DEBE HABER SIDO INICIALIZADO CON disp_init()

void dispInit(void){
    disp_init();
}

static void drawSprite(dcoord_t p, sprite_t alien){ //Esta funcion imprime en display un enemigo en un sprite dados en la posicion p
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<ANCHO_SPRITE ; i++){
        for(j=0 ; j<ALTO_SPRITE ; j++){
            if (alien [j][i] == 1){
                pAux.x = p.x+i;
                pAux.y = p.y+j;
                if(pAux.x>DISP_MAX||pAux.y>DISP_MAX){
                    printf("Fuera de rango de impresion en algun pixel de la nave\n");
                }
                disp_write(pAux,D_ON); //Actualiza el buffer          
            }
        }
    }
}

static void clearBuffer(void){ //Esta limpia el buffer
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<=DISP_MAX ; i++){
        for(j=0 ; j<=DISP_MAX ; j++){
            pAux.x = i;
            pAux.y = j;
            disp_write(pAux, D_OFF); //Actualiza el buffer          
        }
    }
}

static void printLetter(caracteres_t letter, dcoord_t coordenada){ //imprime una letra barriendo los 32 pixeles de una matriz de 8X4
    int i,j;
    dcoord_t punto;
    for (i=0; i<ALTO_LETRA; i++){
        for (j=0; j<ANCHO_LETRA; j++){
            punto.x=j+coordenada.x;
            punto.y=i+coordenada.y;
            if (letter [i][j]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    sem_wait(&SEM_DRIVER);
    disp_update();
    sem_post(&SEM_DRIVER);
}

static void printHalfDisp(halfDisp_t halfDispSprite, char mitad){ //imprime la mitad de la pantalla barriendo los 128 pixeles de una matriz
    int i,j,offset;                                        //se puede elegir cual mitad del display ingresando I (inf) o S (sup)
    if (mitad=='I'){
        offset=8;
    }
    else if(mitad=='S'){
        offset=0;
    }
    else {
        printf("Letra no valida para la impresion de medio display\n");
        return;
    }
    dcoord_t punto;
    for (i=0; i<((DISP_MAX+1)/2); i++){
        for (j=0; j<=DISP_MAX; j++){
            punto.x=j;
            punto.y=i+offset;
            if (halfDispSprite [i][j]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    sem_wait(&SEM_DRIVER);
    disp_update();
    sem_post(&SEM_DRIVER);
}

void initSem(void){
    sem_init(&SEM_DRIVER, 0, 1);
}

void clearDisplay(void){
    sem_wait(&SEM_DRIVER);
    disp_clear();
    sem_post(&SEM_DRIVER);
}
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

/******************************************************************************************************************************************
 * 
  _____   _                            _        _         _        _   _               _                   ___   ___   ___ 
 |_   _| | |_    _ _   ___   __ _   __| |    __| |  ___  | |    __| | (_)  ___  _ __  | |  __ _   _  _    | _ \ | _ \ |_ _|
   | |   | ' \  | '_| / -_) / _` | / _` |   / _` | / -_) | |   / _` | | | (_-< | '_ \ | | / _` | | || |   |   / |  _/  | | 
   |_|   |_||_| |_|   \___| \__,_| \__,_|   \__,_| \___| |_|   \__,_| |_| /__/ | .__/ |_| \__,_|  \_, |   |_|_\ |_|   |___|
                                                                               |_|                |__/                     
 * 
 ******************************************************************************************************************************************/

int displayRPI (argDisplay_t* argDisplayRPI){

    if(argDisplayRPI->pantalla){
        char fueraDeRango = 0;

        object_t* balasEnemigas;
        object_t* balasUsr;
        object_t* aliens;
        object_t* naveUser;
        object_t* barriers;
        object_t* mothership;

        dcoord_t punto; //punto del display a escribir

        fueraDeRango = 0;

        balasEnemigas = *(argDisplayRPI->balasEnemigas); //Puntero a la lista de balas enemigas
        balasUsr = *(argDisplayRPI->balasUsr); //Puntero a la lista de balas del usuario
        aliens = *(argDisplayRPI->aliens); //Puntero a la lista de aliens
        naveUser = *(argDisplayRPI->naveUser); //Puntero a la nave del usuario
        barriers = *(argDisplayRPI->barriers); //Puntero a la nave nodriza
        mothership = *(argDisplayRPI->mothership); //Puntero a la nave nodriza

        clearBuffer(); //limpio el buffer            
        //Actualizo el buffer con la nueva posicion de los aliens
        object_t* aux = aliens;
        objectType_t * objTypePointer;      //Variable auxiliar que almacena el objectType de cada elemento
        while (aliens!= NULL){ //mientras no se haya llegado al final de la lista

            punto.x=aliens->pos.x; //se definen posiciones en x y en y de los aliens, tomando como pivote la esquina superior izquierda
            punto.y=aliens->pos.y;

            if (punto.x>DISP_MAX||punto.y>DISP_MAX||punto.x<DISP_MIN||punto.y<DISP_MIN){
                printf("Fuera de rango de impresion en la nave\n"); //chequeo de pixel a imprimir
                fueraDeRango = 1;
            }
            objTypePointer = getObjType(aliens->type);
            if(objTypePointer == NULL){ //Si no encuentra el object type devuelve null
                printf("Error in displayRaspi.c, displayRPIThread function : Object type %d not found\n", aliens->type);
                return -1;
            }
            int sprite1 = atoi(objTypePointer->sprite1);
            int sprite2 = atoi(objTypePointer->sprite2);
            int sprite;
            if(sprite1 < 1 || sprite1 > MAX_SPRITES){
                printf("Error in displayRaspi.c displayRPIThread function : sprite: %s (atoi %d, id %d) has an invalid format\n", objTypePointer->sprite1, sprite1, objTypePointer->id);
                return -1; 
            }
            if(sprite1 < 1 || sprite2 > MAX_SPRITES){
                printf("Error in displayRaspi.c,231 displayRPIThread function : sprite: %s (atoi %d, id %d) has an invalid format\n", objTypePointer->sprite2, sprite2, objTypePointer->id);
                return -1;
            }
            if (aliens->animationStatus%2){ //chequeo de estado de animacion, se imprime un sprite u otro dependiendo de cuantas veces se haya desplazado un alien
                sprite = sprite1;
            }
            else{
                sprite = sprite2;
            }
            switch(sprite){ //dependiendo del estado de animacion y el alien a imprimir, se imprime un sprite distinto
                case 1:
                    drawSprite(punto,daniel1);
                    break;
                case 2:
                    drawSprite(punto,daniel2);
                    break;
                case 3:
                    drawSprite(punto,nicolas1);
                    break;
                case 4:
                    drawSprite(punto,nicolas2);
                    break;
                case 5:
                    drawSprite(punto,pablo1);
                    break;
                case 6:
                    drawSprite(punto,pablo2);
                    break;
                
                default: printf("Se esta queriendo imprimir como alien algo que no es un alien\n");
                    break;
                
            }   
            aliens=aliens->next; //se pasa al siguiente alien en la lista
        }
        aliens=aux; //devuelve aliens al principio de la lista

        if(naveUser != NULL){
            punto.x=naveUser->pos.x; //posicion en x y en y de la nave
            punto.y=naveUser->pos.y;
            drawSprite(punto,nave); //copia la nave en el buffer
        }
        else{
            printf("Err in displayRaspi.c, in fuction displayRPI: usrList cannot be NULL in displayRPI\n");
        }
        
        aux = balasEnemigas;

        while (balasEnemigas!= NULL){ //mientras no se haya llegado al final de la lista

            punto.x=balasEnemigas->pos.x; //se definen posiciones en x y en y de las balas, tomando como pivote la esquina superior izquierda
            punto.y=balasEnemigas->pos.y;
            if (punto.x>DISP_MAX||punto.y>DISP_MAX||punto.x<DISP_MIN||punto.y<DISP_MIN){
                printf("Fuera de rango de impresion en la bala enemiga \n"); //chequea de pixel a imprimir
                fueraDeRango = 1;
            }
            else{   
                disp_write(punto,D_ON);
            }
            balasEnemigas = balasEnemigas -> next;
        }
        balasEnemigas = aux;

        aux = balasUsr;
        while (balasUsr!= NULL){ //mientras no se haya llegado al final de la lista

            punto.x=balasUsr->pos.x; //se definen posiciones en x y en y de las balas, tomando como pivote la esquina superior izquierda
            punto.y=balasUsr->pos.y;
            if (punto.x>DISP_MAX||punto.y>DISP_MAX||punto.x<DISP_MIN||punto.y<DISP_MIN){
                printf("Fuera de rango de impresion en la bala aliada\n"); //chequea de pixel a imprimir
                fueraDeRango = 1;
            }
            else{   
                disp_write(punto,D_ON);
            }
            balasUsr = balasUsr -> next;
        }
        balasUsr = aux;

        aux = barriers;
        while (barriers!= NULL){ //mientras no se haya llegado al final de la lista

            punto.x=barriers->pos.x; //se definen posiciones en x y en y de las balas, tomando como pivote la esquina superior izquierda
            punto.y=barriers->pos.y;
            if (punto.x>DISP_MAX||punto.y>DISP_MAX||punto.x<DISP_MIN||punto.y<DISP_MIN){
                printf("Fuera de rango de impresion de las barreras\n"); //chequea de pixel a imprimir
                fueraDeRango = 1;
            }
            else{   
                disp_write(punto,D_ON);
            }
            barriers = barriers -> next;
        }
        barriers = aux;
        punto.y=0;

        if(*(argDisplayRPI->mothership) != NULL){
            if(mothership->pos.x>=DISP_MIN&&mothership->pos.x<=DISP_MAX){
                punto.x=mothership->pos.x;
                disp_write(punto,D_ON);
            }
            if(mothership->pos.x+1>=DISP_MIN&&mothership->pos.x+1<=DISP_MAX){
                punto.x=mothership->pos.x+1;
                disp_write(punto,D_ON);
            }    
            if(mothership->pos.x+2>=DISP_MIN&&mothership->pos.x+2<=DISP_MAX){
                punto.x=mothership->pos.x+2;
                disp_write(punto,D_ON);
            }
        } 


        if(fueraDeRango == 0){
            sem_wait(&SEM_DRIVER);
            disp_update(); //se transfiere del buffer al display de la RPI
            sem_post(&SEM_DRIVER);
        }
        else if(fueraDeRango == 1){
            return -1;
        }
    }
    return 0;
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/******************************************************************************************************************************************
 * 
             _____   _                            _        _   _               __  __                         ___   ___   ___ 
            |_   _| | |_    _ _   ___   __ _   __| |    __| | (_)  ___  _ __  |  \/  |  ___   _ _    _  _    | _ \ | _ \ |_ _|
              | |   | ' \  | '_| / -_) / _` | / _` |   / _` | | | (_-< | '_ \ | |\/| | / -_) | ' \  | || |   |   / |  _/  | | 
              |_|   |_||_| |_|   \___| \__,_| \__,_|   \__,_| |_| /__/ | .__/ |_|  |_| \___| |_||_|  \_,_|   |_|_\ |_|   |___|
                                                                       |_|                                                         
 * 
 ******************************************************************************************************************************************/

void* textAnimMenu(void* argTextAnimMenu){
//Este thread es el que se encarga de realizar el barrido de texto durante la ejecucion de un menu.
    
    argTextAnimMenu_t * data = (argTextAnimMenu_t*)argTextAnimMenu;

    int i, j, offset;
    int firstBarr = 4;
    pthread_t drawingSwipeT;

    int firstLetter = ( (data -> direccion) == DERECHA ) ? 3 : 0;
    int lastLetter = ( (data -> direccion) == DERECHA ) ? -1 : 4;
    
    //Primero imprimimos las primeras 4 letras y el dibujo.
    if(data->higherDispMenu == NULL){
        printf("Error in displayRaspi.c, higherDispMenu cannot be NULL in textAnimMenu\n");
        pthread_exit(0);
    }
    if(data->lowerDispMenu == NULL){
        printf("Error in displayRaspi.c, lowerDispMenu cannot be NULL in textAnimMenu\n");
        pthread_exit(0);
    }
    if(data->drawing == NULL){
        printf("Error in displayRaspi.c, drawing cannot be NULL in textAnimMenu\n");
        pthread_exit(0);
    }

    if((data->notSwipe) == 0){
        int i;
        caracteres_t* caracter;
        dcoord_t pos = { 0, 8 };
        printHalfDisp(*(data->drawing), 'S');
        for(i = 0 ; i < 4 ; i++){
            offset = offsetAlfabeto((data -> msg)[i]);
            caracter = alfabeto[offset];
            printLetter(*caracter, pos);
            pos.x += 4;
        }
    }
    else{
        argSwipeDrawing_t argSwipeDrawing = { data->higherDispMenu, data->direccion, data->drawing };
        pthread_create(&drawingSwipeT, NULL, swipeDrawing, &argSwipeDrawing);//Agrega el dibujo.
        for(i = firstLetter ; i != lastLetter ; i -= (data -> direccion)){

        offset = offsetAlfabeto((data -> msg)[i]);
        if(offset == -1){
            pthread_exit(0);
        }
        swipeCharacter(data -> lowerDispMenu, *(alfabeto[offset]), data -> direccion);
    }
    }
    
    pthread_join(drawingSwipeT, NULL);
    

    usleep(WAIT_SEEP_MENU * U_SEC2M_SEC);//Espera medio segundo.

    if(velDispAnimation != 1){
        velDispAnimation = VEL_DISP_ANIMATION;
    }

    do{//Barre el texto hasta que se le indique lo contrario.
        if((data->notSwipe) == 1){//Si esta en el menu de volumen no hay que barrer el texto.
            for(j = firstBarr ; (data -> msg)[j] != '\0' ; j++){//Barre todas las letras del texto.

                offset = offsetAlfabeto((data -> msg)[j]);
                if(offset == -1){
                    pthread_exit(0);
                }
                swipeCharacter(data -> lowerDispMenu, *(alfabeto[offset]), IZQUIERDA);       
            }
            firstBarr = 0;//Reinicia el proceso.
        }
    }
    while(*(data -> changeAnimation));

    velDispAnimation = VEL_DISP_ANIMATION;
    pthread_exit(0);
}


static int offsetAlfabeto(char caracter){
//Esta funcion se utiliza para obtener el offset necesario para acceder al string "alfabeto".
    int offset;
    
    if(caracter >= 'a' && caracter <= 'z'){//Si es una letra minuscula.
        offset = caracter - 'a' + OFFSETLETRA;
    }
    else if(caracter >= 'A' && caracter <= 'Z'){//Si es una letra mayuscula.
        offset = caracter - 'A' + OFFSETLETRA;
    }
    else if(caracter >= '0' && caracter <= '9'){//Si es un numero.
        offset = caracter - '0' + OFFSETNUM;
    }
    else if(caracter == '-'){//Si es un guion.
        offset = OFFSETCHARESP;
    }
    else if(caracter == '.'){//Si es un punto.
        offset = OFFSETCHARESP + 1;
    }
    else if(caracter == ' '){//Si es un espacio.
        offset = OFFSETCHARESP + 2;
    }
    else if(caracter == '>'){//Si es un espacio.
        offset = OFFSETCHARESP + 6;
    }
    else{
        printf("Error in displayRaspi.c, invalid character %c in fuction offsetAlfabeto\n", caracter);
        offset = -1;
    }
    return offset;
}

static void swipeCharacter(halfDisp_t* lowerDispMenu, caracteres_t caracter, int direccion){
//Esta funcion agrega un caracter completo al buffer.
    int i, fil, col, colInicialB, colFinalB, colInicialL, colFinalL;
    
    colInicialB = (direccion == DERECHA) ? 14 : 1;//Define a partir de que columna se realiza el barrido.
    colFinalB = (direccion == DERECHA) ? -1 : 16;//Define la ultima columna que se barrera.
    colInicialL = (direccion == DERECHA) ? 3 : 0;//Define a partir de que columna se muestra la nueva letra.
    colFinalL = (direccion == DERECHA) ? -1 : 4;//Define a la ultima columna de la letra que se mostrara.

    for(i = colInicialL ; i != colFinalL ; i -= direccion){//Recorre todas las columnas del caracter.

        for(col = colInicialB ; col != colFinalB ; col -= direccion){//Barre el display hacia un costado.
            usleep(velDispAnimation);//Indica a que velocidad se debe hacer el barrido.
            
            for(fil = 0 ; fil < 8 ; fil++){
                (*lowerDispMenu)[fil][col + direccion] = (*lowerDispMenu)[fil][col];//Realiza el barrido.
            }
        }

        for(fil = 0 ; fil < 8 ; fil++){//Ingresa la columna de la nueva letra al buffer.
            (*lowerDispMenu)[fil][colFinalB + direccion] = caracter[fil][i];
        }
        
        printHalfDisp(*lowerDispMenu, 'I');//Muestra el contenido en el display.
    }
    
}

static void* swipeDrawing(void* data){
/*Este thread se encarga de mostrar los dibujos de la parte superior del display durante un menu.
    Recibe un puntero a la parte superior del display, la direccion en la que se debe mover y un puntero al dibujo a mostrar.*/

    argSwipeDrawing_t* argSwipeDrawing = (argSwipeDrawing_t*)data;

    int i, fil, col, colInicialB, colFinalB, colInicialD, colFinalD;
    
    colInicialB = (argSwipeDrawing -> direccion == DERECHA) ? 14 : 1;//Define a partir de que columna se realiza el barrido.
    colFinalB = (argSwipeDrawing -> direccion == DERECHA) ? -1 : 16;//Define la ultima columna que se barrera.
    colInicialD = (argSwipeDrawing -> direccion == DERECHA) ? 15 : 0;//Define a partir de que columna se muestra la nueva letra.
    colFinalD = (argSwipeDrawing -> direccion == DERECHA) ? -1 : 16;//Define a la ultima columna de la letra que se mostrara.

    for(i = colInicialD ; i != colFinalD ; i -= argSwipeDrawing -> direccion){//Recorre todas las columnas del dibujo.

        for(col = colInicialB ; col != colFinalB ; col -= argSwipeDrawing -> direccion){//Barre el display hacia un costado.
            usleep(velDispAnimation);//Indica a que velocidad se debe hacer el barrido.
            
            for(fil = 0 ; fil < 8 ; fil++){
                (*( argSwipeDrawing -> higerDispMenu ))[fil][col + argSwipeDrawing -> direccion] = (*( argSwipeDrawing -> higerDispMenu ))[fil][col];//Realiza el barrido.
            }
        }

        for(fil = 0 ; fil < 8 ; fil++){//Ingresa la columna del dibujo.
            (*( argSwipeDrawing -> higerDispMenu ))[fil][colFinalB + argSwipeDrawing -> direccion] = (*( argSwipeDrawing -> drawing ))[fil][i];
        }
        printHalfDisp(*( argSwipeDrawing -> higerDispMenu ), 'S');//Muestra el contenido en el display.
    }
    pthread_exit(0);
}
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/******************************************************************************************************************************************
 * 
                          _     _                _  _                    _   _                 ___                           
             ___   _ __  | |_  (_)  ___   _ _   | || |  __ _   _ _    __| | | |  ___   _ _    | _ \  __ _   _  _   ___  __ _ 
            / _ \ | '_ \ |  _| | | / _ \ | ' \  | __ | / _` | | ' \  / _` | | | / -_) | '_|   |  _/ / _` | | || | (_-< / _` |
            \___/ | .__/  \__| |_| \___/ |_||_| |_||_| \__,_| |_||_| \__,_| |_| \___| |_|     |_|   \__,_|  \_,_| /__/ \__,_|
                  |_|                                                                                                                                                             
 * 
 ******************************************************************************************************************************************/

void changeOption(void* argChangeOption){
//Esta funcion es la encargada de cambiar el texto que se muestra en pantalla en un menu.
    
    argChangeOption_t * data = (argChangeOption_t*)argChangeOption;

    static argTextAnimMenu_t argTextAnimMenu;

    velDispAnimation = 1;

    *(data -> animStatus) = 0;

    pthread_join(*(data -> threadMenu), NULL);//Termina el thread anterior aumentando la velocidad del barrido.

    *(data -> animStatus) = 1;

    argTextAnimMenu.msg = data -> nuevoTexto;
    argTextAnimMenu.lowerDispMenu = data -> lowerDispMenu;
    argTextAnimMenu.higherDispMenu = data -> higherDispMenu;
    argTextAnimMenu.drawing = data -> drawing;
    argTextAnimMenu.direccion = data -> direccion;
    argTextAnimMenu.changeAnimation = data -> animStatus;
    argTextAnimMenu.notSwipe = data -> notSwipe;
                                        //Inicia el nuevo thread que mostrara el nuevo texto.

    if(data->higherDispMenu == NULL){
        printf("Error in displayRaspi.c, higherDispMenu cannot be NULL in function changeOption\n");
    }
    if(data->lowerDispMenu == NULL){
        printf("Error in displayRaspi.c, lowerDispMenu cannot be NULL in function changeOption\n");
    }
    if(data->drawing == NULL){
        printf("Error in displayRaspi.c, drawing cannot be NULL in function changeOption\n");
    }
    if(data->nuevoTexto == NULL){
        printf("Error in displayRaspi.c, nuevoTexto cannot be NULL in function changeOption\n");
    }

    pthread_create(data -> threadMenu, NULL, textAnimMenu, &argTextAnimMenu);

}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/



/*******************************************************************************************************************************************
 * 
                                                     ___                        
                                                    / __|  __   ___   _ _   ___ 
                                                    \__ \ / _| / _ \ | '_| / -_)
                                                    |___/ \__| \___/ |_|   \___|
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/

halfDisp_t* getLeaderBoardName(halfDisp_t* nameDispMenu, int select){

    leaderboard_t leaderboard;
    parseScore(leaderboard);  //Se lee el leaderboard de score.txt

    char copiaLeaderboard [ROWSIZE];
    char name [5];
    int letra;
    int caracter = 1;
    
    strcpy(copiaLeaderboard, leaderboard[select]); //Accede al valor numerico de puntaje de cada posicion
    strtok(copiaLeaderboard," "); //Corta el texto del leaderboard hasta el espacio, se queda solo con el numero
    
    name [0] = '>'; //Agrega una flecha al principio del nombre
    name [4] = 0;  //Terminador
    for (letra=strlen(copiaLeaderboard)+1; leaderboard[select][letra]!=0; letra++){ //Copia el nombre del jugador en un string
        name[caracter]=leaderboard[select][letra];
        caracter++;
    }

    nameDispMenu = strToHalfDisplay(nameDispMenu, name); //Convierte el string a algo que se puede mostrar en el display.
    return nameDispMenu;
}


halfDisp_t* strToHalfDisplay(halfDisp_t * nombre, char* nombreStr){
//Esta funcion convierte un string de 4 letras en un halfDisplay_t.

    caracteres_t* caracter; //Variable que almacena el dibujo del caracter en pixeles.
    
    int offset; //Variable para desreferenciar en el arreglo de alfabeto.
    int i, j, k;
    int l = 0;

    for(i = 0; i < 4; i++){ //Recorre todas las letras del nombre
        offset = offsetAlfabeto(nombreStr[i]);
        caracter = alfabeto[offset];

        for(j = 0; j < 8; j++){//Recorre las filas del halfDisplay

            for(k = l; k < l+4; k++){//Recorre las columnas del halfDisplay
                (*nombre)[j][k] = (*caracter)[j][k-l]; //Copia el caracter en el halfDisplay
            }
        }
        l += 4;
    }

    return nombre;
}

void* letterFlashThread(void* data){
//Este thread se encarga de hacer titilar una letra en el display.

    letterFlash_t* letterFlash = (letterFlash_t*)data;

    caracteres_t* caracter; //Variable que almacena el dibujo del caracter en pixeles.
    
    int offset; //Variable para desreferenciar en el arreglo de alfabeto.
    int i, j;

    while(*(letterFlash->exitStatus)){
        offset = offsetAlfabeto(*(letterFlash->letra));
        caracter = alfabeto[offset];

        //Primero apaga todos los pixeles
        for(i = (letterFlash->pos)->y ; i < (letterFlash->pos)->y + 8 ; i++){//Recorre las filas

            for(j = (letterFlash->pos)->x ; j < (letterFlash->pos)->x + 4 ; j++){//Recorre las columnas
                (*(letterFlash->display))[i][j] = 0;
            }
        }
        printHalfDisp(*(letterFlash->display), 'S');//Muestra el contenido en el display.

        usleep(VEL_TITILEO * U_SEC2M_SEC);

        //Luego imprime la letra
        for(i = (letterFlash->pos)->y ; i < (letterFlash->pos)->y + 8 ; i++){//Recorre las filas

            for(j = (letterFlash->pos)->x ; j < (letterFlash->pos)->x + 4 ; j++){//Recorre las columnas
                (*(letterFlash->display))[i][j] = (*caracter)[i][j - (letterFlash->pos)->x];
            }
        }
        printHalfDisp(*(letterFlash->display), 'S');//Muestra el contenido en el display.

        usleep(2 * VEL_TITILEO * U_SEC2M_SEC);        
    }

    pthread_exit(0);
}


//BARRIDO LETRA: recibe la primer y segunda letra a barrer, el sentido del barrido y la coordenada a imprimir el barrido
//Esta funcion hace el barrido vertical de las letras en el display 
//Asumo el display ya correctamente incializado
void barridoLetra (char letraUno, char letraDos, int sentido, vector_t coordenada ){
    int i;
    uint8_t matriz [8][4];
    uint8_t matrizCopy [8][4];
    copyMatrixLetter(letraUno, matriz);
    copyMatrixLetter(letraDos, matrizCopy);
    dcoord_t coordenadaAImprimir = {coordenada.x,coordenada.y};
    for (i = 0; i<8 ; i++){
        sweepMatrix(matriz, sentido);
        addRow(matriz,matrizCopy[FIRSTROW(sentido)+i*sentido],sentido);
        printLetter(matriz, coordenadaAImprimir);
        usleep(BARRIDO_LETRA);
    }
    return;
}


//Esta funcion agrega la linea correspondiente en la matriz al hacer el barrido vertical de las letras
static void addRow(uint8_t matriz [8][4], uint8_t rowToAdd [4], int sentido){
    int i;
    for (i=0; i<4; i++){
        matriz[FIRSTROW(-sentido)][i]=rowToAdd[i];
    }
    return;
}

//Esta funcion barre una fila la matriz indicada en la direccion correspondiente. La ultima fila va a ser sobreescrita por addRow
static void sweepMatrix(uint8_t matriz [8][4], int sentido){
    int i,j;
    for (j=0; j<7; j++){
        for(i=0; i<4; i++){
            matriz[FIRSTROW(sentido)+j*sentido][i]= matriz[FIRSTROW(sentido)+sentido+j*sentido][i];
        }
    }
}


//Esta matriz copia la matriz de letra indicada en la matriz correspondiente
static void copyMatrixLetter(char letra, uint8_t matriz [8][4]){
    int i,j;
    for (j=0; j<8; j++){
        for(i=0; i<4; i++){
            matriz[j][i]= (*(alfabeto[offsetAlfabeto(letra)]))[j][i];
        }
    }

    return;
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

