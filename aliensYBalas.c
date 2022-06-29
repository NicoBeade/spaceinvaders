/**********************************************************************************************************************************************************
 * 
                                         _   _                       __     __  ____            _                          
                                        | | (_)                      \ \   / / |  _ \          | |                         
                                  __ _  | |  _    ___   _ __    ___   \ \_/ /  | |_) |   __ _  | |   __ _   ___        ___ 
                                 / _` | | | | |  / _ \ | '_ \  / __|   \   /   |  _ <   / _` | | |  / _` | / __|      / __|
                                | (_| | | | | | |  __/ | | | | \__ \    | |    | |_) | | (_| | | | | (_| | \__ \  _  | (__ 
                                 \__,_| |_| |_|  \___| |_| |_| |___/    |_|    |____/   \__,_| |_|  \__,_| |___/ (_)  \___|
                                                                                            
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
*           -addAlien: agrega un nuevo alien a la lista.
*
 **********************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdarg.h>
#include "aliensYBalas.h"


/*******************************************************************************************************************************************
 * 
                                 ___               _           _     _                   
                                | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___
                                |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<
                                |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/
                                                                        |_|                                                            
 * 
 ******************************************************************************************************************************************/

static int detectarDireccion (int direccion, object_t* alien, int xMax, int margenX, int yMax, int margenY, int tamAlienX);    //Detecta en que direccion se debe mover a los aliens.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/



/*******************************************************************************************************************************************
 * 
             ___                      _                                _            ___    _        _         _              
            | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___     / _ \  | |__    (_)  ___  | |_   ___   ___
            | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_)   | (_) | | '_ \   | | / -_) |  _| / _ \ (_-<
            |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___|    \___/  |_.__/  _/ | \___|  \__| \___/ /__/
                                                                                                |__/ 
 * 
 ******************************************************************************************************************************************/

object_t* addObj(object_t * firstObj, vector_t setPos, types_t setType, int setLives){
/* Esta funcion se encarga de agregar un nuevo alien a la lista, inicializando su posicion, tipo y cantidad de vidas.
    Devuelve un puntero al primer elemento de la lista.
*/	
	object_t * newObj = malloc(sizeof(object_t));//Agrega el nuevo alien

	if(newObj == NULL){//Si no se puede hacer el malloc indica error.
		printf( "No se ha podido agregar el elemento a la lista.\n");
		return NULL; //error
	}

    if(firstObj != NULL){//Si no es el primero de la lista debe avanzar hasta el ultimo elemento.
        object_t * lastObj = firstObj;//Se almacena el puntero al primer elemento.

		while(lastObj -> next != NULL){
			lastObj = lastObj -> next;
		}
        lastObj -> next = newObj;
	}
    else{//Si es el primero de la lista debemos devolver ese puntero.
        firstObj = newObj;
    }

	newObj -> pos = setPos;//Asigna los valores indicados en los distitntos campos del alien.
	newObj -> type = setType;
	newObj -> lives = setLives;
    newObj -> next = NULL;

	return firstObj;//Devuelve un puntero al primer elemento.
}
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/



/*******************************************************************************************************************************************
 * 
                 ___                      _                                _             _     _   _                   
                | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___      /_\   | | (_)  ___   _ _    ___
                | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_)    / _ \  | | | | / -_) | ' \  (_-<
                |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___|   /_/ \_\ |_| |_| \___| |_||_| /__/ 

 * 
 ******************************************************************************************************************************************/

