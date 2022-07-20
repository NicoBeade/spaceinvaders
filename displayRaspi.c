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
*   Este archivo contiene las funciones y threads encargadas de todas las acciones relacionadas al manejo del display RPI
*   Contiene los siguientes threads:
*       -displayRPI: se encarga de realizar la actualizacion del display durante la ejecucion del juego. 
*            Para ello utiliza las siguientes funciones:
*               -drawSprite
*               -cleanSprite
*               -clearBuffer
*
*                                        Se usaran los siguientes recursos de la libreria del display
*                                             -dcoord_t, tipo de dato: struct con un campo x y un campo y
*                                             -disp_init(void), inicializa el display
*                                             -disp_clear(void), limpia el display
*                                             -disp_write(dcoord_t,dlevel_t), escribe en el display
*                                             -D_OFF,D_ON, ctes de prendido y apagado del display
*                                             -disp_update(void), actualiza el display
*
*
 **********************************************************************************************************************************************************/


#include <stdint.h>
#include <unistd.h>
#include "utilidades.h"
#include <pthread.h>
#include "displayRaspi.h"
#include <stdio.h>
#include "sprites.h"

/*int main (void){
    dcoord_t p = {5,5};
    disp_init();
    disp_clear();
    drawSprite(p,daniel1);
    disp_update()
    usleep(500*1000);
    cleanEnemy(p);
    drawSprite(p,daniel2);
    disp_update();
    return 0;
}
*/ //Ejemplo de main 

