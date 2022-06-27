#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "inputRaspi.h"
#include <unistd.h>
#include "utilidades.h"

int timerTick = 1000000;

int main(void){

    object_t * naveUsuario = (object_t*) malloc(sizeof(object_t));//Crea la nave del usuario

    naveUsuario -> pos.x = 0;
    naveUsuario -> pos.y = 1;

    argUpdateInputGame_t updateInput = { naveUsuario, 15, 3, 1 };

    pthread_t TupdateInputGame;

    pthread_create(&TupdateInputGame, NULL, updateInputGame, &updateInput);

    while(1){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.

        printf("x: %d ; y: %d", naveUsuario -> pos.x, naveUsuario -> pos.y);
    }

    pthread_join(TupdateInputGame, NULL);

    return 0;
}


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