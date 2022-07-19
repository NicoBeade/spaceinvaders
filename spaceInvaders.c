/**********************************************************************************************************************************************************
 *
                          _____                                   _____                               _                     
                         / ____|                                 |_   _|                             | |                    
                        | (___    _ __     __ _    ___    ___      | |    _ __   __   __   __ _    __| |   ___   _ __   ___ 
                         \___ \  | '_ \   / _` |  / __|  / _ \     | |   | '_ \  \ \ / /  / _` |  / _` |  / _ \ | '__| / __|
                         ____) | | |_) | | (_| | | (__  |  __/    _| |_  | | | |  \ V /  | (_| | | (_| | |  __/ | |    \__ \
                        |_____/  | .__/   \__,_|  \___|  \___|   |_____| |_| |_|   \_/    \__,_|  \__,_|  \___| |_|    |___/
                                 | |                                                                                        
                                 |_|                                                
 * 
 ***********************************************************************************************************************************************************
*   
*
 **********************************************************************************************************************************************************/


#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include "utilidades.h"
#include "aliensYBalas.h"
#include <unistd.h>

#define RASPI

#ifdef RASPI
#include "inputRaspi.h"
#include "displayRaspi.h"
#endif

#ifdef ALLEGRO
#include "inputAllegro.h"
#include "displayAllegro.h"
#endif

/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/


/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                             __  __                              
                                            |  \/  |  __ _   __   _ _   ___   ___
                                            | |\/| | / _` | / _| | '_| / _ \ (_-<
                                            |_|  |_| \__,_| \__| |_|   \___/ /__/                                                    
 * 
 ******************************************************************************************************************************************/

#ifdef RASPI

#define INPUT_THREAD inputRPIThread
#define DISPLAY_THREAD_GAME displayRPIThread

#define SIGUIENTE ((menu->keys)->x == 1)    //Macros para indicar que representa un cambio de opcion en un menu.
#define ANTERIOR  ((menu->keys)->x == -1)

#endif

#ifdef ALLEGRO

#define INPUT_THREAD keyboardt
#define DISPLAY_THREAD_GAME displayt

#define SIGUIENTE ((menu->keys)->y == 1)    //Macros para indicar que representa un cambio de opcion en un menu.
#define ANTERIOR  ((menu->keys)->y == -1)
#endif


#define PRESS     ((menu->keys)->press == 1)    //Macro para detectar cuando se presiona para seleccionar una opcion en un menu.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                     ___                     _                   _              
                                    / __|  ___   _ _    ___ | |_   __ _   _ _   | |_   ___   ___
                                   | (__  / _ \ | ' \  (_-< |  _| / _` | | ' \  |  _| / -_) (_-<
                                    \___| \___/ |_||_| /__/  \__| \__,_| |_||_|  \__| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/


/*******************************************************************************************************************************************
*******************************************************************************************************************************************/
static void* menuHandlerThread(void * data);

