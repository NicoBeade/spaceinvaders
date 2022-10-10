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
#include "levelLoader/levelLoader.h"

#define RASPI 

#ifdef RASPI
#include "raspi/inputRaspi.h"
#include "raspi/displayRaspi.h"
#include "raspi/drivers/disdrv.h"
#include "raspi/drivers/joydrv.h"
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
    object_t ** alienList;
}argMoveBala_t;

typedef struct{
	level_setting_t * levelSettings;
    object_t ** alienList;
    object_t ** usrList;
	object_t ** balasEnemigas;
	object_t ** balasUsr;
}argCollider_t;

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

static void* levelHandlerThread(void * data);

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

game_t menuGame = { &KEYS, NULL, NULL, NULL, 0}; //Estructura del level handler.

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
int velAliens = 100;\
int velBalas = 50;
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
        printf("timer tick: %d\n", timerTick);
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

    object_t * alienList = NULL; //Se crea la lista de aliens
    object_t * UsrList = NULL; //Se crea la lista de nave usuario
    object_t * barrerasList = NULL; //Se crea la lista de barreras
    object_t * balasUsr = NULL; //Se crea la lista de las balas del usuario
    object_t * balasAlien = NULL; //Se crea la lista de las balas de los aliens

    level_setting_t levelSettings;

    int levelCounter = 0;
    #ifdef RASPI
    char platform[4] = "rpi";
    #endif
    #ifdef ALLEGRO
    char platform[4] = "lnx";
    #endif

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
                sem_wait(&SEM_MENU);
                if(levelCounter == 0){
                    directory_t carpetaAssets = {};
                    loadDirectory("game/assets", &carpetaAssets);   //ESTO HAY QUE CAMBIARLO ESTA HARCODEADO
                    loadAllAssets("rpi", &carpetaAssets);   //ESTO HAY QUE CAMBIARLO ESTA HARCODEADO
                    int levelStatus = loadLevel(levelCounter, &levelSettings, &(platform[0]), &alienList, &UsrList, &barrerasList);
                    if(levelStatus == -1){
                        printf("Error in spaceInvaders.c, level number 0 not found\n");
                        return -1;
                    }
                    levelCounter++;
                }
                int levelStatus = loadLevel(levelCounter, &levelSettings, &(platform[0]), &alienList, &UsrList, &barrerasList);
                vector_t pos;
                pos.x = levelSettings.disInicialUsrX;
                pos.y = levelSettings.disInicialUsrY;
                UsrList = addObj(UsrList, pos,2, 3);  //ESTO HAY QUE CAMBIARLO ESTA HARCODEADO
                if(levelStatus == -1){
                    printf("Error in spaceInvaders.c, Couldnt start level\n");
                    return -1;
                }
                else if (levelStatus == -2){        //Si es -2 termina el juego
                    GAME_STATUS.exitStatus = 0;
                }
                if(alienList == NULL){
                    printf("Error in spaceInvaders.c, Couldnt start level, alienList null\n");
                    return -1;
                }
                if(UsrList == NULL){
                    printf("Error in spaceInvaders.c, Couldnt start level, UsrList null\n");
                    return -1;
                }
               /* if(barrerasList == NULL){
                    printf("Error in spaceInvaders.c, Couldnt start level, barrerasList null\n");
                    return -1;
                }
                */
                levelCounter++;
                printf("Por crear move alien y move bala\n");
                argMoveAlien_t argMoveAlien = { &levelSettings, &alienList };
                argMoveBala_t argMoveBala = { &levelSettings, &balasAlien, &balasUsr, &alienList };
                pthread_create(&moveAlienT, NULL, moveAlienThread, &argMoveAlien);
                pthread_create(&moveBalaT, NULL, moveBalaThread, &argMoveBala);
                printf("Move alien y bala creados\n");

                #ifdef RASPI
                argDisplayRPI_t argDisplayRPI = { &alienList, &UsrList };
                pthread_create(&displayT, NULL, displayRPIThread, &argDisplayRPI);
                #endif
                printf("Display Raspi creado\n");

                #ifdef ALLEGRO
                
                #endif

                menuGame.naveUsr = &UsrList;
                menuGame.levelSettings = &levelSettings;
                menuGame.balasUsr = &balasUsr;
                menuGame.exitStatus = 1;

                pthread_create(&levelHandlerT, NULL, levelHandlerThread, &menuGame);//Se inicializa el thread de level handler con el nivel indicado.
                printf("Level handler creado \n");
                pthread_join(levelHandlerT, NULL);//Espera hasta que se cree un menu.

                sem_post(&SEM_MENU);
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
            
            if (DERECHA_INPUT){//Si se presiona para ir a la siguiente opcion

                printf("Siguiente opcion \n");
                select += 1;
                if(select == (menu -> cantOpciones)){//Si llegamos a la ultima opcion pasamos a la primera
                    select = 0;
                }

                #ifdef RASPI
                argChangeOption_t argChangeOption = { &displayMenuT, &animStatus, &lowerDispMenu, &higherDispMenu, (menu -> drawingOpciones)[select], (menu -> textOpciones)[select], IZQUIERDA };
                #endif

                (menu -> changeOption)(&argChangeOption);
                
            }

            if (IZQUIERDA_INPUT){//Si se presiona para ir a la opcion anterior

                select -= 1;
                if(select < 0){//Si llegamos a la primer opcion pasamos a al ultima
                    select = (menu -> cantOpciones) - 1;
                }

                #ifdef RASPI
                argChangeOption_t argChangeOption = { &displayMenuT, &animStatus, &lowerDispMenu, &higherDispMenu, (menu -> drawingOpciones)[select], (menu -> textOpciones)[select], IZQUIERDA };
                #endif

                (menu -> changeOption)(&argChangeOption);
                
            }

            if (PRESS_INPUT){//Si se selecciona la opcion
                menu -> exitStatus = (menu->selectOption[select])();//Se llama al callback que indica que accion realizar al presionar dicha opcion.
            }
            
        }
    }
    animStatus = 0;

    pthread_join(displayMenuT, NULL);
    
    pthread_exit(0);
}



