#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>
#include "aliensYBalas.h"
#include "utilidades.h"

int shootBalaUsr(nave_t * naveUsr){
    //Se cuenta la cantidad de balas del usuari
    object_t * listaBalasAux = naveUsr -> listaBalasUsr;
    nave_t * usuario = naveUsr;
    int balasActuales = 0;
    while(listaBalasAux != NULL){
        balasActuales++;
        listaBalasAux = listaBalasAux -> next;
    }
    if(balasActuales < maxBullets){
        naveUsr -> listaBalasUsr = addObj(naveUsr -> listaBalasUsr, naveUsr -> pos, BALA_USUARIO, 1);
        return 0;
    }
    else{
        return -1;
    }
}

object_t * shootBalaEnemy(object_t listaNaves, object_t listaBalas){

}

unsigned int counterBala(){
    while(listaBalasAux != NULL){
        balasActuales++;
        listaBalasAux = listaBalasAux -> next;
    }
}