object_t * initAliens(object_t * listAliens, level_setting_t * levelSetting, char * str, ...){      
/*Utiliza la funcion addAlien para crear la lista con todos los aliesn al empezar un nivel. Devuelve un puntero al primer elemento de la lista.
    Recibe como parametros:
        -listAliens: puntero al primer elemento de la lista.
        -xMax: tamano en x del display.
        -yMax: tamano en y del display.
        -distInicialX: distancia minima a la que se puede encontrar el primer alien respecto al borde izquierdo.
        -distInicialX: distancia minima a la que se puede encontrar el primer alien respecto al borde superior.
        -saltoX: distancia en x entre la coordenada 0 en x de cada columna de aliens (TAM_ALIENS_X + ESP_ALIENS_X).
        -saltoY: distancia en y entre la coordenada 0 en y de cada fila de aliens (TAM_ALIENS_Y + ESP_ALIENS_Y).
        -str: puntero a un string que contiene la cantidad de aliens que debe haber en cada fila.
        -Argumentos variables: indican en orden el tipo de alien que tendra cada fila.
*/
    va_list tipos;             //Puntero a la lista de argumentos variables
    va_start(tipos, str);      //Inicializa los argumentos variables
    char * string = str;        //String auxiliar para recorrer la entrada
    int letra;                  //Indice letra
    int err = 0;                //Variable error (si es distinta de 0 hubo error)
    for(letra = 0; string[letra] != '\0'; letra++){     //Se recorre el string para revisar que la entrada sea correcta
        err = !CHECK_HEXA(string[letra]);               //Si la entrada no es un numero Hexa hubo error
    }
    if((letra-1) > (levelSetting -> yMax)/(levelSetting -> saltoY)){                        //Si hubo mas letras que filas tambien hubo error
        err = 1;
    }                         
    
    if(err == 0){                   //Si no hubo error
        vector_t alienPos = {(levelSetting -> distInicialX), (levelSetting -> distInicialY)};
        object_t * newList = listAliens; 
        for(letra = 0; string[letra] != '\0'; letra++){     //Se recorre el string
            if(string[letra] != '0'){                       //Si no es 0 entonces rellena esa cantidad de aliens
                int col;                //Contador de columnas (aliens rellenados en esa fila)
                int tipoActual = (int) va_arg(tipos, int);              //Toma el tipo ingresado para la fila correspondiente
                    int vidaActual;                                         //Crea una variable que indica las vidas de cada tipo
                    switch(tipoActual){                                     //Selecciona las vidas con las que inicializa esa fila
                        case DANIEL:
                            vidaActual = levelSetting -> initDanielLives;   //Dependiendo del tipo de alien se busca la vida correspondiente
                            break;
                        case PABLO:
                            vidaActual = levelSetting -> initDanielLives;
                            break;
                        case NICOLAS:
                            vidaActual = levelSetting -> initDanielLives;
                            break;
                        default:                                            //Si el tipo de nave no es valido se genera un error
                            return NULL;
                    }
                int dirRelleno; //Si el proximo alien es un un numero par (Segundo, cuarto...) lo pone en la izquierda (-1)
                //Si en cambio el prox alien es impar lo pone en la derecha (+1)  
                for(col = 0; col < ASCII2HEXA(string[letra]); col++){       //Recorre toda la fila
                    newList = addObj(newList, alienPos, tipoActual, vidaActual);
                    dirRelleno = (col%2) ? 1 : -1;
                    alienPos.x += (levelSetting -> saltoX)*dirRelleno*(col+1) ;//Se desplaza en X. dirRelleno indica el lado para el que se va a mover y 
                                                        //col +1 es un multiplicador que realiza la cuenta de separacion de los aliens respecto al centro
                }
                alienPos.x = levelSetting -> distInicialX;//Reinicia el desplazamiento en X
            }
            alienPos.y += levelSetting -> saltoY;       //Se desplaza por fila
        }
    va_end(tipos);              //Se finalizan los argumentos variables
    return newList;
    }
    else{
        return NULL;
    }
}     


void removeAlienList(object_t* listAlien){
/*Esta funcion se encarga de liberar del heap la lista creada de los aliens*/
    if(listAlien != NULL){
        object_t * lastAlien = listAlien; //Se crean dos punteros auxiliares
        object_t * nextAlien;
        do {
            nextAlien = lastAlien -> next; //Se apunta al siguiente nodo
            free(lastAlien); //Se libera la memoria dinamica del nodo a eliminar
            lastAlien = nextAlien;
        } while (nextAlien != NULL);
    }
}

//******************************************    Thread moveAlien    ***********************************************************