/*******************************************************************************************************************************************
 * 
                                     ___                     _                   _              
                                    / __|  ___   _ _    ___ | |_   __ _   _ _   | |_   ___   ___
                                   | (__  / _ \ | ' \  (_-< |  _| / _` | | ' \  |  _| / -_) (_-<
                                    \___| \___/ |_||_| /__/  \__| \__,_| |_||_|  \__| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/
#define FRAMERATE 4 //tasa de refresco del display

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
//*****************THREAD DISPLAY DURANTE MENUES
static int offsetAlfabeto(char caracter); //Se utiliza para obtener el offset necesario para acceder al string "alfabeto".
static void swipeCharacter(halfDisp_t* lowerDispMenu, caracteres_t caracter, int direccion); //Agrega un caracter completo al buffer.
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

void drawSprite(dcoord_t p, sprite_t alien){ //Esta funcion imprime en display un enemigo en un sprite dados en la posicion p
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<=2 ; i++){
        for(j=0 ; j<=1 ; j++){
            if (alien [j][i] == 1){
                pAux.x = p.x+i;
                pAux.y = p.y+j;
                if(pAux.x>15||pAux.y>15)printf("Fuera de ranfo de impresion en algun pixel de la nave");
                disp_write(pAux,D_ON); //Actualiza el buffer          
            }
        }
    }
}

void cleanSprite(dcoord_t p){ //Esta funcion borra en display un enemigo (tienen todos el mismo tamanyo) en la posicion p
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<=2 ; i++){
        for(j=0 ; j<=1 ; j++){
            pAux.x = p.x+i; 
            pAux.y = p.y+j;
            if(pAux.x>15||pAux.y>15){
            printf("Fuera de ranfo de impresion en algun pixel de la nave");
            }
            disp_write(pAux ,D_OFF); //Actualiza el buffer
        }
    }
}

void clearBuffer(void){ //Esta funcion imprime en display un enemigo en un sprite dados en la posicion p
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<16 ; i++){
        for(j=0 ; j<16 ; j++){
            pAux.x = i;
            pAux.y = j;
            disp_write(pAux, D_OFF); //Actualiza el buffer          
        }
    }
}

void printLetter(caracteres_t letter){ //imprime una letra barriendo los 32 pixeles de una matriz de 8X4
    int i,j;
    dcoord_t punto;
    for (i=0; i<8; i++){
        for (j=0; j<4; j++){
            punto.x=j;
            punto.y=i;
            if (letter [i][j]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    disp_update();
}

void printFullDisp(fullDisp_t displaySprite){ //imprime toda la pantalla barriendo los 256 pixeles de una matriz de 16x16
    int i,j;
    dcoord_t punto;
    for (i=0; i<16; i++){
        for (j=0; j<16; j++){
            punto.x=j;
            punto.y=i;
            if (displaySprite[i][j]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    disp_update();
}

void printHalfDisp(halfDisp_t halfDispSprite, char mitad){ //imprime la mitad de la pantalla barriendo los 128 pixeles de una matriz
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
    for (i=0; i<8; i++){
        for (j=0; j<16; j++){
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
    disp_update();
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

/*
void* displayRPIThread (void* argDisplayRPI){
    //object_t* balas = ((argDisplayRPI_t*)argDisplayRPI)->balas; //Puntero a la lista de balas
    object_t* aliens = ((argDisplayRPI_t*)argDisplayRPI)->aliens; //Puntero a la lista de aliens
    object_t* naveUser = ((argDisplayRPI_t*)argDisplayRPI)->naveUser; //Puntero a la nave del usuario
    dcoord_t punto; //punto del display a escribir
    while(1){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % FRAMERATE) == 0 ){
            sem_wait(&SEM_GAME);
            clearBuffer(); //limpio el buffer            
            //Actualizo el buffer con la nueva posicion de los aliens
            printf("esta por entrar al while\n");
            object_t* aux = aliens;
            while (aliens!= NULL){ //mientras no se haya llegado al final de la lista

                punto.x=aliens->pos.x; //se definen posiciones en x y en y de los aliens, tomando como pivote la esquina superior izquierda
                punto.y=aliens->pos.y;

                if (punto.x>15||punto.y>15||punto.x<0||punto.y<0){
                    printf("Fuera de rango de impresion en la nave/n"); //chequeo de pixel a imprimir
                }
                
                switch(aliens->type){ //dependiendo del estado de animacion y el alien a imprimir, se imprime un sprite distinto
                    case DANIEL:
                        if (aliens->animationStatus%2){ //chequeo de estado de animacion, se imprime un sprite u otro dependiendo de cuantas veces se haya desplazado un alien
                            drawSprite(punto,daniel1);
                        }
                        else{
                            drawSprite(punto,daniel2);
                        }
                        break;
                    case NICOLAS:
                        if (aliens->animationStatus%2){
                            drawSprite(punto,nicolas1);
                        }
                        else{
                            drawSprite(punto,nicolas2);
                        }
                        break;
                    case PABLO:
                        if (aliens->animationStatus%2){
                            drawSprite(punto,pablo1);
                        }
                        else{
                            drawSprite(punto,pablo2);
                        }
                        break;
                    default: printf("Se esta queriendo imprimir como alien algo que no es un alien");break;
                    
                }   
                aliens=aliens->next; //se pasa al siguiente alien en la lista
            }
            aliens=aux; //devuelve aliens al principio de la lista
            
            punto.x=naveUser->pos.x; //posicion en x y en y de la nave
            punto.y=naveUser->pos.y;
            drawSprite(punto,nave); //copia la nave en el buffer
            
            //aux = balas;//Guarda el puntero al primer elemento de la lista de las balas
           /* while (balas!=NULL){ //mientras no se haya llegado al final de la lista
                punto.x=balas->pos.x; //se definen posiciones en x y en y de las balas
                punto.y=balas->pos.y;
                if (punto.x>15||punto.y>15)printf("Fuera de rango de impresion en la bala/n"); //chequeo de pixel a imprimir
                disp_write(punto,  D_ON); //como las balas son pixeles, se imprime el pixel donde se encuentra la bala
                balas=balas->next; //se pasa a la siguiente bala de la lista
            }*/
            //balas = aux;
