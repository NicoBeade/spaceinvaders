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
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "menuUtilities.h"

#ifdef RASPI
#include "raspi/inputRaspi.h"
#include "raspi/drivers/disdrv.h"
#include "raspi/drivers/joydrv.h"
#include "raspi/audioHandlerRaspi.h"
#endif

#ifdef ALLEGRO

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
    audioCallback_t audioCallback;
}argMoveAlien_t;

typedef struct{
    level_setting_t * levelSettings;
    object_t ** mothership;
    audioCallback_t audioCallback;
}argMoveMothership_t;

typedef struct{
	level_setting_t * levelSettings;
	object_t ** balasEnemigas;
	object_t ** balasUsr;
    object_t ** alienList;
    audioCallback_t audioCallback;
}argMoveBala_t;

typedef struct{
	level_setting_t * levelSettings;
    object_t ** alienList;
    object_t ** usrList;
    object_t ** barriersList;
	object_t ** balasEnemigas;
	object_t ** balasUsr;
    object_t ** motherShip;
    int * score;//Almacena el score del usuario.
    int * scoreInstantaneo;//Muestra el score a medida que se va actualizando sin necesidad de ganar el nivel.
    int nivelActual;//Indica el nivel que se esta jugando.
    audioCallback_t audioCallback;
}argCollider_t;

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

void * moveAlienThread(void* argMoveAlien);

void * moveMothershipThread(void* argMoveMothership);

void * moveBalaThread(void * argMoveBala);

void * colliderThread(void * argCollider);

static void* menuHandlerThread(void * data);

static void* saveScoreHandlerThread(void * data);

static void* levelHandlerThread(void * data);

/*******************************************************************************************************************************************
 * 
                __   __               _          _      _                 ___   _         _             _            
                \ \ / /  __ _   _ _  (_)  __ _  | |__  | |  ___   ___    / __| | |  ___  | |__   __ _  | |  ___   ___
                 \ V /  / _` | | '_| | | / _` | | '_ \ | | / -_) (_-<   | (_ | | | / _ \ | '_ \ / _` | | | / -_) (_-<
                  \_/   \__,_| |_|   |_| \__,_| |_.__/ |_| \___| /__/    \___| |_| \___/ |_.__/ \__,_| |_| \___| /__/
 * 
 ******************************************************************************************************************************************/

#define MAX_USR_LIVES 5

gameStatus_t GAME_STATUS = { .pantallaActual = MENU, .pantallaAnterior = MENU, .nivelActual = 0 , .menuActual = 0, .menuAnterior = -1, .inGame = 0, .usrLives = MAX_USR_LIVES, .exitStatus = 1};

keys_t KEYS = { .x =0, .y = 0, .press = 0 };//Almacena las teclas presionadas por el usuario.

sem_t SEM_GAME;//Semaforo que regula la ejecucion de los niveles.
sem_t SEM_MENU;//Semaforo que regula la ejecucion de los menues.
sem_t SEM_DRIVER;

game_t menuGame = { &KEYS, NULL, NULL, NULL, 0, NULL, NULL}; //Estructura del level handler.

#ifdef ALLEGRO
texto_t * toText = NULL;
sprite_t * screenObjects = NULL;
TextObj_t listasAllegro = {NULL, NULL};
TextObj_t * allegroList = &listasAllegro;
#endif

menu_t* MENUES[] = {&menuInicio, &menuPausa, &menuWonLevel, &menuLostLevel, &menuLeaderboard, &menuLevels, &menuVolume};//Arreglo que contiene punteros a todos los menues. No tiene por que estar definido aca, solo lo cree para hacer algo de codigo.
level_setting_t* LEVELS[10];//Arrego que contiene punteros a la config de todos los niveles.

char stringWithScore[20];

unsigned int timerTick = 1000000;

//------------------------    Variables de velocidad de ejecucion de threads     ------------------------
#ifdef RASPI
const int velMenu = 20;         //Velocidad a la que se lee el input durante un menu
const int velCollider = 1;      //Velocidad a la que se ejecuta el collider
int velDispAnimation = 2;       //Velocidad a la que se realiza el barrido del display durante un menu
const int velInputGameShoot = 5;//Velocidad a la que se lee el input para el disparo del usuario durante el juego.
const int velInputGameMoove = 5;//Velocidad a la que se lee el input para el movimiento del usuario durante el juego.
#define STOP_SHOOT 10
#define VEL_INCR_ALIENS 2
#endif
#ifdef ALLEGRO
const int velMenu = 5;         //Velocidad a la que se lee el input durante un menu
const int velCollider = 10;     //Velocidad a la que se realiza el barrido del display durante un menu
const int velInputGameMoove = 2;//Velocidad a la que se lee el input para el movimiento del usuario durante el juego.
const int velInputGameShoot = 2;//Velocidad a la que se lee el input para el disparo del usuario durante el juego.
#define VEL_INCR_ALIENS 5
#define STOP_SHOOT 20
#endif
const int velInput = 1;