/*******************************************************************************************************************************************
 * 
                __   __               _          _      _                 ___   _         _             _            
                \ \ / /  __ _   _ _  (_)  __ _  | |__  | |  ___   ___    / __| | |  ___  | |__   __ _  | |  ___   ___
                 \ V /  / _` | | '_| | | / _` | | '_ \ | | / -_) (_-<   | (_ | | | / _ \ | '_ \ / _` | | | / -_) (_-<
                  \_/   \__,_| |_|   |_| \__,_| |_.__/ |_| \___| /__/    \___| |_| \___/ |_.__/ \__,_| |_| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/

gameStatus_t GAME_STATUS = { .pantallaActual = MENU, .nivelActual = 0 , .menuActual = 0 , .exitStatus = 1};

keys_t KEYS = { .x =0, .y = 0, .press = 0 };//Almacena las teclas presionadas por el usuario.

sem_t SEM_GAME;//Semaforo que regula la ejecucion de los niveles.

menu_t* MENUES[10];//Arreglo que contiene punteros a todos los menues. No tiene por que estar definido aca, solo lo cree para hacer algo de codigo.
level_setting_t* LEVELS[10];//Arrego que contiene punteros a la config de todos los niveles.

extern unsigned int timerTick = 10000000;
extern unsigned int velInput = 1;
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


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
    while(GAME_STATUS.exitStatus){
        usleep(10 * U_SEC2M_SEC); //Sleep 10mS.
        timerTick -= 1;
    }
    pthread_exit(0);
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/



/*******************************************************************************************************************************************
 * 
                                                         __  __          _        
                                                        |  \/  |  __ _  (_)  _ _  
                                                        | |\/| | / _` | | | | ' \ 
                                                        |_|  |_| \__,_| |_| |_||_|
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/




int main(void){

    pthread_t timerT, inputT, menuHandlerT, levelHandlerT, moveAlienT, moveBalaT, displayT;

    pthread_create(&timerT, NULL, timer, NULL);

    pthread_create(&inputT, NULL, INPUT_THREAD, &KEYS);

    while(GAME_STATUS.exitStatus){//El juego se ejecuta hasta que se indique lo contrario en exitStatus.

        switch(GAME_STATUS.pantallaActual){//Esta seccion del codigo se encarga de inicializar los threads correctos dependiendo de la pantalla
                                           //actual y de la opcion seleccionada en algun menu.
            case MENU://Entra a este caso cuando el programa se encuentra en cualquier menu.
                sem_wait(&SEM_GAME);//Pausa la ejecucion del juego.
                pthread_create(&menuHandlerT, NULL, menuHandlerThread, MENUES[GAME_STATUS.menuActual]);//Se inicializa el thread de menu handler con el menu indicado.

                pthread_join(menuHandlerT, NULL);
                sem_post(&SEM_GAME);

                break;
            
            case START_LEVEL://Entra a este caso cuando se crea un nivel.
                printf("se inicio el nivel\n");
                GAME_STATUS.exitStatus = 0;
                /*
                object_t* alienList;//Se crea la lista de los aliens.
                alienList = initAliens(alienList, LEVELS[GAME_STATUS.nivelActual], "STRING QUE DICE LA CANTIDAD DE ALIENS POR FILAS", PABLO, SEXO);
                object_t* listBalasEnemigas;//Lista de las balas de los aliens.
                object_t* listBalasUsr;//Lista de las balas del usuario.

                object_t * naveUsuario = (object_t*) malloc(sizeof(object_t));//Crea la nave del usuario

                if(naveUsuario == NULL){\
                    printf("no se pudo crear al usuario por malloc \n");
                }

                naveUsuario -> pos.x = LEVELS[GAME_STATUS.nivelActual] -> disInicialUsrX;   //Inicializa la nave del usuario.
                naveUsuario -> pos.y = LEVELS[GAME_STATUS.nivelActual] -> disInicialUsrY;
                naveUsuario -> type = NAVE;
                naveUsuario -> lives = 1;
                naveUsuario -> next = NULL;

                #ifdef RASPI
                argDisplayRPI_t argDisplay = { .balasEnemigas = &listBalasEnemigas , .balasUsr = &listBalasUsr , .aliens = &alienList , .naveUser = & naveUsuario };
                #endif

                #ifdef ALLEGRO
                //aca hay que crear el argumento que recibe el thread del display de allegro.
                #endif

                pthread_create(&displayT, NULL, DISPLAY_THREAD_GAME, &argDisplay);//Inicializa el thead del display.

                pthread_create(&levelHandlerT, NULL, levelHandlerThread, MENUES[GAME_STATUS.menuActual]);//Se inicializa el thread de level handler con el nivel indicado.
                
                argMoveAlien_t argMoveAlien = { .levelSettings = LEVELS[GAME_STATUS.nivelActual] , .alienList = &alienList };//Inicializa el thread Move Alien.
                pthread_create(&moveAlienT, NULL, moveAlienThread, &argMoveAlien);

                argMoveBala_t argMoveBala = { .levelSettings = LEVELS[GAME_STATUS.nivelActual] , .balasEnemigas = &listBalasEnemigas, .balasUsr = &listBalasUsr };//Inicializa el thread Move Bala.
                pthread_create(&moveBalaT, NULL, moveBalaThread, &argMoveBala);

                pthread_join(levelHandlerT, NULL);//Espera hasta que se cree un menu.
                */
                break;

            case IN_GAME://Entra a este caso cuadno se reanuda un nivel.
                //pthread_create(&levelHandlerT, NULL, levelHandlerThread, MENUES[GAME_STATUS.menuActual]);//Se inicializa el thread de level handler con el nivel indicado.

                //pthread_join(levelHandlerT, NULL);//Espera hasta que se cree un menu.

                break;

            case DESTROY_LEVEL://Entra a este caso cuando se debe salir de un nivel, como cuando el usuario pierde o se desea volver al menu de inicio.
                //Aca hay que liberar del heap todas las listas y parar los threads que ejecutan el juego.

            default:
                break;
        }
    }

    pthread_join(inputT, NULL);
    pthread_join(timerT, NULL);

    return 0;
}
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
             __  __                                  _                        _     _  _                    _   _             
            |  \/  |  ___   _ _    _  _     _  _    | |     ___  __ __  ___  | |   | || |  __ _   _ _    __| | | |  ___   _ _ 
            | |\/| | / -_) | ' \  | || |   | || |   | |__  / -_) \ V / / -_) | |   | __ | / _` | | ' \  / _` | | | / -_) | '_|
            |_|  |_| \___| |_||_|  \_,_|    \_, |   |____| \___|  \_/  \___| |_|   |_||_| \__,_| |_||_| \__,_| |_| \___| |_|  
                                            |__/                                                                                                                                                                                                               
 * 
 ******************************************************************************************************************************************/

static void* menuHandlerThread(void * data){
/*Este thread es el encargado de manejar los menues.
*/
	menu_t * menu = (menu_t *) data;

    int select = 0;//Esta variable se utiliza para indicar la opcion seleccionada dentro del menu.

    while(menu -> exitStatus){
        usleep(10 * U_SEC2M_SEC);

        if (SIGUIENTE){//Si se presiona para ir a la siguiente opcion

            select += 1;
            if(select == (menu -> cantOpciones) - 1){//Si llegamos a la ultima opcion pasamos a la primera
                select = 0;
            }
            //(menu -> changeOption)(1);
            printf("Opcion: %d\n", select);
        }

        if (ANTERIOR){//Si se presiona para ir a la opcion anterior

            select -= 1;
            if(select < 0){//Si llegamos a la primer opcion pasamos a al ultima
                select = (menu -> cantOpciones) - 1;
            }
            //(menu -> changeOption)(-1);
            printf("Opcion: %d\n", select);
        }

        if (PRESS){//Si se selecciona la opcion
            menu -> exitStatus = (menu->selectOption[select])();//Se llama al callback que indica que accion realizar al presionar dicha opcion.
        }
    }
    
    pthread_exit(0);
}


/*
static void* levelHandlerThread(void * data){

	menu_t * menu = (menu_t *) data;


	if (DISPARAR){
		//Animacion
		disparar();
	}

	if (MOVER){
		//Animacion
		moverder();
	}

	if (PRESS){
		*opcionActual();
	}

}
*/
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/