/*
            disp_update(); //se transfiere del buffer al display de la RPI
            sem_post(&SEM_GAME);

        }
    }
    pthread_exit(0);
}
*/
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
    
    int i, j, offset;
    int firstBarr = 4;

    int firstLetter = ( (((argTextAnimMenu_t*)argTextAnimMenu) -> direccion) == DERECHA ) ? 3 : 0;
    int lastLetter = ( (((argTextAnimMenu_t*)argTextAnimMenu) -> direccion) == DERECHA ) ? -1 : 4;
    
    //Primero imprimimos las primeras 4 letras.
    for(i = firstLetter ; i != lastLetter ; i -= (((argTextAnimMenu_t*)argTextAnimMenu) -> direccion)){

        offset = offsetAlfabeto((((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[i]);
        swipeCharacter(((argTextAnimMenu_t*)argTextAnimMenu) -> lowerDispMenu, *(alfabeto[offset]), ((argTextAnimMenu_t*)argTextAnimMenu) -> direccion);
    }   
    usleep(500 * U_SEC2M_SEC);//Espera medio segundo.

    velDispAnimation = VEL_DISP_ANIMATION;

    do{//Barre el texto hasta que se le indique lo contrario.
        for(j = firstBarr ; (((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[j] != '\0' ; j++){//Barre todas las letras del texto.

            offset = offsetAlfabeto((((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[j]);
            swipeCharacter(((argTextAnimMenu_t*)argTextAnimMenu) -> lowerDispMenu, *(alfabeto[offset]), IZQUIERDA);
        }
        firstBarr = 0;//Reinicia el proceso.
    }
    while(*(((argTextAnimMenu_t*)argTextAnimMenu) -> changeAnimation));

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
        printHalfDisp(*lowerDispMenu, 'I');//Muetra el contenido en el display.
    }
    
}

/*
void* dispMenu(void* punteroPausa){

    punteroMenu_t menuDisplay = {0,0,0};

    int exitThread = 1;//Se utiliza para saber si hay que salir del thread.

    while(exitThread){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % FRAMERATE) == 0 ){

            if( ((punteroMenu_t*)punteroPausa) -> x != 0 ){//Si se movio el joistick
                menuDisplay.x += ((punteroMenu_t*)punteroPausa) -> x;
                if(menuDisplay.x < 0){//estos if estan para que se pueda "dar la vuelta" en las opciones.
                    menuDisplay.x = (((punteroMenu_t*)punteroPausa) -> cantOpciones) - 1;//Si nos pasamos por la izquierda vamos a la ultima opcion.
                }
                else if(menuDisplay.x > (((punteroMenu_t*)punteroPausa) -> cantOpciones) - 1){
                    menuDisplay.x = 0;//Si nos pasamos por la derecha vamos a la primera opcion.
                }
                (((punteroMenu_t*)punteroPausa) -> changeOption)(menuDisplay.x);//Llama a la funcion que se encarga de mostrar en pantalla la opcion indicada
            }

            menuDisplay.press = ((punteroMenu_t*)punteroPausa) -> press;
            if(menuDisplay.press != 0){
                exitThread = (((punteroMenu_t*)punteroPausa) -> selectOption)(menuDisplay.x);//Llama a la funcion que se encarga de procesar que hacer cuadno se selecciona una opcion.
            }
        }
    }
}
*/
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
    
    velDispAnimation = 1;

    *(((argChangeOption_t*)argChangeOption) -> animStatus) = 0;

    pthread_join(((argChangeOption_t*)argChangeOption) -> threadMenu, NULL);//Termina el thread anterior aumentando la velocidad del barrido.

    *(((argChangeOption_t*)argChangeOption) -> animStatus) = 1;

    argTextAnimMenu_t argTextAnimMenu = { ((argChangeOption_t*)argChangeOption) -> nuevoTexto,  ((argChangeOption_t*)argChangeOption) -> lowerDispMenu,
                                         ((argChangeOption_t*)argChangeOption) -> direccion, ((argChangeOption_t*)argChangeOption) -> animStatus};
                                        //Inicia el nuevo thread que mostrara el nuevo texto.

    pthread_create(&(((argChangeOption_t*)argChangeOption) -> threadMenu), NULL, textAnimMenu, &argTextAnimMenu);
}

/*
void changeOption(int actualOption){
    switch(actualOption){
        case RESUME://Volver al juego
            printHalfDisp(halfDispResume);
            swipeTextDisp("RESUME");
        case VOLUMEN://Modificar el volumen
            printHalfDisp(halfDispVolume);
            swipeTextDisp("VOLUME");
        case HOME://Volver al menu de inicio
            printHalfDisp(halfDispAlienSpaceInvaders);
            swipeTextDisp("HOME");
        case RESTART://Reiniciar el nivel
            printHalfDisp(halfDispRestart);
            swipeTextDisp("RESTART");
        case SCORE://Ver el puntaje actual
            printHalfDisp(halfDispTrophy);
            swipeTextDisp("SCORE");
        default:
    }
}
*/
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
             ___                      _                                _           __  __                                
            | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___    |  \/  |  ___   _ _    _  _   ___   ___
            | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_)   | |\/| | / -_) | ' \  | || | / -_) (_-<
            |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___|   |_|  |_| \___| |_||_|  \_,_| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/

//********************************************              MENU DE INICIO          ********************************************************

int selectPlayInicio(void){
    printf("Select Play Inicio\n");
    return 1;
}

int selectLevelsInicio(void){
    return 1;
}

int selectVolumeInicio(void){
    return 1;
}

int selectQuitGameInicio(void){
    GAME_STATUS.pantallaActual = START_LEVEL;
    return 0;
}

//******************************************************************************************************************************************

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/