int velAliens;
int velMothership;
int velBalas;
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

    srand(time(NULL));
    directory_t carpetaNiveles = {};

    #ifdef RASPI//Inicializa los drives para la raspi
    disp_init();
    joy_init();
    pthread_t displayT;
    #endif

    pthread_t timerT, inputT, menuHandlerT, levelHandlerT, moveAlienT, moveBalaT, colliderT, mothershipT, saveScoreT;
    
    sem_init(&SEM_GAME, 0, 1);//Inicializa los semaforos
    sem_init(&SEM_MENU, 0, 1);
    sem_init(&SEM_DRIVER, 0, 1);

    pthread_create(&timerT, NULL, timer, NULL);

    object_t * alienList = NULL; //Se crea la lista de aliens
    object_t * UsrList = NULL; //Se crea la lista de nave usuario
    object_t * barrerasList = NULL; //Se crea la lista de barreras
    object_t * balasUsr = NULL; //Se crea la lista de las balas del usuario
    object_t * balasAlien = NULL; //Se crea la lista de las balas de los aliens
    object_t * mothershipList = NULL; //Se crea la lista de la nave nodriza.

    level_setting_t levelSettings;//Esta variable almacena la informacion de cada nivel

    int score = 0;  //Esta variable almacena el puntaje del usuario.
    int scoreInstantaneo = 0; //Almacena el score en todo momento sin necesidad de ganar el nivel.

    #ifdef ALLEGRO
    punteros_t punteros = {&alienList, &UsrList, &barrerasList, &balasUsr, &balasAlien, &mothershipList, &screenObjects};
    data_allegro_t dataIn = {punteros, &toText, &KEYS};
    data_allegro_t * dataInput = &dataIn;
    #endif

    #ifdef RASPI
    keys_t * dataInput = &KEYS;
    #endif

    pthread_create(&inputT, NULL, INPUT_THREAD, dataInput);//Comienza a leer el input

    audioCallback_t audioCallback;
    volumeCallback_t volumeCallback;
    
    #ifdef RASPI
    char platform[4] = "rpi";
    //Inicializacion del audio de raspi
    if (initAudio() == NO_INIT){
        fprintf(stderr, "Audio not initilized.\n");
	    endAudio();
	    return -1;
    }
    audioCallback = audioHandlerRaspi;
    volumeCallback = regVolumeRaspi;
    #endif

    #ifdef ALLEGRO
    char platform[4] = "lnx";

    audioCallback = playAudioAllegro;
    volumeCallback = regAudioAllegro;
    #endif

    menuGame.audioCallback = audioCallback;

    level_t levelArray[MAX_LEVEL];
    indexAllLevels(platform,LEVELSDIR, LEVELPREFIX, &carpetaNiveles, levelArray);
    
    menuLevels.cantOpciones = levelArrayLen(levelArray) -1;
    
    //Se carga el nivel 0

    directory_t carpetaAssets = {};
    loadDirectory(ASSETSDIR, &carpetaAssets);   
    loadAllAssets(platform, &carpetaAssets);   
    int levelStatus = loadLevel(GAME_STATUS.nivelActual, levelArray, &levelSettings, &(platform[0]), &alienList, &UsrList, &barrerasList);

    if(levelStatus == -1){
        printf("Error in spaceInvaders.c, level number 0 not found\n");
        return -1;
    }
    GAME_STATUS.nivelActual = 1;
    
    while(GAME_STATUS.exitStatus){//El juego se ejecuta hasta que se indique lo contrario en exitStatus.

        switch(GAME_STATUS.pantallaActual){//Esta seccion del codigo se encarga de inicializar los threads correctos dependiendo de la pantalla
                                           //actual y de la opcion seleccionada en algun menu.
            case MENU://-------------------------------------    MENU:  Entra a este caso cuando el programa se encuentra en cualquier menu.    ------------------------------------

                if(GAME_STATUS.menuActual == MENU_INICIO){//Detecta si se tiene que comenzar a reproducir la muscia del menu.
                    audioCallback(MUSICA_JUEGO);
                }

                #ifdef RASPI
                if(GAME_STATUS.menuActual == MENU_PAUSA){
                    sprintf(menuPausa.textOpciones[0],"%s%d%s%d    ","Resume   Score > ", scoreInstantaneo,"   Lives > ", UsrList->lives);
                }
                #endif
                
                sem_wait(&SEM_GAME);//Pausa la ejecucion del juego.
                
                MENUES[GAME_STATUS.menuActual] -> exitStatus = 1;

                MENUES[GAME_STATUS.menuActual] -> audioCallback = audioCallback; 

                MENUES[GAME_STATUS.menuActual] -> volumeCallback = volumeCallback;


                pthread_create(&menuHandlerT, NULL, menuHandlerThread, MENUES[GAME_STATUS.menuActual]);//Se inicializa el thread de menu handler con el menu indicado.
                
                pthread_join(menuHandlerT, NULL);

                sem_post(&SEM_GAME);

                break;
            
            case SAVE_SCORE://-----------------------------     SAVE_SCORE: Entra a este caso cuando el usuario desea cargar su score.      ----------------------------------------

                sem_wait(&SEM_GAME);//Pausa la ejecucion del juego.

                #ifdef RASPI
                sprintf(stringWithScore,"%d    ",score);
                #endif

                #ifdef ALLEGRO
                sprintf(stringWithScore,"%d",score);
                #endif

                saveScore_t saveScore = { &KEYS, stringWithScore, score, 1, 3, &audioCallback};

                pthread_create(&saveScoreT, NULL, saveScoreHandlerThread, &saveScore);//Se inicializa el thread de menu handler con el menu indicado.
                
                pthread_join(saveScoreT, NULL);

                sem_post(&SEM_GAME);

                break;
            
            case START_LEVEL://-----------------------------    START_LEVEL: Entra a este caso cuando se crea un nivel.     ---------------------------------------------------------

                GAME_STATUS.pantallaAnterior = START_LEVEL;
                
                sem_wait(&SEM_MENU);

                int levelStatus = loadLevel(GAME_STATUS.nivelActual, levelArray, &levelSettings, &(platform[0]), &alienList, &UsrList, &barrerasList);

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

                velAliens = levelSettings.velAliens;
                velMothership = levelSettings.velMothership;
                velBalas = levelSettings.velBalas;

                GAME_STATUS.inGame = 1;

                UsrList->lives = GAME_STATUS.usrLives;

                //Inicializa los threads encargados de controlar el juego.
                argMoveAlien_t argMoveAlien = { &levelSettings, &alienList, audioCallback};
                argMoveMothership_t argMoveMothership = {&levelSettings, &mothershipList, audioCallback};
                argMoveBala_t argMoveBala = { &levelSettings, &balasAlien, &balasUsr, &alienList, audioCallback };
                argCollider_t argCollider = { &levelSettings, &alienList, &UsrList, &barrerasList, &balasAlien, &balasUsr, &mothershipList, &score, &scoreInstantaneo, GAME_STATUS.nivelActual, audioCallback };
                pthread_create(&moveAlienT, NULL, moveAlienThread, &argMoveAlien);
                pthread_create(&mothershipT, NULL, moveMothershipThread, &argMoveMothership);
                pthread_create(&moveBalaT, NULL, moveBalaThread, &argMoveBala);
                pthread_create(&colliderT, NULL, colliderThread, &argCollider);

                #ifdef RASPI
                argDisplayRPI_t argDisplayRPI = {&balasAlien, &balasUsr, &alienList, &UsrList, &barrerasList, &mothershipList };
                pthread_create(&displayT, NULL, displayRPIThread, &argDisplayRPI);
                #endif

                menuGame.naveUsr = &UsrList;
                menuGame.levelSettings = &levelSettings;
                menuGame.balasUsr = &balasUsr;
                menuGame.exitStatus = 1;
                menuGame.scoreInstantaneo = &scoreInstantaneo;

                pthread_create(&levelHandlerT, NULL, levelHandlerThread, &menuGame);//Se inicializa el thread de level handler con el nivel indicado.
                pthread_join(levelHandlerT, NULL);//Espera hasta que se cree un menu.

                sem_post(&SEM_MENU);

                break;

            case IN_GAME://--------------------------   IN_GAME: Entra a este caso cuadno se reanuda un nivel.      -------------------------------------------------------------------
                
                GAME_STATUS.pantallaAnterior = IN_GAME;

                sem_wait(&SEM_MENU);
                pthread_create(&levelHandlerT, NULL, levelHandlerThread, &menuGame);//Se inicializa el thread de level handler con el nivel indicado.

                pthread_join(levelHandlerT, NULL);//Espera hasta que se cree un menu.

                sem_post(&SEM_MENU);
                break;

            case DESTROY_LEVEL://---------------------      DESTROY_LEVEL: Entra a este caso cuando hay que eliminar las listas del heap. Como cuadno se pierde un nivel.   -----------

                GAME_STATUS.inGame = 0;

                usleep(50 * U_SEC2M_SEC);
                //Elimina todas las listas del heap.
                if(alienList != NULL){
                    alienList = removeList(alienList); 
                }
                if(UsrList != NULL){
                    UsrList = removeList(UsrList);
                }
                if(barrerasList != NULL){
                    barrerasList = removeList(barrerasList);
                }
                if(balasAlien != NULL){
                    balasAlien = removeList(balasAlien);
                }
                if(balasUsr != NULL){
                    balasUsr = removeList(balasUsr);
                }
                if(mothershipList != NULL){
                    mothershipList = removeList(mothershipList);
                }

                if(GAME_STATUS.menuActual == START_LEVEL_MENU){
                    GAME_STATUS.pantallaActual = START_LEVEL; //Si hay que iniciar nuevamente el juego
                }
                else{
                    GAME_STATUS.pantallaActual = MENU; //Se pasa al menu correspondiente.
                }
                break;

            case QUIT_GAME://-------------------------      QUIT_GAME: Entra a este caso cuadno se quiere salir del juego.      ------------------------------------------------------
                GAME_STATUS.inGame = 0;

                usleep(50 * U_SEC2M_SEC);
                //Elimina todas las listas del heap.
                if(alienList != NULL){
                    alienList = removeList(alienList); 
                }
                if(UsrList != NULL){
                    UsrList = removeList(UsrList);
                }
                if(barrerasList != NULL){
                    barrerasList = removeList(barrerasList);
                }
                if(balasAlien != NULL){
                    balasAlien = removeList(balasAlien);
                }
                if(balasUsr != NULL){
                    balasUsr = removeList(balasUsr);
                }
                if(mothershipList != NULL){
                    mothershipList = removeList(mothershipList);
                }

                GAME_STATUS.exitStatus = 0;

                break;

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

    unsigned char stopSweep = 0;//Esta variable se utiliza para evitar que el usuario pueda cambiar de opcion muy rapido

    int select = 0;//Esta variable se utiliza para indicar la opcion seleccionada dentro del menu.
    if(GAME_STATUS.menuActual == MENU_VOLUME){
        select=(menu->volumeCallback)(CHECK_AUDIO);            
    }
    //*****************************************     Inicializa el thread que barre el display       *****************************
    #ifdef RASPI

        halfDisp_t* halfDispNameScore;

        halfDisp_t nameDispMenu = {//Matriz para almacenar el nombre del jugador a mostrar.
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
            };

        if(GAME_STATUS.menuActual == MENU_LEADERBOARD){//Si hay que rellenar utilizando el leaderBoard.
            halfDispNameScore = &nameDispMenu;
            (menu -> drawingOpciones)[select] = getLeaderBoardName(halfDispNameScore, select);
        }

        pthread_t displayMenuT;
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

        argTextAnimMenu_t argTextAnimMenu = { (menu -> textOpciones)[select],  &lowerDispMenu, &higherDispMenu, (menu -> drawingOpciones)[select], IZQUIERDA, &animStatus, GAME_STATUS.menuActual};
    
        pthread_create(&displayMenuT, NULL, DISP_ANIM_MENU, &argTextAnimMenu);
    #endif

    #ifdef ALLEGRO
        int preSelect = 0;//Esta variable se utiliza para almacenar el valor previo de opcion seleccionada a la ahora de cambiarlo.
        
        sem_wait(&SEM_MENU);
        if(GAME_STATUS.menuActual != MENU_LEADERBOARD){
            allegroList = allegroMenu(MENUES[GAME_STATUS.menuActual], allegroList);
            toText = allegroList->textoList;
            screenObjects = allegroList->spriteList;
        }else{
            allegroList = allegroLiderboard(MENUES[GAME_STATUS.menuActual], allegroList);
            toText = allegroList->textoList;
            screenObjects = allegroList->spriteList;
        }
        sem_post(&SEM_MENU);
    #endif
    //***************************************************************************************************************************

    if(GAME_STATUS.menuActual == MENU_LEADERBOARD){//Si estamos en el menu del leaderboard hay que llenar el texto de los menues con los puntajes
        fillLeaderboardMenu(menu);
    }

    usleep(200 * U_SEC2M_SEC);
    while(menu -> exitStatus){

        usleep(10 * U_SEC2M_SEC);
        if( (timerTick % velMenu) == 0 ){
            
            #ifdef ALLEGRO
            if(stopSweep){
                stopSweep -= 1;
            }
            #endif

            if (SIGUIENTE && !stopSweep){//Si se presiona para ir a la siguiente opcion
                #ifdef ALLEGRO
                preSelect = select;
                #endif
                select += 1;
                if(select == (menu -> cantOpciones) && GAME_STATUS.menuActual != MENU_VOLUME){//Si llegamos a la ultima opcion pasamos a la primera
                    select = 0;
                }
                else if(select == (menu -> cantOpciones) && GAME_STATUS.menuActual == MENU_VOLUME){
                    (menu->volumeCallback)(SUBIR_AUDIO);
                    select -= 1;
                }

                #ifdef RASPI
                if(GAME_STATUS.menuActual == MENU_LEADERBOARD){//Si hay que rellenar utilizando el leaderBoard.
                    (menu -> drawingOpciones)[select] = getLeaderBoardName(halfDispNameScore, select);        
                }
                argChangeOption_t argChangeOption = { &displayMenuT, &animStatus, &lowerDispMenu, &higherDispMenu, (menu -> drawingOpciones)[select], (menu -> textOpciones)[select], IZQUIERDA, GAME_STATUS.menuActual };
                
                if(GAME_STATUS.menuActual == MENU_VOLUME){
                    (menu->volumeCallback)(SUBIR_AUDIO);
                }
                
                #endif

                #ifdef ALLEGRO
                changeOptionData_t argChangeOption = { &toText, &screenObjects, preSelect, select, menu};
                stopSweep = 4;
                
                if(GAME_STATUS.menuActual == MENU_VOLUME){
                    (menu->volumeCallback)(BAJAR_AUDIO);
                }
                #endif

                (menu -> changeOption)(&argChangeOption);
                
                if(GAME_STATUS.menuActual != MENU_VOLUME){
                    (menu->audioCallback)(SWAP_MENU);                
                }

                #ifdef ALLEGRO
                if(GAME_STATUS.menuActual == MENU_VOLUME){
                    (menu->audioCallback)(SELECT_MENU);
                }
                #endif
            }

            if (ANTERIOR && !stopSweep){//Si se presiona para ir a la opcion anterior
                #ifdef ALLEGRO
                preSelect = select;
                #endif
                select -= 1;
                if(select < 0 && GAME_STATUS.menuActual != MENU_VOLUME){//Si llegamos a la ultima opcion pasamos a la primera
                    select = (menu -> cantOpciones) - 1;
                }
                else if(select < 0 && GAME_STATUS.menuActual == MENU_VOLUME){
                    (menu->volumeCallback)(BAJAR_AUDIO);
                    select += 1;
                }

                #ifdef RASPI
                if(GAME_STATUS.menuActual == MENU_LEADERBOARD){//Si hay que rellenar utilizando el leaderBoard.
                    (menu -> drawingOpciones)[select] = getLeaderBoardName(halfDispNameScore, select);
                }
                argChangeOption_t argChangeOption = { &displayMenuT, &animStatus, &lowerDispMenu, &higherDispMenu, (menu -> drawingOpciones)[select], (menu -> textOpciones)[select], DERECHA, GAME_STATUS.menuActual };
                
                if(GAME_STATUS.menuActual == MENU_VOLUME){
                    (menu->volumeCallback)(BAJAR_AUDIO);
                }
                #endif

                #ifdef ALLEGRO
                changeOptionData_t argChangeOption = { &toText, &screenObjects, preSelect, select, menu};
                stopSweep = 4;
                
                if(GAME_STATUS.menuActual == MENU_VOLUME){
                    (menu->volumeCallback)(SUBIR_AUDIO);
                }
                #endif                

                (menu -> changeOption)(&argChangeOption);

                if(GAME_STATUS.menuActual != MENU_VOLUME){
                    (menu->audioCallback)(SWAP_MENU);                
                }

                #ifdef ALLEGRO
                if(GAME_STATUS.menuActual == MENU_VOLUME){
                    (menu->audioCallback)(SELECT_MENU);
                }
                #endif
            }

            if (PRESS_INPUT){//Si se selecciona la opcion

                if(GAME_STATUS.menuActual == MENU_LEVELS){
                    GAME_STATUS.nivelActual = select + 1;
                    GAME_STATUS.menuActual = START_LEVEL_MENU;
                    GAME_STATUS.menuAnterior = -1;
                    GAME_STATUS.pantallaActual = DESTROY_LEVEL;
                    menu -> exitStatus = 0;
                }
                else{
                    menu -> exitStatus = (menu->selectOption[select])();//Se llama al callback que indica que accion realizar al presionar dicha opcion.
                }
                if(menu -> exitStatus == 0){
                    (menu->audioCallback)(SELECT_MENU);
                }
            }
            if(ATRAS && GAME_STATUS.menuAnterior != -1){//Si se quiere volver al menu anterior
                menu -> exitStatus = (menu->backMenuAnterior)();//Se llama al callback que indica que accion realizar al volver hacia atras.          
            }
            else if(ATRAS && GAME_STATUS.menuAnterior == -1){
                (menu->audioCallback)(ERROR_MENU);
            }
        }
    }
    #ifdef RASPI
    animStatus = 0;
    pthread_join(displayMenuT, NULL);
    #endif

    #ifdef ALLEGRO
        toText = emptyText(toText);
        screenObjects = emptySprite(screenObjects);
        KEYS.press=0;
    #endif

    pthread_exit(0);
}


static void* saveScoreHandlerThread(void * data){
//Este thread es el encargado de manejar los menues.

	saveScore_t * menu = (saveScore_t *) data;

    int select = 0;//Esta variable se utiliza para indicar la letra seleccionada dentro del menu. 

    unsigned char stopSweep = 1;//Esta variable se utiliza para evitar que el usuario pueda cambiar de opcion muy rapido
    
    char letraActual[4] = {'A', 'A', 'A', 0}; //En este struct se almacena la letra que se esta mostrando actualmente en cada posicion.
    char letraAnterior;
    char titilar = 1; //Flag que indica si se debe titilar la letra.


    #ifdef ALLEGRO
    char letras[15][2] = {"Y","Z","A","B","C","Y","Z","A","B","C","Y","Z","A","B","C"}; //Array para almacenar las letras que se muestran en pantalla 
    #endif

    //*****************************************     Inicializa el thread que barre el display       *****************************
    #ifdef RASPI

        vector_t posLetra = {4,0};//Variable que indica la posicion de la esquina izquierda superior de la letra a mostrar en el display.
        dcoord_t posLetraDisplay;

        pthread_t displayMenuT, titileoT;
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

        argTextAnimMenu_t argTextAnimMenu = { menu -> puntaje,  &lowerDispMenu, &higherDispMenu, &halfDispAAA, IZQUIERDA, &animStatus, GAME_STATUS.menuActual};
    
        pthread_create(&displayMenuT, NULL, DISP_ANIM_MENU, &argTextAnimMenu);//Inicia el thread encargado de barrer el display

        usleep(1800 * U_SEC2M_SEC);

        letterFlash_t letterFlash = {&letraActual[select], &higherDispMenu, &posLetra, &titilar};
        pthread_create(&titileoT, NULL, letterFlashThread, &letterFlash);//Inicia el thread encargado de hacer titilar las letras.
    #endif

    #ifdef ALLEGRO
        allegroList = allegroScore(allegroList, menu -> puntaje , letras);

        sem_wait(&SEM_MENU);
        toText = allegroList->textoList;
        screenObjects = allegroList->spriteList;
        sem_post(&SEM_MENU);
    #endif

    //***************************************************************************************************************************

    usleep(200 * U_SEC2M_SEC);

    while(menu -> exitStatus){
        usleep(10 * U_SEC2M_SEC);
        if( (timerTick % velMenu) == 0 ){
            
            if(stopSweep){
                stopSweep -= 1;
            }

            if (SIGUIENTESCORE && !stopSweep){//Si se presiona para ir a la siguiente opcion
                #ifdef RASPI
                titilar = 0;//Dejamos de titilar la letra
                pthread_join(titileoT, NULL);
                posLetra.x += 4;
                #endif

                select += 1;

                if(select == (menu -> cantOpciones)){//Si llegamos a la ultima opcion pasamos a la primera
                    select = 0;
                    #ifdef RASPI
                    posLetra.x = 4;
                    #endif
                }

                #ifdef ALLEGRO
                    screenObjects = changeCol(screenObjects, select);
                #endif
                
                #ifdef RASPI
                letterFlash.letra = &letraActual[select];
                titilar = 1;//Comenzamos a titilar de vuelta.
                pthread_create(&titileoT, NULL, letterFlashThread, &letterFlash);//Inicia el thread encargado de hacer titilar las letras.
                #endif

                (*menu->audioCallback)(SWEEP_LETRA);
                stopSweep = 4;                
            }

            if (ANTERIORSCORE && !stopSweep){//Si se presiona para ir a la opcion anterior
                #ifdef RASPI
                titilar = 0;//Dejamos de titilar la letra    
                pthread_join(titileoT, NULL);            
                posLetra.x -= 4;
                #endif

                select -= 1;

                if(select < 0){//Si llegamos a la primer opcion pasamos a al ultima
                    select = (menu -> cantOpciones) - 1;
                    #ifdef RASPI
                    posLetra.x = 12;
                    #endif
                }

                #ifdef ALLEGRO
                    screenObjects = changeCol(screenObjects, select);
                #endif

                #ifdef RASPI
                letterFlash.letra = &letraActual[select];
                titilar = 1;//Comenzamos a titilar de vuelta.
                pthread_create(&titileoT, NULL, letterFlashThread, &letterFlash);//Inicia el thread encargado de hacer titilar las letras.  
                #endif

                (*menu->audioCallback)(SWEEP_LETRA); 

                stopSweep = 4;
            }

            if(ARRIBA_INPUT && !stopSweep){//Si se presiona para cambiar de letra hacia arriba
                #ifdef RASPI
                titilar = 0;//Dejamos de titilar la letra    
                pthread_join(titileoT, NULL);            
                #endif

                letraAnterior = letraActual[select];
                letraActual[select] += 1; //Apunta a la siguiente letra.

                #ifdef RASPI
                letterFlash.letra = &letraActual[select];
                #endif

                #ifdef ALLEGRO
                changeLetra(letras, select, 1);
                #endif

                switch (letraActual[select]){//Chequea que no se pase de los caracteres posibles.
                case 'Z' + 1:
                    letraActual[select] = ' ';
                    break;
                case ' ' + 1:
                    letraActual[select] = '0';
                    break;
                case '9' + 1:
                    letraActual[select] = 'A';
                default:
                    break;
                }

                #ifdef RASPI
                titilar = 0;//Dejamos de titilar la letra
                posLetraDisplay.x = posLetra.x;
                posLetraDisplay.y = posLetra.y;
                barridoLetra(letraAnterior, letraActual[select],1, posLetraDisplay); //Realiza un barrido para mostrar la nueva letra.
                titilar = 1;//Comenzamos a titilar de vuelta.
                pthread_create(&titileoT, NULL, letterFlashThread, &letterFlash);//Inicia el thread encargado de hacer titilar las letras.
                #endif

                (*menu->audioCallback)(SWEEP_LETRA);

                stopSweep = 4;
            }

            if(ABAJO_INPUT && !stopSweep){//Si se presiona para cambiar de letra hacia abajo
                #ifdef RASPI
                titilar = 0;//Dejamos de titilar la letra    
                pthread_join(titileoT, NULL);            
                #endif

                letraAnterior = letraActual[select];
                letraActual[select] -= 1; //Apunta a la siguiente letra.

                #ifdef RASPI
                letterFlash.letra = &letraActual[select];
                #endif

                #ifdef ALLEGRO
                changeLetra(letras, select, -1);
                #endif

                switch (letraActual[select]){//Chequea que no se pase de los caracteres posibles.
                case 'A' - 1:
                    letraActual[select] = '9';
                    break;
                case '0' - 1:
                    letraActual[select] = ' ';
                    break;
                case ' ' - 1: 
                    letraActual[select] = 'Z';
                default:
                    break;
                }

                #ifdef RASPI
                titilar = 0;//Dejamos de titilar la letra
                posLetraDisplay.x = posLetra.x;
                posLetraDisplay.y = posLetra.y;
                barridoLetra(letraAnterior, letraActual[select],-1, posLetraDisplay); //Realiza un barrido para mostrar la nueva letra.
                titilar = 1;//Comenzamos a titilar de vuelta.
                pthread_create(&titileoT, NULL, letterFlashThread, &letterFlash);//Inicia el thread encargado de hacer titilar las letras.
                #endif

                (*menu->audioCallback)(SWEEP_LETRA);
                
            }

            if (PRESS_INPUT){//Si se selecciona la opcion
                leaderboard_t leaderboard;
                parseScore(leaderboard);//Obtiene el valor actual del leaderboard
                addScore(leaderboard, menu -> puntajeNumerico, letraActual);//Modifica el leaderboard con el valor a guardar
                //Si el score no entra en el leaderboard entonces no modifica el leaderboard
                saveScore(leaderboard);

                GAME_STATUS.pantallaActual = MENU;
                GAME_STATUS.menuActual = MENU_INICIO;
                GAME_STATUS.menuAnterior = -1;
                menu -> exitStatus = 0;

                (*menu->audioCallback)(SAVED_SCORE);
                
                stopSweep = 4;
            }
        }
    }
    #ifdef RASPI
    animStatus = 0;
    titilar = 0;
    pthread_join(titileoT, NULL);
    pthread_join(displayMenuT, NULL);
    #endif

    #ifdef ALLEGRO
        toText = emptyText(toText);
        screenObjects = emptySprite(screenObjects);
        KEYS.press=0;
    #endif

    pthread_exit(0);
} 


static void* levelHandlerThread(void * data){

    unsigned char stopShoot = 1;//Esta variable se utiliza para evitar que el usuario pueda disparar mas de una bala a la vez

	game_t * menu = (game_t *) data;
    
    #ifdef ALLEGRO
    char vidas[2] = {(*(menu -> naveUsr)) -> lives + '0', 0};
    char score[7] = {'0', '0', '0', '0', '0', '0', 0}; 
    toText = levelAllegro(toText, score, vidas);
    #endif
    while(menu -> exitStatus){
        
        #ifdef ALLEGRO
            refreshDatos(score, vidas, *(menu -> scoreInstantaneo), (*(menu -> naveUsr)) -> lives );
        #endif
        if (PRESS_INPUT){//Si se presiono el joystick se debe pausar el juego.
                GAME_STATUS.menuActual = MENU_PAUSA;//Indica que se pauso el juego.
                GAME_STATUS.pantallaActual = MENU;

                menu -> exitStatus = 0;//Indica que hay que salir del level Handler
                #ifdef ALLEGRO
                KEYS.press = 0;
                #endif
        }
        usleep(10 * U_SEC2M_SEC);
        if( ((timerTick % velInputGameShoot) == 0) && menu -> exitStatus ){
            
            if(stopShoot){
                stopShoot -= 1;
            }
            if (ARRIBA_INPUT && !stopShoot){//Dispara una bala
                
                char eventoBala = shootBala(menu -> naveUsr, menu -> balasUsr, menu -> levelSettings);
                
                if(eventoBala == SL_BALA_USER){

                    (menu->audioCallback)(BALA_USER);
                }

                stopShoot = STOP_SHOOT;
            }
        }
        if( ((timerTick % velInputGameMoove) == 0) && menu -> exitStatus ){
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
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


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

    static int direccion = DERECHA; //Determina la direccion en la que se tienen que mover los aliens en el proximo tick

    char evento;
    while(GAME_STATUS.inGame){
        
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velAliens) == 0 && GAME_STATUS.inGame){
            sem_wait(&SEM_GAME);

            evento = moveAlien( ((argMoveAlien_t*)argMoveAlien) -> levelSettings,  (((argMoveAlien_t*)argMoveAlien) -> alienList), &direccion);
            switch (evento){
            case FASTER_ALIENS:
                velAliens -= VEL_INCR_ALIENS; //Incrementa la velocidad de los aliens.
                if(velAliens <= 0){
                    velAliens = 1;
                }
                (((argMoveAlien_t*)argMoveAlien)->audioCallback)(MOVIMIENTO_ALIENS);
                usleep((VEL_INCR_ALIENS + 1) * U_SEC2M_SEC);
                break;
            case SL_MOVIMIENTO_ALIENS:
                (((argMoveAlien_t*)argMoveAlien)->audioCallback)(MOVIMIENTO_ALIENS);

                break;
            default:
                break;
            }
            sem_post(&SEM_GAME);

        }   
    }

    pthread_exit(0);
}

void * moveMothershipThread(void* argMoveMothership){

    argMoveMothership_t * data = (argMoveMothership_t*)argMoveMothership;
    while(GAME_STATUS.inGame){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        object_t * mothership = *(data -> mothership);   
        printf("mothership %p     maxMshipXLevel %d\n", mothership, (data -> levelSettings) -> maxMShipXLevel);
        if( ((timerTick % ((data -> levelSettings) -> velMothership) == 0))){
            sem_wait(&SEM_GAME);
            if(*(data->mothership) != NULL){
                //Se incrementa/decrementa en una unidad de desplazamiento la posicion en x de la nave nodriza
                //Este evento sucede nada mas si la nave nodriza "esta viva", es decir si sus vidas son distintas de 0
                //El desplazamiento se da hasta que la nave nodriza haya llegado al otro lado de la pantalla
                objectType_t * motherAsset = getObjType((*(data->mothership))->type);
                mothership->pos.x += motherAsset->velocidad;
                if(((mothership->pos).x > ((data -> levelSettings) -> xMax )) || (mothership->pos.x < ((data -> levelSettings) -> xMin - motherAsset->ancho))){
                    mothership->lives = 0; // Si se va out of bounds mata a la nave
                    (*(data->mothership)) = destroyObj((*(data->mothership)), mothership);
                }
            }
            if((data -> levelSettings) -> maxMShipXLevel > 0){  //Si todavia hay naves nodrizas disponibles en el nivel
                if(mothershipCreator(data->mothership, data -> levelSettings) == MOTHERSHIP_CREATED){//Ejecuta la funcion que las intenta crear
                    (data->audioCallback)(MOTHERSHIP_APARECE);
                } 
            }
            sem_post(&SEM_GAME);
        }
    }

    pthread_exit(0);
}


//******************************************    Thread moveBala    **********************************************************
void * moveBalaThread(void * argMoveBala){
    //Este thread se encarga de accionar el disparo de los aliens y el movimiento de las balas del usuario y de los aliens.
    argMoveBala_t * data = (argMoveBala_t*)argMoveBala;

    char evento;

    while(GAME_STATUS.inGame){
        
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velBalas) == 0 && GAME_STATUS.inGame ){

            sem_wait(&SEM_GAME);

            if(*(data -> balasEnemigas) != NULL){

                (*(data -> balasEnemigas)) = moveBala(data -> balasEnemigas, data -> levelSettings);
            }

            if(*(data -> alienList) != NULL){

                evento = shootBala(data -> alienList, (data -> balasEnemigas), data -> levelSettings);
            }
            else{
                printf("Err in spaceInvaders.c, alienList cannot be null in moveBalaThread\n");
            }
            
            switch (evento){
            case SL_BALA_ALIEN:
                (data->audioCallback)(BALA_ALIEN);
                break;
            case SL_BALA_USER:
                (data->audioCallback)(BALA_USER);
                break;
            default:
                break;
            }

            if(*(data -> balasUsr) != NULL){

                (*(data -> balasUsr)) = moveBala(data -> balasUsr, data -> levelSettings);
            }
            sem_post(&SEM_GAME);
        } 
    }

    pthread_exit(0);
}
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/



