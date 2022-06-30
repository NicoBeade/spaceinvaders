#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>
#include "aliensYBalas.h"
#include "utilidades.h"
#include "stdlib.h"
/*int shootBalaUsr(nave_t * naveUsr){
    //Se cuenta la cantidad de balas del usuari
    nave_t * usuario = naveUsr;
    unsigned int balasActuales = counterBala(naveUsr -> listaBalasUsr);
    if(balasActuales < naveUsr -> maxBullets){
        naveUsr -> listaBalasUsr = addObj(naveUsr -> listaBalasUsr, naveUsr -> pos, BALA_USUARIO, 1);
        return 0;
    }
    else{
        return -1;
    }
}*/

object_t * shootBala(object_t * listaNaves, object_t * listaBalas, level_setting_t * levelSetting){
    int balasActuales = countList(listaBalas);
    if(nave -> type == NAVE){
        int balasDisponibles = levelSetting -> maxUsrBullets - balasActuales;
    }
    else{
        int balasDisponibles = levelSetting -> maxEnemyBullets - balasActuales;
    }
    object_t * nave = listaNaves;
    object_t * bala = listaBalas;
    int probabilidad;
    int tipoBala;
    while(balasDisponibles > 0 && nave != NULL){
        switch(nave -> type){
            case DANIEL:
                probabilidad = levelSetting->shootProbDani;
                tipoBala = BALA_DANIEL;
                break;
            case PABLO:
                probabilidad = levelSetting->shootProbPablo;
                tipoBala = BALA_PABLO;
                break;
            case NICOLAS:
                probabilidad = levelSetting->shootProbNico;
                tipoBala = BALA_NICOLAS;
                break;
            case NAVE:
                probablidad = 100;
                tipoBala = BALA_USUARIO;
                break;
            default:
                return NULL;
        }
        if((rand()%100) < probabilidad){
            vector_t posicionBala;
            if(tipoBala == BALA_USUARIO){
                posicionBala.x = nave->pos.x - (levelSetting->anchoUsr)/2;
                posicionBala.y = nave->pos.y - (levelSetting->altoUsr)/2;
            }
            else{
                posicionBala.x = nave->pos.x + (levelSetting->anchoNave)/2;
                posicionBala.y = nave->pos.y + (levelSetting->altoNave)/2;
            }
            bala = addObj(bala, nave->pos + posicionBala,tipoNave);
            balasDisponibles--;
        }
        nave = nave -> next;
    }
    return bala;
}

unsigned int countList(object_t * lista){  //Cuenta la cantidad de nodos de una lista de obj
    unsigned int nodosCant = 0;                 //Se inicializa la variable cantidad de nodos
    while(lista != NULL){                  //Si el nodo no esta vacio (no es el ultimo)
        nodosCant++;                            //Se cuenta el nodo
        lista = lista -> next;     //Se apunta al siguiente nodo
    }
    return nodosCant;                           //Se devuelve la cantidad de nodos
}

//typedef struct OBJECT{//Cada alien, barrera, bala es un struct de este tipo y se los organizara en listas en funcion de cual de estos es
//    vector_t pos;//Posicion en x e y
//    types_t type;//Tipo de objeto y categoria dentro del tipo
 //   int lives;//Cantidad de vidas del objeto, cada objeto podria tener distinta cantidad de vidas
 // int ancho
 // int alto
//    char animationStatus;//Estado de la animacion de cada nave, puede ser distinto para cada nave
 //   struct OBJECT * next;//Puntero al siguiente objeto de la lista.
//}object_t;


object_t initBarreras(int cantBarreras)
