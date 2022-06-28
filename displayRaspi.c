#include <stdint.h>
#include <unistd.h>
#include "utilidades.h"
#include <pthread.h>
#include "displayRaspi.h"
#include <stdio.h>

/*
 Se usaran los siguientes recursos de la libreria del display
    -dcoord_t, tipo de dato: struct con un campo x y un campo y
    -disp_init(void), inicializa el display
    -disp_clear(void), limpia el display
    -disp_write(dcoord_t,dlevel_t), escribe en el display
    -D_OFF,D_ON, ctes de prendido y apagado del display
    -disp_update(void), actualiza el display
*/
/*int main (void){
    dcoord_t p = {5,5};
    disp_init();
    disp_clear();
    drawEnemy(p,daniel1);
    disp_update()
    usleep(500*1000);
    cleanEnemy(p);
    drawEnemy(p,daniel2);
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

enemy_t daniel1 = {{1,0,1},{1,1,1}}; //2 sprites para cada tipo de enemigo
enemy_t daniel2 = {{1,1,1},{1,0,1}};

enemy_t pablo1 ={{1,1,0},{1,0,1}};
enemy_t pablo2 ={{0,1,1},{1,0,1}};

enemy_t nicolas1 ={{1,0,1},{0,1,0}};
enemy_t nicolas2 ={{0,1,0},{1,0,1}};

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

void drawEnemy(dcoord_t p, enemy_t alien){ //Esta funcion imprime en display un enemigo en un sprite dados en la posicion p
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

void cleanEnemy(dcoord_t p){ //Esta funcion borra en display un enemigo (tienen todos el mismo tamanyo) en la posicion p
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
    dcoord_t punto; //punto del display a escribir
    while(1){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % FRAMERATE) == 0 ){
            sem_wait(&semaforo);
            disp_clear(); //limpio el buffer            
            //Actualizo el buffer con la nueva posicion de los aliens
            printf("esta por entrar al while\n");
            object_t* aux = aliens;
            while (aliens!= NULL){ //mientras no se haya llegado al final de la lista
                printf("entro al while\n");
                printf("x: %d ; y: %d\n", aliens->pos.x, aliens->pos.y);
                punto.x=aliens->pos.x; //se definen posiciones en x y en y de los aliens, tomando como pivote la esquina superior izquierda
                punto.y=aliens->pos.y;
                printf("x: %d ; y: %d\n", aliens->pos.x, aliens->pos.y);
                if (punto.x>15||punto.y>15){
                    printf("Fuera de rango de impresion en la nave/n"); //chequeo de pixel a imprimir
                }
                
                switch(aliens->type){ //dependiendo del estado de animacion y el alien a imprimir, se imprime un sprite distinto
                    case DANIEL:
                        if (aliens->animationStatus%2){ //chequeo de estado de animacion, se imprime un sprite u otro dependiendo de cuantas veces se haya desplazado un alien
                            drawEnemy(punto,daniel1);
                        }
                        else{
                            drawEnemy(punto,daniel2);
                        }
                        break;
                    case NICOLAS:
                        if (aliens->animationStatus%2){
                            drawEnemy(punto,nicolas1);
                        }
                        else{
                            drawEnemy(punto,nicolas2);
                        }
                        break;
                    case PABLO:
                        if (aliens->animationStatus%2){
                            drawEnemy(punto,pablo1);
                        }
                        else{
                            drawEnemy(punto,pablo2);
                        }
                        break;
                    default: printf("Se esta queriendo imprimir como alien algo que no es un alien");break;
                }
                aliens=aux;
                printf("se termino de imprimir el alien\n");
                aliens=aliens->next; //se pasa al siguiente alien en la lista
            }
            printf("se imprimieron todos los aliens de la lista\n");
           /* while (balas!=NULL){ //mientras no se haya llegado al final de la lista
                punto.x=balas->pos.x; //se definen posiciones en x y en y de las balas
                punto.y=balas->pos.y;
                if (punto.x>15||punto.y>15)printf("Fuera de rango de impresion en la bala/n"); //chequeo de pixel a imprimir
                disp_write(punto,  D_ON); //como las balas son pixeles, se imprime el pixel donde se encuentra la bala
                balas=balas->next; //se pasa a la siguiente bala de la lista
            }*/
            disp_update(); //se transfiere del buffer al display de la RPI
            sem_post(&semaforo);
            printf("Se salio del mutex\n");
        }
    }
    pthread_exit(0);
}