/*******************************************************************************************************************************************
 * 
                                              ___         _   _   _      _             
                                             / __|  ___  | | | | (_)  __| |  ___   _ _ 
                                            | (__  / _ \ | | | | | | / _` | / -_) | '_|
                                             \___| \___/ |_| |_| |_| \__,_| \___| |_|                                                                                       
 * 
 ******************************************************************************************************************************************/

void * colliderThread(void * argCollider){
    //Este thread se utiliza para detectar si hubo colisiones.

    argCollider_t * data = (argCollider_t*)argCollider;
    
    char gameData = 0;
    char lost = 1;

    *(data->scoreInstantaneo) = *(data->score);    //Esta variable almacena el score del usuario constantemente y solo se almacena cuando se gana el nivel.

    while(GAME_STATUS.inGame){
        
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velCollider) == 0 && GAME_STATUS.inGame ){
            sem_wait(&SEM_GAME);
            if(*(data->alienList) != NULL && *(data->alienList) != NULL){   //Si hay aliens y usuario 
                gameData = collider(data->levelSettings, data->alienList, data->usrList, data->barriersList, data->balasEnemigas, data->balasUsr, data->motherShip, data->nivelActual, data->score, data->scoreInstantaneo);

                switch (gameData){//Detecta el evento
                    case LOST_LEVEL://Si se perdio el nivel
                        GAME_STATUS.pantallaActual = MENU;
                        GAME_STATUS.menuActual = MENU_LOST_LEVEL;
                        menuGame.exitStatus = 0;
                        *(data->score) = 0;//Se borra el score
                        objectType_t * assetUsuario = getObjType((*(data->usrList))->type);
                        GAME_STATUS.usrLives = assetUsuario->initLives;
                        (data->audioCallback)(COLISION_USER_MUERTO);
                        lost = 0;
                        GAME_STATUS.inGame = 0;
                        break;
                    case WON_LEVEL://Si se gano el nivel
                        GAME_STATUS.pantallaActual = MENU;
                        GAME_STATUS.menuActual = MENU_WON_LEVEL;
                        menuGame.exitStatus = 0;
                        (*(data->usrList))->lives += 1;
                        (data->audioCallback)(PARTIDA_GANADA);
                        GAME_STATUS.inGame = 0;
                        break;
                    case SL_COLISION_BALAS://Si hubo colision entre las balas
                        (data->audioCallback)(COLISION_CHOQUE_BALAS);
                        break;
                    case SL_COLISION_BARRERA_TOCADA://Si toco una barrera
                        (data->audioCallback)(COLISION_BARRERA_TOCADA);
                        break;
                    case SL_COLISION_BARRERA_MUERTA://Si mato una barrera
                        (data->audioCallback)(COLISION_BARRERA_MUERTA);
                        break;
                    case SL_COLISION_MOTHERSHIP_MUERTA://Si se mato la mothership
                        (data->audioCallback)(COLISION_MOTHERSHIP_MUERTA);
                        break;
                    case SL_COLISION_ALIEN_MUERTO://Si se mato a un alien
                        (data->audioCallback)(COLISION_ALIEN_MUERTO);
                        break;
                    case SL_COLISION_ALIEN_TOCADO://Si se golpeo a un alien
                        (data->audioCallback)(COLISION_ALIEN_TOCADO);
                        break;
                    case SL_COLISION_USER_TOCADO://Si el usuario recibio un disparo
                        (data->audioCallback)(COLISION_USER_TOCADO);
                        break;
                    default:
                        break;
                }
                sem_post(&SEM_GAME);
            }
        }
    }
    if(lost){
        GAME_STATUS.usrLives = (*(data->usrList))->lives;
    }
    GAME_STATUS.inGame = 0;

    pthread_exit(0);
}

 /*******************************************************************************************************************************************
*******************************************************************************************************************************************/