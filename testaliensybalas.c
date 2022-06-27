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
#include <unistd.h>
#include <stdarg.h>
#include "testaliensYBalas.h"
#include "utilidades.h"

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
                for(col = 0; col < ASCII2HEXA(string[letra]); col++){       //Recorre toda la fila
                    newList = addObj(newList, alienPos, tipoActual, vidaActual);
                    int dirRelleno = (col%2) ? -1 : 1; //Si el proximo alien es un un numero par (Segundo, cuarto...) lo pone en la izquierda (-1)
                    //Si en cambio el prox alien es impar lo pone en la derecha (+1)  
                    alienPos.x += (levelSetting -> saltoX)*dirRelleno*((col/2)+1) ;//Se desplaza en X. dirRelleno indica el lado para el que se va a mover y 
                                        //col/2 +1 es un multiplicador que realiza la cuenta de separacion de los aliens respecto al centro
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

int main(void) {

    level_setting_t settings;
    settings.yMax = 15;
    settings.yMin = 0;
    settings.xMax = 15;
    settings.xMin = 0;
    settings.saltoX = 4;
    settings.saltoY = 3;
    settings.initDanielLives = 1;
    settings.initNicolasLives = 2;
    settings.initPabloLives = 3;
    settings.distInicialX = 6;
    settings.distInicialY = 2;
    printf("pepe\n");
    object_t* listAlien = NULL;//Puntero al primer elemento de la lista de los aliens.
    listAlien = initAliens(listAlien, &settings, "20403", PABLO, DANIEL, NICOLAS);
    printf("george\n");
//************************************* Esta seccion del codigo se usa para probar que funcionen las listas *****************************
    int i = 1;
    object_t* prueba = listAlien;
    while(prueba != NULL){
        printf("Alien %d: x: %d ; y: %d ; tipo: %d ; vidas: %d\n", i, prueba -> pos.x, prueba -> pos.y, prueba -> type, prueba -> lives);
        i++;
        prueba = prueba -> next;
    }
}