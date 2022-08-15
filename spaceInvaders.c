/***********************************************************************************************************************************************************
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
#include "spaceLib/spaceLib.h"
#include <unistd.h>

#define RASPI 

#ifdef RASPI
#include "inputRaspi.h"
#include "displayRaspi.h"
#include "disdrv.h"
#include "joydrv.h"
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
typedef struct{
	level_setting_t * levelSettings;
	object_t ** alienList;
}argMoveAlien_t;

typedef struct{
	level_setting_t * levelSettings;
	object_t ** balasEnemigas;
	object_t ** balasUsr;
}argMoveBala_t;

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

#define INPUT_THREAD inputRPIThread             //Thread encargado de leer el input en la RPI
#define DISPLAY_THREAD_GAME displayRPIThread    //Thread encargado de actualizar el display durante la ejecucion del juego en la RPI
#define DISP_ANIM_MENU  textAnimMenu            //Thread encargado de actualizar el display durante un menu en la RPI.

#define DERECHA_INPUT ((menu->keys)->x == 1)    //Macros para detectar como se movio el joystick.
#define IZQUIERDA_INPUT  ((menu->keys)->x == -1)
#define ARRIBA_INPUT ((menu->keys)->y == 1)
#define ABAJO_INPUT  ((menu->keys)->y == -1)

#endif

#ifdef ALLEGRO

#define INPUT_THREAD keyboardt
#define DISPLAY_THREAD_GAME displayt

#define SIGUIENTE_INPUT ((menu->keys)->y == 1)    //Macros para indicar que representa un cambio de opcion en un menu.
#define ANTERIOR_INPUT  ((menu->keys)->y == -1)
#endif


#define PRESS_INPUT     ((menu->keys)->press == 1)    //Macro para detectar cuando se presiona para seleccionar una opcion en un menu.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

void * moveAlienThread(void* argMoveAlien);

void * moveBalaThread(void * argMoveBala);

static void* menuHandlerThread(void * data);

/*******************************************************************************************************************************************
 * 
                                     ___                     _                   _              
                                    / __|  ___   _ _    ___ | |_   __ _   _ _   | |_   ___   ___
                                   | (__  / _ \ | ' \  (_-< |  _| / _` | | ' \  |  _| / -_) (_-<
                                    \___| \___/ |_||_| /__/  \__| \__,_| |_||_|  \__| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/

extern halfDisp_t halfDispTrophy;
extern halfDisp_t halfDispAlienSpaceInvaders;
extern halfDisp_t halfDispVolume;
extern halfDisp_t halfDispResume;
extern halfDisp_t halfDispRestart;
/*******************************************************************************************************************************************
********************************************************************************************************************************************

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
sem_t SEM_MENU;//Semaforo que regula la ejecucion de los menues.

game_t menuGame = { &KEYS, NULL, NULL, 0}; //Estructura del level handler.

menu_t menuInicio = { &KEYS , {selectPlayInicio, selectLevelsInicio, selectVolumeInicio, selectQuitGameInicio},
                      {"Quick Play    ", "Niveles    ", "Volumen    ", "Salir del juego    "}, 
                      {&halfDispAlienSpaceInvaders, &halfDispResume, &halfDispVolume, &halfDispRestart}, 
                      4 , 1 , changeOption };//Estructura del menu de inicio.

menu_t menuPausa = { &KEYS , {selectPlayInicio, selectLevelsInicio, selectVolumeInicio, selectQuitGameInicio},
                      {"Quick Play    ", "Niveles    ", "Volumen    ", "Salir del juego    "}, 
                      {&halfDispAlienSpaceInvaders, &halfDispResume, &halfDispVolume, &halfDispRestart}, 
                      4 , 1 , changeOption };//Estructura del menu de pausa.

menu_t* MENUES[] = {&menuInicio, &menuPausa};//Arreglo que contiene punteros a todos los menues. No tiene por que estar definido aca, solo lo cree para hacer algo de codigo.
level_setting_t* LEVELS[10];//Arrego que contiene punteros a la config de todos los niveles.

unsigned int timerTick = 1000000;
int velInput = 1;
int velMenu = 20;
int velDispAnimation = 1;
int velInputGame = 10;
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

    disp_init();
    joy_init();

    pthread_t timerT, inputT, menuHandlerT, levelHandlerT, moveAlienT, moveBalaT, displayT;

    sem_init(&SEM_GAME, 0, 1);
    sem_init(&SEM_MENU, 0, 1);

    pthread_create(&timerT, NULL, timer, NULL);

    pthread_create(&inputT, NULL, INPUT_THREAD, &KEYS);

    while(GAME_STATUS.exitStatus){//El juego se ejecuta hasta que se indique lo contrario en exitStatus.

        switch(GAME_STATUS.pantallaActual){//Esta seccion del codigo se encarga de inicializar los threads correctos dependiendo de la pantalla
                                           //actual y de la opcion seleccionada en algun menu.
            case MENU://Entra a este caso cuando el programa se encuentra en cualquier menu.
                
                sem_wait(&SEM_GAME);//Pausa la ejecucion del juego.
                
                MENUES[GAME_STATUS.menuActual] -> exitStatus = 1;

                pthread_create(&menuHandlerT, NULL, menuHandlerThread, MENUES[GAME_STATUS.menuActual]);//Se inicializa el thread de menu handler con el menu indicado.
                
                pthread_join(menuHandlerT, NULL);

                sem_post(&SEM_GAME);

                break;
            
            case START_LEVEL://Entra a este caso cuando se crea un nivel.
            /*    
                sem_wait(&SEM_MENU);

                object_t* alienList = NULL;//Se crea la lista de los aliens.
                //alienList = initAliens(alienList, LEVELS[GAME_STATUS.nivelActual], "STRING QUE DICE LA CANTIDAD DE ALIENS POR FILAS", PABLO, SEXO);
                object_t* listBalasEnemigas;//Lista de las balas de los aliens.
                object_t* listBalasUsr;//Lista de las balas del usuario.
                
                object_t * naveUsuario = (object_t*) malloc(sizeof(object_t));//Crea la nave del usuario

                if(naveUsuario == NULL){\
                    printf("no se pudo crear al usuario por malloc \n");
                }

                level_setting_t settings;
    settings.desplazamientoX = 1;
    settings.desplazamientoY = 1;
    settings.desplazamientoUsr = 1;
    settings.disInicialUsrX = 9;
    settings.disInicialUsrY = 1;
    settings.distInicialX = 2;
    settings.distInicialY = 2;
    settings.margenX = 1;
    settings.margenY = 1;
    settings.saltoX = 4;
    settings.saltoY = 3;
    settings.xMax = 15;
    settings.xMin = 0;
    settings.yMax = 15;
    settings.yMin = 0;
    #define NICOLAS 1
    #define PABLO 2
    #define DANIEL 3
    #define BALANICOLAS 4
    #define BALAPABLO 5
    #define BALADANIEL 6
    #define NAVE 8
    addObjType(NICOLAS, 1, 3, 2, 1, 30, 4, BALANICOLAS);
    addObjType(PABLO, 1, 3, 2, 2, 40, 4, BALAPABLO);
    addObjType(DANIEL, 1, 3, 2, 3, 50, 4, BALADANIEL);
    addObjType(BALANICOLAS, 1, 1, 3, 1, 0, 0, NONEOBJTYPEID);
    addObjType(BALAPABLO, 1, 1, 3, 1, 0, 0, NONEOBJTYPEID);
    addObjType(BALADANIEL, 1, 1, 3, 1, 0, 0, NONEOBJTYPEID);
    alienList = initAliens(alienList, &settings, "20403", NICOLAS, PABLO, DANIEL);
    
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

                menuGame.naveUsr = &naveUsuario;
                //menuGame.levelSettings =
                //Aca tiene que ir el puntero a level settings.
                menuGame.exitStatus = 1;
                pthread_create(&levelHandlerT, NULL, levelHandlerThread, MENUES[GAME_STATUS.menuActual]);//Se inicializa el thread de level handler con el nivel indicado.
                
                argMoveAlien_t argMoveAlien = { .levelSettings = LEVELS[GAME_STATUS.nivelActual] , .alienList = &alienList };//Inicializa el thread Move Alien.
                pthread_create(&moveAlienT, NULL, moveAlienThread, &argMoveAlien);

                argMoveBala_t argMoveBala = { .levelSettings = LEVELS[GAME_STATUS.nivelActual] , .balasEnemigas = &listBalasEnemigas, .balasUsr = &listBalasUsr };//Inicializa el thread Move Bala.
                pthread_create(&moveBalaT, NULL, moveBalaThread, &argMoveBala);

                pthread_join(levelHandlerT, NULL);//Espera hasta que se cree un menu.
                
                sem_wait(&SEM_MENU);
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

    //*****************************************     Inicializa el thread que barre el display       *****************************
    pthread_t displayMenuT;

    #ifdef RASPI
        halfDisp_t higherDispMenu = {//Parte superior del display
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        };

        halfDisp_t lowerDispMenu = {//Parte inferior del display
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        };

        int animStatus = 1;

        argTextAnimMenu_t argTextAnimMenu = { (menu -> textOpciones)[select],  &lowerDispMenu, &higherDispMenu, (menu -> drawingOpciones)[select], IZQUIERDA, &animStatus};
    #endif

    pthread_create(&displayMenuT, NULL, DISP_ANIM_MENU, &argTextAnimMenu);

    //***************************************************************************************************************************

    while(menu -> exitStatus){
        usleep(10 * U_SEC2M_SEC);
        if( (timerTick % velMenu) == 0 ){
            sem_wait(&SEM_MENU);
            
            if (DERECHA_INPUT){//Si se presiona para ir a la siguiente opcion

                printf("Siguiente opcion \n");
                select += 1;
                if(select == (menu -> cantOpciones)){//Si llegamos a la ultima opcion pasamos a la primera
                    select = 0;
                }
                
                argChangeOption_t argChangeOption = { &displayMenuT, &animStatus, &lowerDispMenu, &higherDispMenu, (menu -> drawingOpciones)[select], (menu -> textOpciones)[select], IZQUIERDA };
                (menu -> changeOption)(&argChangeOption);
                
            }

            if (IZQUIERDA_INPUT){//Si se presiona para ir a la opcion anterior

                select -= 1;
                if(select < 0){//Si llegamos a la primer opcion pasamos a al ultima
                    select = (menu -> cantOpciones) - 1;
                }
                argChangeOption_t argChangeOption = { &displayMenuT, &animStatus, &lowerDispMenu, &higherDispMenu, (menu -> drawingOpciones)[select], (menu -> textOpciones)[select], DERECHA };
                (menu -> changeOption)(&argChangeOption);
                
            }

            if (PRESS_INPUT){//Si se selecciona la opcion
                menu -> exitStatus = (menu->selectOption[select])();//Se llama al callback que indica que accion realizar al presionar dicha opcion.
            }
            sem_post(&SEM_MENU);
        }
    }
    animStatus = 0;

    pthread_join(displayMenuT, NULL);
    
    pthread_exit(0);
}


/*
static void* levelHandlerThread(void * data){

	game_t * menu = (game_t *) data;

    while(menu -> exitStatus){

        if (PRESS_INPUT){//Si se presiono el joystick se debe pausar el juego.
                GAME_STATUS.menuActual = MENU_PAUSA;//Indica que se pauso el juego.
                GAME_STATUS.pantallaActual = MENU;
                menu -> exitStatus = 0;//Indica que hay que salir del level Handler
        }

        usleep(10 * U_SEC2M_SEC);
        if( ((timerTick % velInputGame) == 0) && menu -> exitStatus ){
            if (ARRIBA_INPUT){//Dispara una bala
                //Disparar una bala
            }

            if (DERECHA_INPUT){//Mueve al usuario
                moveNaveUsuario(menu -> naveUsr, menu -> levelSettings, DERECHA);
            }

            if (IZQUIERDA_INPUT){
                moveNaveUsuario(menu -> naveUsr, menu -> levelSettings, IZQUIERDA);
            }
        }
    }

    pthread_exit(0);
}
*/


