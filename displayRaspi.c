
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


void* displayRPI (void* argDisplayRPI){
    //object_t* balas = ((argDisplayRPI_t*)argDisplayRPI)->balas; //Puntero a la lista de balas
    object_t* aliens = ((argDisplayRPI_t*)argDisplayRPI)->aliens; //Puntero a la lista de aliens
    object_t* naveUser = ((argDisplayRPI_t*)argDisplayRPI)->naveUser; //Puntero a la nave del usuario
    dcoord_t punto; //punto del display a escribir
    while(1){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % FRAMERATE) == 0 ){
            sem_wait(&semaforo);
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

            disp_update(); //se transfiere del buffer al display de la RPI
            sem_post(&semaforo);

        }
    }
    pthread_exit(0);
}
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/******************************************************************************************************************************************
 * 
         _____   _                            _     ___    _                 ___                               ___   ___   ___ 
        |_   _| | |_    _ _   ___   __ _   __| |   |   \  (_)  ___  _ __    | _ \  __ _   _  _   ___  __ _    | _ \ | _ \ |_ _|
          | |   | ' \  | '_| / -_) / _` | / _` |   | |) | | | (_-< | '_ \   |  _/ / _` | | || | (_-< / _` |   |   / |  _/  | | 
          |_|   |_||_| |_|   \___| \__,_| \__,_|   |___/  |_| /__/ | .__/   |_|   \__,_|  \_,_| /__/ \__,_|   |_|_\ |_|   |___|
                                                                   |_|                    
 * 
 ******************************************************************************************************************************************/

void* dispMenu(void* punteroPausa){

    punteroMenu_t menuDisplay = {0,0,0};

    while(1){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % FRAMERATE) == 0 ){

            if( ((punteroMenu_t*)punteroPausa) -> x != 0 ){//Si se movio el joistick
                menuDisplay.x += ((punteroMenu_t*)punteroPausa) -> x;
                if(menuDisplay.x < 0){//estos if estan para que se pueda "dar la vuelta" en las opciones.
                    menuDisplay.x = cantOpciones - 1;//Si nos pasamos por la izquierda vamos a la ultima opcion.
                }
                else if(menuDisplay.x > (((punteroMenu_t*)punteroPausa) -> cantOpciones) - 1){
                    menuDisplay.x = 0;//Si nos pasamos por la derecha vamos a la primera opcion.
                }

                (((punteroMenu_t*)punteroPausa) -> changeOption)(pausaDisplay.x);//Llama a la funcion que se encarga de mostrar en pantalla la opcion indicada
            }

            menuDisplay.press = ((punteroMenu_t*)punteroPausa) -> press;
            if(menuDisplay.press != 0){
                (((punteroMenu_t*)punteroPausa) -> selectOption)(pausaDisplay.x);//Llama a la funcion que se encarga de procesar que hacer cuadno se selecciona una opcion.
            }
        }
    }
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/