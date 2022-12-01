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
#include <semaphore.h>


void* inputRPIThread(void* argInputRPI){

    jcoord_t coordJoy;
    jswitch_t switchJoy;

    while(GAME_STATUS.exitStatus){

        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velInput) == 0 ){//Determina cada cuanto se actualiza el input.
            sem_wait(&SEM_DRIVER);
            joy_update();
            coordJoy = joy_get_coord();//Obtiene las coordenadas del joystick.
            switchJoy =  joy_get_switch();
            sem_post(&SEM_DRIVER);

            //Si el joystick se mueve, indico ese valor en los campos de la variable keys.
            ((keys_t*)argInputRPI) -> x = (coordJoy.x <= JOY_ACTIVE_NEG) ? -1 : ( (coordJoy.x >= JOY_ACTIVE_POS) ? 1 : 0); 

            ((keys_t*)argInputRPI) -> y = (coordJoy.y <= JOY_ACTIVE_NEG_Y) ? -1 : ( (coordJoy.y >= JOY_ACTIVE_POS) ? 1 : 0);

            //Esta seccion detecta si se presiono el boton del joystick.
            ((keys_t*)argInputRPI) -> press = (switchJoy == J_PRESS) ? 1 : 0;
            
        }
    }
    pthread_exit(0);
}
