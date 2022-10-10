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
*   Funciona de la siguiente manera: tiene acceso a un struct global (keys) que almacena hacia donde se movio el joyustick. Este thread se encarga de 
*   modificar el contenido de dicho struct para que el resto del programa pueda saber hacia donde se movio el joystick.
*
 **********************************************************************************************************************************************************/

#include <stdio.h>
#include "drivers/joydrv.h"
#include "inputRaspi.h"
#include <pthread.h>
#include <unistd.h>


void* inputRPIThread(void* argInputRPI){

    jcoord_t coordJoy;
    jswitch_t switchJoy;

    while(GAME_STATUS.exitStatus){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velInput) == 0 ){//Determina cada cuanto se actualiza el input.

            joy_update();
            coordJoy = joy_get_coord();//Obtiene las coordenadas del joystick.
            switchJoy =  joy_get_switch();

            //Si el joystick se mueve, indico ese valor en los campos de la variable keys.
            ((keys_t*)argInputRPI) -> x = (coordJoy.x <= JOY_ACTIVE_NEG) ? -1 : ( (coordJoy.x >= JOY_ACTIVE_POS) ? 1 : 0); 

            ((keys_t*)argInputRPI) -> y = (coordJoy.y <= JOY_ACTIVE_NEG) ? -1 : ( (coordJoy.y >= JOY_ACTIVE_POS) ? 1 : 0);

            //Esta seccion detecta si se presiono el boton del joystick.
            ((keys_t*)argInputRPI) -> press = (switchJoy == J_PRESS) ? 1 : 0;
            
        }
    }
    pthread_exit(0);
}















/*
 void* updateInputGameThread(void* argUpdateInputGame){

    level_setting_t * infoNivel = ((argUpdateInputGame_t*)argUpdateInputGame) -> levelSettings;

    int velUsuario = 5;/*Determina que tan rapido podra mover la nave del usuario. La conversion es: si velUsuario = 1, entonces la nave se podra mover 
        cada 10mS. Para ejecutar que la nave se pueda mover cada 1s, velUsuario debe valer 100. Por defecto se mueve cada medio segundo.*/
    /*jcoord_t coordJoy;
    pthread_t TdisplayPausa, TinputPausa;
    punteroMenu_t punteroPausa = {0,0,0};
    while(1){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velInput) == 0 ){

            joy_update();
            coordJoy = joy_get_coord();

            if( ((coordJoy.x <= JOY_ACTIVE_NEG) || (coordJoy.x >= JOY_ACTIVE_POS)) && (timerTick % velUsuario == 0) ){//If para limitar la velocidad de la nave.

                if(coordJoy.x < 0){//Desplaza para la izquierda
                    MOOVE_NAVE_USUARIO(NAVE_USUARIO, -DESPLAZAMIENTO_USR_L(infoNivel), X_MAX_L(infoNivel), ANCHO_USR_L(infoNivel));
                }
                else if(coordJoy.x > 0){//Desplaza para la derecha
                    MOOVE_NAVE_USUARIO(NAVE_USUARIO, -DESPLAZAMIENTO_USR_L(infoNivel), X_MAX_L(infoNivel), ANCHO_USR_L(infoNivel));
                }
            }

            if(joy_get_switch() == J_PRESS){

                sem_wait(&semaforo);
                pthread_create(&TdisplayPausa, NULL, THREAD_DISPLAY_PAUSA, &punteroPausa);//Inicializa el thread que gestiona el display durante la pausa.
                pthread_create(&TinputPausa, NULL, inputMenuThread, &punteroPausa);//Inicializa el thread que gestiona el input durante la pausa.

                pthread_join(TdisplayPausa, NULL);
                punteroPausa.x = -2;
                pthread_join(TinputPausa, NULL);

                sem_post(&semaforo);
            }
        }
    }
}


void* inputMenuThread(void* punteroPausa){

    jcoord_t coordJoy;

    while(((punteroMenu_t*)punteroPausa) -> x != -2){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velInput) == 0 ){

            joy_update();
            coordJoy = joy_get_coord();

            if(coordJoy.x >= JOY_ACTIVE_NEG && coordJoy.x <= JOY_ACTIVE_POS){//Si el joystick esta en el centro en X
                ((punteroMenu_t*)punteroPausa) -> x = 0;
            }
            else if(coordJoy.x <= JOY_ACTIVE_NEG){//Si el joistick esta para la izquierda en X
                ((punteroMenu_t*)punteroPausa) -> x = -1;
            }
            else if(coordJoy.x >= JOY_ACTIVE_POS){//Si el joistick esta para la derecha en X
                ((punteroMenu_t*)punteroPausa) -> x = 1;
            }

            if(coordJoy.y >= JOY_ACTIVE_NEG && coordJoy.y <= JOY_ACTIVE_POS){//Si el joystick esta en el centro en Y
                ((punteroMenu_t*)punteroPausa) -> y = 0;
            }
            else if(coordJoy.y <= JOY_ACTIVE_NEG){//Si el joistick esta para la izquierda en Y
                ((punteroMenu_t*)punteroPausa) -> y = -1;
            }
            else if(coordJoy.y >= JOY_ACTIVE_POS){//Si el joistick esta para la derecha en Y
                ((punteroMenu_t*)punteroPausa) -> y = 1;
            }

            ((punteroMenu_t*)punteroPausa) -> press = (joy_get_coord == J_NOPRESS) ? 0 : 1;
        }
    }
}
*/