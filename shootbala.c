#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>
#include "aliensYBalas.h"
#include "utilidades.h"
#include "stdlib.h"
#include "time.h"
int shootBalaUsr(nave_t * naveUsr){
    //Se cuenta la cantidad de balas del usuari
    nave_t * usuario = naveUsr;
    unsigned int balasActuales = counterBala(naveUsr -> listaBalasUsr);
    if(balasActuales < naveUsr.maxBullets){
        naveUsr -> listaBalasUsr = addObj(naveUsr -> listaBalasUsr, naveUsr -> pos, BALA_USUARIO, 1);
        return 0;
    }
    else{
        return -1;
    }
}

object_t * shootBalaEnemy(object_t * listaNaves, object_t * listaBalas, level_setting_t * levelSetting,){
    unsigned int balasActuales = countList(listaBalas);
    int balasDisponibles = levelSetting -> maxEnemyBullets - balasActuales;
    object_t * nave = listaNaves;
    object_t * bala = listaBalas;
    srand(time(NULL));
    int probabilidad;
    while(balasDisponibles > 0 && nave != NULL){
        switch(nave -> type){
            case DANIEL:
                probabilidad = levelSetting->shootProbDani;
            case PABLO:
                probabilidad = levelSetting->shootProbPablo;
            case NICOLAS:
                probabilidad = levelSetting->shootProbNico;
        }
        if((rand()%100) >= probabilidad){
            bala = addObj(listaBalas, nave.pos,);
        }
    }
}

unsigned int countList(object_t * lista){  //Cuenta la cantidad de nodos de una lista de obj
    unsigned int nodosCant = 0;                 //Se inicializa la variable cantidad de nodos
    while(lista != NULL){                  //Si el nodo no esta vacio (no es el ultimo)
        nodosCant++;                            //Se cuenta el nodo
        lista = lista -> next;     //Se apunta al siguiente nodo
    }
    return nodosCant;                           //Se devuelve la cantidad de nodos
}

vector_t getObjCenter(object_t * nodo, level_setting_t * levelSetting){
    if(nodo != NULL){
        
    }
}