/*******************************************************************************************************************************************
 * 
                                         _     _   _                                                     ___          _        
             _ __    ___  __ __  ___    /_\   | | (_)  ___   _ _      _  _     _ __    ___  __ __  ___  | _ )  __ _  | |  __ _ 
            | '  \  / _ \ \ V / / -_)  / _ \  | | | | / -_) | ' \    | || |   | '  \  / _ \ \ V / / -_) | _ \ / _` | | | / _` |
            |_|_|_| \___/  \_/  \___| /_/ \_\ |_| |_| \___| |_||_|    \_, |   |_|_|_| \___/  \_/  \___| |___/ \__,_| |_| \__,_|
                                                                    |__/                                                                                                                                                                                                                                                   
 * 
 ******************************************************************************************************************************************/
/*
//******************************************    Thread moveAlien    **********************************************************
void * moveAlienThread(void* argMoveAlien){
    //Este thread se encarga de mover la posicion de los aliens teniendo en cuenta para ello la variable direccion.
    
    int static direccion = DERECHA; //Determina la direccion en la que se tienen que mover los aliens en el proximo tick
    while(1){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velAliens) == 0 ){

            sem_wait(&SEM_GAME);

            moveAlien( ((argMoveAlien_t*)argMoveAlien) -> levelSettings,  *(((argMoveAlien_t*)argMoveAlien) -> alienList), direccion);

            sem_post(&SEM_GAME);
        }
    }
    pthread_exit(0);
}

void * moveBalaThread(void * argMoveBala){

    while(1){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velBalas) == 0 ){
            sem_wait(&SEM_GAME);
            object_t * balas = NULL;
            //printf("%d   %d", Y_MAX_L(argMoveBala), Y_MIN_L(argMoveBala));
            //printf("%p   ", BALAS_ENEMIGAS_L(argMoveBala));.
            //printf("GEORGE %p   ",  BALAS_ENEMIGAS_L(argMoveBala));
            (*(((argMoveBala_t *) argMoveBala) -> balasEnemigas))  = moveBala(*(((argMoveBala_t *) argMoveBala) -> balasEnemigas), ((argMoveBala_t *) argMoveBala) -> levelSettings);
            //printf("%p   ", ((argMoveBala_t*) argMoveBala) -> balasEnemigas);
            (*(((argMoveBala_t *) argMoveBala) -> balasUsr)) = moveBala(*(((argMoveBala_t *) argMoveBala) -> balasUsr), ((argMoveBala_t *) argMoveBala) -> levelSettings);
            //printf("%p        ", ((argMoveBala_t*) argMoveBala) -> balasEnemigas);
            //printf("%p\n", ((argMoveBala_t*) argMoveBala) -> balasUsr);
            sem_post(&SEM_GAME);
        } 
    }
}
*/