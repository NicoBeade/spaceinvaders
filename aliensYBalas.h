/**********************************************************************************************************************************************************
 * 
                                            _   _                       __     __  ____            _                     _     
                                           | | (_)                      \ \   / / |  _ \          | |                   | |    
                                     __ _  | |  _    ___   _ __    ___   \ \_/ /  | |_) |   __ _  | |   __ _   ___      | |__  
                                    / _` | | | | |  / _ \ | '_ \  / __|   \   /   |  _ <   / _` | | |  / _` | / __|     | '_ \ 
                                   | (_| | | | | | |  __/ | | | | \__ \    | |    | |_) | | (_| | | | | (_| | \__ \  _  | | | |
                                    \__,_| |_| |_|  \___| |_| |_| |___/    |_|    |____/   \__,_| |_|  \__,_| |___/ (_) |_| |_|
                                                                                            
 * 
 ***********************************************************************************************************************************************************
*   Este archivo contiene las funciones y threads encargadas de todas las acciones relacionadas a las balas y los aliens.
*   Contiene los siguientes threads:
*       -moveAlien: se encarga de realizar el movimiento de los aliens. Para ello utiliza las siguientes funciones:
*           -detectarDireccion.
*               -tocaBorde.
*       -moveBala:
*
*   Ademas contiene las siguientes funciones:
*       -initAliens: crea la lista de los aliens. Esta funcion a su vez utiliza:
*           -addObj: agrega un nuevo alien a la lista.
*
 **********************************************************************************************************************************************************/

 #ifndef ALIENS_Y_BALAS_H
 #define ALIENS_Y_BALAS_H

#include "utilidades.h"
#include <pthread.h>
#include <semaphore.h>

/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/
typedef struct{//Este es el tipo de dato que recibe el thread de moveAlien
    object_t * alien;//Necesita un puntero al primer elemento de la lista de los aliens.
    int desplazamientoX;//Y cuanto se deben mover las naves en cada tick.
    int desplazamientoY;
    int xMax;//Coordenadas maximas del display.
    int margenX;//Margen maxiom al que pueden llegar las naves respecto del borde del display.
    int yMax;
    int margenY;
    int tamAlienX;//Hitbox del alien en la coordenada X.
}argMoveAlien_t;



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

//*************TIPOS DE OBJETOS

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                 ___               _           _     _                   
                                | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___
                                |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<
                                |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/
                                                                        |_|                                                            
 * 
 ******************************************************************************************************************************************/

//*****************ALIENS
object_t* addObj(object_t * firstObj, vector_t setPos, types_t setType, int setLives);   //Agrega un objeto a la lista
object_t * initAliens(object_t * listAliens, level_setting_t * levelSetting, char * str, ...); //Inicializa la lista completa de aliens usando addObj.
void removeAlienList(object_t* listAlien);                                               //Elimina de heap la lista creada.

void * moveAlien(void* alien);                                                                //Se encarga de modificar la posicion de los aliens.
int tocaBorde(object_t* alien, int xMax, int margenX, int yMax, int margenY, int tamAlienX);  //Detecta si algun alien esta tocando un borde

//*****************BALAS


//*****************USUARIO
void moveNaveUsuario(object_t * naveUsuario, int desplazamiento, int xMax, int tamAliensX); //Se encarga de actualizar la posicion de la nave del usuario
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

#define CHECK_HEXA(c)    ( ( ( (c) >= '0' && (c) <= '9' ) || ( (c) >= 'A' && (c) >= 'F' ) ) ? 1 : 0 )
//Esta macro devuelve un 1 si un char es un numero en hexa sino devuelve 0

#define ASCII2HEXA(c)    ( ( (c) >= '0' && (c) <= '9' ) ? (c) - '0' : (c) - 'A' + 0xA )   //Convierte un numero de ascii a hexadecimal.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                             ___         _                           
                                            | __| __ __ | |_   ___   _ _   _ _    ___
                                            | _|  \ \ / |  _| / -_) | '_| | ' \  (_-<
                                            |___| /_\_\  \__| \___| |_|   |_||_| /__/                                                                                                                      
 * 
 ******************************************************************************************************************************************/

extern unsigned int timerTick;   //Variable del timer utilizada para saber cuando se deben ejecutar los threads.
extern int velAliens;   /*Determina que tan rapido se moveran los aliens. La conversion es: si velAliens = 1, entonces moveAlien se ejecuta cada 10mS
                                                                        Para ejecutar velAliens cada 1s velAliens debe valer 100.*/
extern sem_t semaforo;
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


 #endif//aliensYBalas.h
object_t * destroyObj(object_t * ListObj, object_t * RipObj);
object_t * moveBala(object_t * ListBalasEnemy, int BalaType, int yMax, int yMin, int velocity);