void * moveAlien(void* argMoveAlien){
/* Este thread se encarga de mover la posicion de los aliens teniendo en cuenta para ello la variable direccion.
    Recibe como parametro el puntero al primer alien de la lista.
*/
    int static direccion = DERECHA; //Determina la direccion en la que se tienen que mover los aliens en el proximo tick
    int vx, vy;//Variables temporales utilizadas para incrementar o decrementar las componentes x e y del vector coordenadas.

    while(1){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velAliens) == 0 ){
            printf("Movealiens\n");

            sem_wait(&semaforo2);
            sem_trywait(&semaforo1);
            printf("timerTick = %d", timerTick);

            direccion = detectarDireccion(direccion, ((argMoveAlien_t*)argMoveAlien) -> alien, ((argMoveAlien_t*)argMoveAlien) -> xMax,
                                          ((argMoveAlien_t*)argMoveAlien) -> margenX, ((argMoveAlien_t*)argMoveAlien) -> yMax,
                                          ((argMoveAlien_t*)argMoveAlien) -> margenY, ((argMoveAlien_t*)argMoveAlien) -> tamAlienX); 
                                          //Modifica la variable de direccion en funcion al estado actual de la direccion

            switch (direccion){//Primero detecta en que sentido debemos mover las naves.
                case IZQUIERDA:
                    vx = -( ((argMoveAlien_t*)argMoveAlien) -> desplazamientoX );
                    vy = 0;
                    break;
                case DERECHA:
                    vx = ( ((argMoveAlien_t*)argMoveAlien) -> desplazamientoX );
                    vy = 0;
                    break;
                case ABAJO:
                    vx = 0;
                    vy = ( ((argMoveAlien_t*)argMoveAlien) -> desplazamientoY );
                    break;
                default:
                    break;
            }
            object_t* auxiliar = ((argMoveAlien_t*)argMoveAlien) -> alien;
            while (auxiliar != NULL){//Mueve los aliens uno por uno

                auxiliar->pos.x += vx;//Modifica su posicion en x e y
                auxiliar->pos.y += vy;
                auxiliar->animationStatus++;
                auxiliar = auxiliar -> next;
            }
        sem_post(&semaforo1);
        sem_post(&semaforo2);
        }
    }
    pthread_exit(0);
}
//*************************************************************************************************************************************

static int detectarDireccion (int direccion, object_t* alien, int xMax, int margenX, int yMax, int margenY, int tamAlienX){
/* Esta funcion se encarga de modificar la variable direccion. Es llamada solo por la funcion moveAlien.
    Recibe como parametro la variable direccion y detecta si alguno de los aliens se encuentra en un borde del mapa y en base a eso modificar
    esta variable. Ademas, si toca el borde inferior, pone la variable vidas en 0, pues si los aliens llegan a la parte inferior el usuario perdera.
    Se va a implementar una maquina de estados que en base al estado actual de la direccion y del borde tocado, deduce la direccion resultante.
    Los estados son las direcciones y ya estan definidos en un enum previo. Recibe como parametros:
        -direccion: variable donde se almacena la direccion en la que se deben mover los aliens
        -alien: puntero al primer elemento de la lista de los aliens.
        -xMax: coordenada maxima de x del mapa
        -margenX: distancia maxima a la que pueden llegar los aliens respecto de los bordes laterales.
        -yMax: coordenada maxima de y del mapa
        -margenY: distancia maxima a la que pueden llegar los aliens respecto de los bordes verticales.
*/
    switch(direccion){
        
        case DERECHA: //Si se viene moviendo para la derecha
            if (tocaBorde(alien, xMax, margenX, yMax, margenY, tamAlienX) == DERECHA){ //y toca el borde derecho
                return ABAJO; //se mueve hacia abajo
            }
            else {
                return DERECHA; //si no, sigue moviendose para la derecha
            }
            break;

        case IZQUIERDA: //Si se viene moviendo para la izquierda
            if (tocaBorde(alien, xMax, margenX, yMax, margenY, tamAlienX) == IZQUIERDA){ //y toca el borde izquierdo
                return ABAJO; //se mueve hacia abajo
            }
            else {
                return IZQUIERDA; //si no, sigue moviendose para la izquierda
            }
            break;

        case ABAJO: //Si se viene moviendo para abajo
            if (tocaBorde(alien, xMax, margenX, yMax, margenY, tamAlienX) == ABAJO){ //Si algun alien toca el suelo, el jugador pierde la partida
                alien -> lives = 0;
            }
            if (tocaBorde(alien, xMax, margenX, yMax, margenY, tamAlienX) == DERECHA){ //Si esta tocando el borde derecho, se mueve hacia la izquierda
                return IZQUIERDA;
            }
            else {
                return DERECHA; //si no, esta tocando el borde izquierdo, por lo que se mueve hacia la derecha
            }
            break;

        default: //default, nunca deberia llegar a este punto
            break;
    } 
    return 0;
}

