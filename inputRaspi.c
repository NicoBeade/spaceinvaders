/**********************************************************************************************************************************************************
 * 
                             _                           _     _____                          _            
                            (_)                         | |   |  __ \                        (_)           
                             _   _ __    _ __    _   _  | |_  | |__) |   __ _   ___   _ __    _        ___ 
                            | | | '_ \  | '_ \  | | | | | __| |  _  /   / _` | / __| | '_ \  | |      / __|
                            | | | | | | | |_) | | |_| | | |_  | | \ \  | (_| | \__ \ | |_) | | |  _  | (__ 
                            |_| |_| |_| | .__/   \__,_|  \__| |_|  \_\  \__,_| |___/ | .__/  |_| (_)  \___|
                                        | |                                          | |                   
                                        |_|                                          |_|
 * 
 ***********************************************************************************************************************************************************
*   Este archivo contiene las funciones y threads encargadas de todas las acciones relacionadas a recibir input del usuario cuando el juego se ejecuta en
*   la raspberry.
 **********************************************************************************************************************************************************/

#include <stdio.h>
#include "joydrv.h"
#include "utilidades.h"
#include "inputRaspi.h"
#include <pthread.h>
#include <unistd.h>

 void* updateInputGame(void* argUpdateInputGame){

    int velUsuario = 50;/*Determina que tan rapido podra mover la nave del usuario. La conversion es: si velUsuario = 1, entonces la nave se podra mover 
        cada 10mS. Para ejecutar que la nave se pueda mover cada 1s, velUsuario debe valer 100. Por defecto se mueve cada medio segundo.*/
    jcoord_t coordJoy;
    while(1){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velInput) == 0 ){

            coordJoy = joy_get_coord();
            printf("Joystick coord: %d", coordJoy.x);

            if( ((coordJoy.x <= JOY_ACTIVE_NEG) || (coordJoy.x >= JOY_ACTIVE_POS)) && (timerTick % velUsuario == 0) ){//If para limitar la velocidad de la nave.
                
                if( !((  ( coordJoy.x <= JOY_ACTIVE_NEG ) && ( NAVE_USUARIO_X == 0 )  ) || (  ( coordJoy.x >= JOY_ACTIVE_POS ) && ( NAVE_USUARIO_X == X_MAX - TAM_X_NAVE)  )) ){
                    //Chequea que no se vaya fuera del limite.

                    if(coordJoy.x < 0){//Desplaza para la izquierda
                        NAVE_USUARIO_X -= DESPLAZAMIENTO;
                    }
                    else if(coordJoy.x > 0){//Desplaza para la derecha
                        NAVE_USUARIO_X += DESPLAZAMIENTO;
                    }
                }
            }
        }
    }
 }