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
    unsigned int balasActuales = counterBala(listaBalas);
    int balasDisponibles = levelSetting.maxEnemyBullets - balasActuales;
    object_t * nave = listaNaves;
    object_t * bala = listaBalas;
    srand(time(NULL));
    while(balasDisponibles > 0 && nave != NULL){
        if((rand()%100) >= levelSetting.shootProbability;){
            bala = addObj(listaBalas, nave.pos,);
        }
    }
}

unsigned int counterBala(object_t * listaBalas){  //Cuenta la cantidad de balas de una lista de balas
    unsigned int balasCant = 0;                 //Se inicializa la variable cantidad de balas
    while(listaBalas != NULL){                  //Si el nodo no esta vacio (no es el ultimo)
        balasCant++;                            //Se cuenta el nodo
        listaBalas = listaBalas -> next;     //Se apunta al siguiente nodo
    }
    return balasCant;                           //Se devuelve la cantidad de balas
}