int tocaBorde(object_t* alien, int xMax, int margenX, int yMax, int margenY, int tamAlienX){
/* Esta funcion detecta si alguna de las naves toco algun borde, teniendo en cuenta todas las posibles combinaciones.
    Devuelve que borde fue tocado. Recibe como parametros:
        -alien: puntero al primer elemento de la lista de los aliens.
        -xMax: coordenada maxima de x del mapa
        -margenX: distancia maxima a la que pueden llegar los aliens respecto de los bordes laterales.
        -yMax: coordenada maxima de y del mapa
        -margenY: distancia maxima a la que pueden llegar los aliens respecto de los bordes verticales.
*/
    int borde = 0;
    while ((alien->next != NULL) && (borde != ABAJO)){ //mientras no se haya llegado al final de la lista o no se haya detectado suelo
        if (alien->pos.x <= 0 + margenX){ //deteccion borde izquierdo
            borde = IZQUIERDA;
        }
        else if (alien->pos.x >= xMax - margenX - tamAlienX){ //deteccion borde derecho
            printf("Llego al borde");
            borde = DERECHA;
        }
        if (alien->pos.y >= yMax - margenY){ //deteccion de suelo
        borde = ABAJO;
        }
        alien = alien -> next;
    }
    return borde;
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                     ___                      _                                _           ___          _        
                    | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___    | _ )  __ _  | |  __ _ 
                    | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_)   | _ \ / _` | | | / _` |
                    |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___|   |___/ \__,_| |_| \__,_|
                                                                                              
 * 
 ******************************************************************************************************************************************/


 
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/
object_t * moveBala(object_t * ListBalasEnemy, int BalaType, int yMax, int yMin, int velocity){      //Mueve las balas de un tipo especifico.
    object_t * Bala = ListBalasEnemy;
    object_t * newList = ListBalasEnemy;
    if(Bala != NULL){
        do{
            if(Bala -> type == BalaType){
                if(Bala -> pos.y < yMax && Bala -> pos.y > yMin){    //Si la bala se encuentra en el interior del display
                    Bala -> pos.y += velocity;
                }
                else{                               //Si la bala se encuentra fuera (o en la frontera)
                    newList = destroyObj(ListBalasEnemy, Bala);     //Se destruye la bala
                }                                                                                           
            }
            Bala = Bala -> next;
        }while(Bala != NULL);
    }
    return newList;
}


object_t * destroyObj(object_t * ListObj, object_t * RipObj){
    object_t * Obj = ListObj;
    if(Obj != NULL && RipObj != NULL){        //Si la lista y el objeto existe
        if(Obj != RipObj){                    //Si el objeto no es el primero de la lista
            object_t * PreObj = Obj;            //Objeto anterior al que se va a eliminar
            while(PreObj -> next != RipObj){     //Recorre la lista hasta encontrar el objeto anterior al que se quiere destruir
                PreObj = PreObj -> next;
            }
            PreObj -> next = RipObj -> next;      //Se asigna el siguiente objeto
        }
        else{                                       //Si el objeto a eliminar es el primero
            Obj = RipObj -> next;                 //Devuelve puntero al segundo objeto si es que existe
        }
        free(RipObj);                          //Se libera la memoria del objeto eliminado
    }
    return Obj;             //Se devuelve la lista
}