static void* levelHandlerThread(void * data){

	game_t * menu = (game_t *) data;
    printf("Level Handler\n");
    while(menu -> exitStatus){

        if (PRESS_INPUT){//Si se presiono el joystick se debe pausar el juego.
                GAME_STATUS.menuActual = MENU_PAUSA;//Indica que se pauso el juego.
                GAME_STATUS.pantallaActual = MENU;
                menu -> exitStatus = 0;//Indica que hay que salir del level Handler
        }

        usleep(10 * U_SEC2M_SEC);
        if( ((timerTick % velInputGame) == 0) && menu -> exitStatus ){
            if (ARRIBA_INPUT){//Dispara una bala
                *(menu -> balasUsr) = shootBala(*(menu -> naveUsr), *(menu -> balasUsr), menu -> levelSettings);
            }

            if (DERECHA_INPUT){//Mueve al usuario
                moveNaveUsuario(*(menu -> naveUsr), menu -> levelSettings, DERECHA);
            }

            if (IZQUIERDA_INPUT){
                moveNaveUsuario(*(menu -> naveUsr), menu -> levelSettings, IZQUIERDA);
            }
        }
    }

    pthread_exit(0);
}



/*******************************************************************************************************************************************
 * 
                                         _     _   _                                                     ___          _        
             _ __    ___  __ __  ___    /_\   | | (_)  ___   _ _      _  _     _ __    ___  __ __  ___  | _ )  __ _  | |  __ _ 
            | '  \  / _ \ \ V / / -_)  / _ \  | | | | / -_) | ' \    | || |   | '  \  / _ \ \ V / / -_) | _ \ / _` | | | / _` |
            |_|_|_| \___/  \_/  \___| /_/ \_\ |_| |_| \___| |_||_|    \_, |   |_|_|_| \___/  \_/  \___| |___/ \__,_| |_| \__,_|
                                                                    |__/                                                                                                                                                                                                                                                   
 * 
 ******************************************************************************************************************************************/

//******************************************    Thread moveAlien    **********************************************************
void * moveAlienThread(void* argMoveAlien){
    //Este thread se encarga de mover la posicion de los aliens teniendo en cuenta para ello la variable direccion.
    printf("Move Alien \n");
    static int direccion = DERECHA; //Determina la direccion en la que se tienen que mover los aliens en el proximo tick
    while(1){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velAliens) == 0 ){
            printf("Move Alien 1\n");
            sem_wait(&SEM_GAME);
            printf("Se ingreso a move alien\n");
            moveAlien( ((argMoveAlien_t*)argMoveAlien) -> levelSettings,  (((argMoveAlien_t*)argMoveAlien) -> alienList), &direccion);

            sem_post(&SEM_GAME);
        }
    }
    pthread_exit(0);
}

void * moveBalaThread(void * argMoveBala){
    printf("Move Bala \n");
    argMoveBala_t * data = (argMoveBala_t*)argMoveBala;

    while(1){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velBalas) == 0 ){
            printf("Move Bala 1\n");
            sem_wait(&SEM_GAME);
            printf("Se ingreso a moveBala \n");

            *(data -> balasEnemigas) = shootBala(*(data -> alienList), *(data -> balasEnemigas), data -> levelSettings);

            printf("ShootBala \n");

            //printf("%d   %d", Y_MAX_L(argMoveBala), Y_MIN_L(argMoveBala));
            //printf("%p   ", BALAS_ENEMIGAS_L(argMoveBala));.
            //printf("GEORGE %p   ",  BALAS_ENEMIGAS_L(argMoveBala));
            (*(data -> balasEnemigas))  = moveBala(*(data -> balasEnemigas), data -> levelSettings);
            printf("MoveBala \n");
            //printf("%p   ", ((argMoveBala_t*) argMoveBala) -> balasEnemigas);
            (*(data -> balasUsr)) = moveBala(*(data -> balasUsr), data -> levelSettings);
            printf("MoveBala \n");
            //printf("%p        ", ((argMoveBala_t*) argMoveBala) -> balasEnemigas);
            //printf("%p\n", ((argMoveBala_t*) argMoveBala) -> balasUsr);
            sem_post(&SEM_GAME);
        } 
    }
}
