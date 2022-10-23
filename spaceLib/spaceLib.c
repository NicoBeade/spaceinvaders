/**********************************************************************************************************************************************************
 * 
                                                                             _        _   _                
                                                                            | |      (_) | |               
                                         ___   _ __     __ _    ___    ___  | |       _  | |__         ___ 
                                        / __| | '_ \   / _` |  / __|  / _ \ | |      | | | '_ \       / __|
                                        \__ \ | |_) | | (_| | | (__  |  __/ | |____  | | | |_) |  _  | (__ 
                                        |___/ | .__/   \__,_|  \___|  \___| |______| |_| |_.__/  (_)  \___|
                                            | |                                                          
                                            |_|                                                                          
 * 
 ***********************************************************************************************************************************************************
*   Este archivo contiene las funciones y threads encargados del backEnd del juego. Tiene las siguientes funcionalidades:
*       -Funciones para crear las listas de los aliens y las balas.
*       -Funciones que se encargan de mover los aliens durante la ejecucion del juego.
*       -Funciones que se encargan de que los aliens disparen.
*       -Funciones que se utilzan para mover al usuario y evitar que se salga de la pantalla.
*       -Funcion que detecta si una bala golpeo algo.
*
 **********************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include "spaceLib.h"
#include <string.h>


/*******************************************************************************************************************************************
 * 
                                 ___               _           _     _                   
                                | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___
                                |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<
                                |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/
                                                                        |_|                                                            
 * 
 ******************************************************************************************************************************************/

static int detectarDireccion (int direccion, level_setting_t * levelSettings, object_t * listAliens);    //Detecta en que direccion se debe mover a los aliens.
static int tocaBorde(level_setting_t * argMoveAlien, object_t * listAliens);  //Detecta si algun alien esta tocando un borde
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/
static unsigned int countList(object_t * lista);


/*******************************************************************************************************************************************
 * 
             ___                      _                                _            ___    _        _         _              
            | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___     / _ \  | |__    (_)  ___  | |_   ___   ___
            | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_)   | (_) | | '_ \   | | / -_) |  _| / _ \ (_-<
            |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___|    \___/  |_.__/  _/ | \___|  \__| \___/ /__/
                                                                                                |__/ 
 * 
 ******************************************************************************************************************************************/

object_t* addObj(object_t * firstObj, vector_t setPos, int setType, int setLives){
/* Esta funcion se encarga de agregar un nuevo alien a la lista, inicializando su posicion, tipo y cantidad de vidas.
    Devuelve un puntero al primer elemento de la lista.
*/	
	object_t * newObj = malloc(sizeof(object_t));//Agrega el nuevo alien

	if(newObj == NULL){//Si no se puede hacer el malloc indica error.
		printf("Err in gameLib, addObj function: couldnt add node to the list\n");
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
    newObj -> animationStatus = 0;
    newObj -> next = NULL;

	return firstObj;//Devuelve un puntero al primer elemento.
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
    else{
        printf("Err in gameLib, destroyObj function: The list %p and the node %p to delete cannot be empty\n", ListObj, RipObj); //Si no existen devuelve error
    }
    return Obj;             //Se devuelve la lista
}

object_t* removeList(object_t* lista){
//Esta funcion se encarga de liberar del heap una lista completa.
    object_t * nextObj;
    while (lista != NULL){
        nextObj = lista -> next; //Se apunta al siguiente nodo
        free(lista); //Se libera la memoria dinamica del nodo a eliminar
        lista = nextObj;
    }

    return lista;
}

static unsigned int countList(object_t * lista){  //Cuenta la cantidad de nodos de una lista de obj
    unsigned int nodosCant = 0;                 //Se inicializa la variable cantidad de nodos
    while(lista != NULL){                  //Si el nodo no esta vacio (no es el ultimo)
        nodosCant++;                            //Se cuenta el nodo
        lista = lista -> next;     //Se apunta al siguiente nodo
    }
    return nodosCant;                           //Se devuelve la cantidad de nodos
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
        printf("Err in gameLib, initAliens function: There are more characters than rows avaiable in the display\n");
    }                         
    
    if(err == 0){                   //Si no hubo error
        vector_t alienPos = {(levelSetting -> distInicialX), (levelSetting -> distInicialY)};
        object_t * newList = listAliens; 
        for(letra = 0; string[letra] != '\0'; letra++){     //Se recorre el string
            if(string[letra] != '0'){                       //Si no es 0 entonces rellena esa cantidad de aliens
                int col;                //Contador de columnas (aliens rellenados en esa fila)
                int tipoActual = (int) va_arg(tipos, int);              //Toma el tipo ingresado para la fila correspondiente
                objectType_t * ObjectTypeActual = getObjType(tipoActual);     //Crea un puntero al tipo de objeto de esa fila
                int vidaActual =  ObjectTypeActual -> initLives;                           //Crea una variable que indica las vidas de cada tipo
                int dirRelleno; //Si el proximo alien es un un numero par (Segundo, cuarto...) lo pone en la izquierda (-1)
                //Si en cambio el prox alien es impar lo pone en la derecha (+1)  
                for(col = 0; col < ASCII2HEXA(string[letra]); col++){       //Recorre toda la fila
                    newList = addObj(newList, alienPos, tipoActual, vidaActual);
                    dirRelleno = (col%2) ? 1 : -1;      //Se rellena a izquierda/derecha/izquierda/derecha
                    alienPos.x += (levelSetting -> saltoX)*dirRelleno*(col+1) ;//Se desplaza en X. dirRelleno indica el lado para el que se va a mover y 
                                                        //col +1 es un multiplicador que realiza la cuenta de separacion de los aliens respecto al centro
                }
                alienPos.x = levelSetting -> distInicialX;//Reinicia el desplazamiento en X
            }
            alienPos.y += levelSetting -> saltoY;       //Se desplaza por fila
        }
    va_end(tipos);              //Se finalizan los argumentos variables
    return newList;     //Si no hubo error devuelve la lista de aliens
    }
    else{               //Si hubo error devuelve NULL
        return NULL;
    }
}     


void moveAlien(level_setting_t*  levelSettings, object_t ** alienList, int* direccion){
    if(*alienList == NULL){
        printf("Err in gameLib, moveAlien function: AlienList cannot be NULL in function 'moveAlien'\n");
    }
    object_t * auxiliar;
    *direccion = detectarDireccion(*direccion, levelSettings, *alienList);  //Modifica la variable de direccion en funcion al estado actual de la direccion
    int vx, vy;//Variables temporales utilizadas para incrementar o decrementar las componentes x e y del vector coordenadas.

    switch (*direccion){//Primero detecta en que sentido debemos mover las naves.
        case IZQUIERDA:
            vx = - (levelSettings -> desplazamientoX);
            vy = 0;
            break;
        case DERECHA:
            vx = levelSettings -> desplazamientoX;
            vy = 0;
            break;
        case ABAJO:
            vx = 0;
            vy = levelSettings -> desplazamientoY;
            break;
        default:
            break;
    }
    auxiliar = *alienList;
    while (auxiliar != NULL){//Mueve los aliens uno por uno

        auxiliar->pos.x += vx;//Modifica su posicion en x e y
        auxiliar->pos.y += vy;
        auxiliar->animationStatus++;
        auxiliar = auxiliar -> next;
    }
}


//*************************************************************************************************************************************

static int detectarDireccion (int direccion, level_setting_t * levelSettings, object_t * listAliens){
/* Esta funcion se encarga de modificar la variable direccion. Es llamada solo por la funcion moveAlien.
    Recibe como parametro la variable direccion y detecta si alguno de los aliens se encuentra en un borde del mapa y en base a eso modificar
    esta variable. Ademas, si toca el borde inferior, pone la variable vidas en 0, pues si los aliens llegan a la parte inferior el usuario perdera.
    Se va a implementar una maquina de estados que en base al estado actual de la direccion y del borde tocado, deduce la direccion resultante.
    Los estados son las direcciones y ya estan definidos en un enum previo.
*/
    switch(direccion){
        
        case DERECHA: //Si se viene moviendo para la derecha
            if (tocaBorde(levelSettings, listAliens) == DERECHA){ //y toca el borde derecho
                return ABAJO; //se mueve hacia abajo
            }
            else {
                return DERECHA; //si no, sigue moviendose para la derecha
            }
            break;

        case IZQUIERDA: //Si se viene moviendo para la izquierda
            if (tocaBorde(levelSettings, listAliens) == IZQUIERDA){ //y toca el borde izquierdo
                return ABAJO; //se mueve hacia abajo
            }
            else {
                return IZQUIERDA; //si no, sigue moviendose para la izquierda
            }
            break;

        case ABAJO: //Si se viene moviendo para abajo
            if (tocaBorde(levelSettings, listAliens) == ABAJO){ //Si algun alien toca el suelo, esta funcion no hace nada al respecto
            }
            if (tocaBorde(levelSettings, listAliens) == DERECHA){ //Si esta tocando el borde derecho, se mueve hacia la izquierda
                return IZQUIERDA;
            }
            else {
                return DERECHA; //si no, esta tocando el borde izquierdo, por lo que se mueve hacia la derecha
            }
            break;

        default: //default, nunca deberia llegar a este punto
            printf("Err in gameLib, detectarDireccion function: invalid movement case reached\n");
            break;
    } 
    return 0;
}

static int tocaBorde(level_setting_t * levelSettings, object_t * alien){
/* Esta funcion detecta si alguna de las naves toco algun borde, teniendo en cuenta todas las posibles combinaciones.
    Devuelve que borde fue tocado.
*/
    if(alien == NULL){
        printf("Err in gameLib, tocaBorde function: alien cannot be a null pointer\n");
        return DERECHA+ABAJO+IZQUIERDA;
    }
    int borde = 0;
    while ((alien != NULL) && (borde != ABAJO)){ //mientras no se haya llegado al final de la lista o no se haya detectado suelo
        objectType_t * tipoAlien = getObjType(alien->type);
        if (alien->pos.x <= 0 + levelSettings->margenX){ //deteccion borde izquierdo
            borde = IZQUIERDA;
        }
        else if (alien->pos.x > levelSettings->xMax - levelSettings->margenX - tipoAlien->ancho + 1){ //deteccion borde derecho
            borde = DERECHA;
        }
        if (alien->pos.y >= levelSettings->yMax - levelSettings->margenY){ //deteccion de suelo
            borde = ABAJO;
        }
        alien = alien -> next;
    }
    return borde;
}

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

object_t * moveBala(object_t ** ListBalasEnemy, level_setting_t * levelSetting){      //Mueve las balas de un tipo especifico.
    object_t * Bala = *ListBalasEnemy;
    object_t * newList = *ListBalasEnemy;
    int yMax = levelSetting -> yMax;
    int yMin = levelSetting -> yMin;
    if(Bala != NULL){
        while(Bala != NULL){
            if(Bala -> pos.y < yMax && Bala -> pos.y > yMin){    //Si la bala se encuentra en el interior del display
                objectType_t * balaType = getObjType(Bala -> type);
                (Bala -> pos.y) += (balaType -> velocidad);
                Bala = Bala -> next;
            }
            else{                               //Si la bala se encuentra fuera (o en la frontera)
                object_t* balaADestruir = Bala;
                Bala = Bala -> next;
                newList = destroyObj(*ListBalasEnemy, balaADestruir);     //Se destruye la bala
                *ListBalasEnemy = newList;
            }                                                                                          
            
        }
    }
    else{
        printf("Err in gameLib, moveBala function: listBalas cannot be empty (null)\n");
    }
    return newList;
}

object_t * shootBala(object_t * listaNaves, object_t * listaBalas, level_setting_t * levelSetting){
    int balasActuales = countList(listaBalas);                  //Se cuenta la cantidad de balas activas
    object_t * nave = listaNaves;                               //Se crea un puntero a la lista de naves
    object_t * bala = listaBalas;                               //Se crea un puntero a la lista de balas
    int probabilidad;                                           //Probabilidad de disparo de la nave
    objectType_t * balaType;                                    //Puntero al tipo de bala
    objectType_t * naveType = getObjType(nave->type);           //Puntero al tipo de nave
    int balasDisponibles;                                       //Balas disponibles para disparar
    if(nave == NULL){
        printf("Err in gameLib, shootBala function: listaNaves cannot be empty (null)\n");
        return NULL;
    }
    balasDisponibles = naveType -> maxBullets - balasActuales;   //La cantidad de balas disponibles es la resta entre las maximas y las actuales. Se toma la primera nave como ref
    while(balasDisponibles > 0 && nave != NULL){
        naveType = getObjType(nave -> type);
        if (naveType == NULL){
            printf("Err in gameLib, shootBala function: naveType not found\n");
            return NULL;
        }
        probabilidad = naveType -> shootProb;
        int balaTypeID = naveType -> balaID;
        balaType = getObjType(balaTypeID);
        int vidaBala = balaType -> initLives;
        if((rand()%100) < probabilidad){
            vector_t posicionBala;
            posicionBala.x = nave->pos.x + (naveType -> ancho)/2;
            posicionBala.y = nave->pos.y; 
            printf("nave->pos.y: %d \n",nave->pos.y);
            bala = addObj(bala, posicionBala, balaTypeID, vidaBala);
            balasDisponibles--;
        }
        nave = nave -> next;
    }
    return bala;
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
             ___                      _                                _           _   _                            _       
            | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___    | | | |  ___  _  _   __ _   _ _  (_)  ___ 
            | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_)   | |_| | (_-< | || | / _` | | '_| | | / _ \
            |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___|    \___/  /__/  \_,_| \__,_| |_|   |_| \___/                                                                                        
 * 
 ******************************************************************************************************************************************/

void moveNaveUsuario(object_t * naveUsuario, level_setting_t* levelSettings, int direccion){
/* Esta funcion se llama como callback por los threads que manejan el input tanto en allegro como en la raspberry. Se encarga de actualizar
    la posicion de la nave del usuario.
*/
    if( direccion == DERECHA && (naveUsuario -> pos.x <= levelSettings->xMax - levelSettings->anchoUsr) ){//Si se tiene que mover para la derecha y no llego al limite
        naveUsuario -> pos.x += levelSettings->desplazamientoUsr;//Desplaza la nave
    }
    else if( direccion == IZQUIERDA && (naveUsuario -> pos.x != levelSettings->xMin) ){//Si se tiene que mover para la izquierda y no llego al limite
        naveUsuario -> pos.x -= levelSettings->desplazamientoUsr;//Desplaza la nave
    }
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
void collider(level_setting_t * levelSettings, object_t ** alienList, object_t ** usrList, object_t ** barrerasList, object_t ** balasEnemigas, object_t ** balasUsr){
//Esta funcion se encarga de detectar si una bala impacta contra algo.

    char collition = 1;//Flag para detectar colisiones. El 1 significa que no hubo colision

    //Primero se crea una copia de los punteros al primer elemento de cada lista para facilitar los llamados.
    object_t * listAliens = *alienList;
    if(listAliens == NULL){
        printf("Err in spaceLib.c alienList cannot be NULL in collider.\n");
    }
    object_t * listUsr = *usrList;
    if(listUsr == NULL){
        printf("Err in spaceLib.c usrList cannot be NULL in collider.\n");
        return;
    }
    object_t * listBarreras = *barrerasList;

    object_t * listBalasEnemigas = *balasEnemigas;

    object_t * listBalasUsr = *balasUsr;

    object_t * balaADestruir;

    while(listBalasEnemigas != NULL  &&  listUsr->lives != 0){//Primero chequea si las balas enemigas golpearon algo.

        while(listBarreras != NULL  &&  collition && listBalasEnemigas != NULL && *balasEnemigas != NULL ){//Chequea todas las barreras
            if(collision(listBalasEnemigas->pos, listBalasEnemigas->type, listBarreras->pos, listBarreras->type)){//Si golpeo a una barrera

                collition = 0;
                listBarreras->lives -= 1;
                if(listBarreras->lives == 0){//Si se mato a esa barrera hay que eliminarla de la lista

                    *barrerasList = destroyObj(*barrerasList, listBarreras);
                    listBarreras = *barrerasList;
                }
                listBalasEnemigas->lives -= 1;
                if(listBalasEnemigas->lives == 0){//Si la bala debe morir
                    printLista(*balasEnemigas, "Balas George");
                    balaADestruir = listBalasEnemigas;
                    listBalasEnemigas = listBalasEnemigas->next;//Apunta a la siguiente bala

                    *balasEnemigas = destroyObj(*balasEnemigas, balaADestruir);

                    printLista(*balasEnemigas, "Balas Jeremy");
                }
                else{//Si la bala no debe morir
                listBalasEnemigas = listBalasEnemigas->next;//Apunta a la siguiente bala
                }
            }
            else{//Si no golpeo a esa barrera chequea el siguiente
                listBarreras = listBarreras->next;
            }
        }
        if(collision(listBalasEnemigas->pos, listBalasEnemigas->type, listUsr->pos, listUsr->type) && collition){

            collition = 0;
            listUsr->lives -= 1;//Si una bala golpeo al usuario se le quita una vida.
            if(listUsr->lives == 0){//Si el usuario muere termina el nivel.
                GAME_STATUS.pantallaActual = MENU;
                GAME_STATUS.menuActual = MENU_LOST_LEVEL;
                menuGame.exitStatus = 0;
            }
            listBalasEnemigas->lives -= 1;
            if(listBalasEnemigas-> lives == 0){//Si la bala debe morir
                object_t * balaADestruir = listBalasEnemigas;
                listBalasEnemigas = listBalasEnemigas->next;//Apunta a la siguiente bala
                *balasEnemigas = destroyObj(*balasEnemigas, balaADestruir);
                listBalasEnemigas = *balasEnemigas;
            }
            else{//Si la bala no debe morir
                listBalasEnemigas = listBalasEnemigas->next;//Apunta a la siguiente bala
            }
        }
        else if(collition){//Si no hubo colision
            listBalasEnemigas = listBalasEnemigas->next;//Apunta a la siguiente bala
        }
        
        collition = 1;
    }

    while(listBalasUsr != NULL  &&  listUsr->lives != 0){//Chequea si las balas del usuario golpearon algo.

        while(listAliens != NULL  &&  collition){//Chequea todos los aliens
            if(collision(listBalasUsr->pos, listBalasUsr->type, listAliens->pos, listAliens->type)){//Si golpeo a un alien

                collition = 0;
                listAliens->lives -= 1;
                if(listAliens->lives == 0){//Si se mato a ese alien hay que eliminarlo de la lista
                    *alienList = destroyObj(*alienList, listAliens);
                    listAliens = *alienList;
                    if(listAliens == NULL){//Si se mataron a todos los aliens hay que terminar el juego.
                        GAME_STATUS.pantallaActual = MENU;
                        GAME_STATUS.menuActual = MENU_WON_LEVEL;
                        menuGame.exitStatus = 0;
                    }
                }
                listBalasUsr->lives -= 1;
                if(listBalasUsr->lives == 0){//Si la bala debe morir
                    object_t * balaADestruir = listBalasUsr;
                    listBalasUsr = listBalasUsr->next;//Apunta a la siguiente bala
                    *balasUsr = destroyObj(*balasUsr, balaADestruir);
                }
                else{//Si la bala no debe morir
                listBalasUsr = listBalasUsr->next;//Apunta a la siguiente bala
                }
            }
            else{//Si no golpeo a ese alien chequea el siguiente
                listAliens = listAliens->next;
            }
        }
        if(collition){//Si no hubo colision
            listBalasUsr = listBalasUsr->next;//Apunta a la siguiente bala
        }
        collition = 1;
    }
}


int collision(vector_t balaPos, int balaType, vector_t objectPos, int objectType){
//Esta funcion se encarga de detectar si hubo una colision teniendo en cuenta la hitbox de los objetos.

    objectType_t * objType = getObjType(objectType);
    if(objType == NULL){
        return -1;
    }
    int objectAncho = objType->ancho;//Obtiene la hitbox del objeto
    int objectAlto = objType->alto;

    objectType_t * balType = getObjType(balaType);
    if(balType == NULL){
        return -1;
    }
    int balaAncho = balType->ancho;//Obtiene la hitbox de la bala
    int balaAlto = balType->alto;
    
    int minXBala = balaPos.x;//Obtiene los extremos de la bala
    int maxXBala = minXBala + balaAncho - 1;
    int minYBala = balaPos.y;
    int maxYBala = minYBala + balaAlto - 1;

    int minXObj = objectPos.x;//Obtiene los extremos del objeto
    int maxXObj = minXObj + objectAncho - 1;
    int minYObj = objectPos.y;
    int maxYObj = minYObj + objectAlto - 1;

    int interseccionX = maxXBala >= minXObj && minXBala <= maxXObj;//Detecta si se intersectan
    int interseccionY = maxYBala >= minYObj && minYBala <= maxYObj;

    return interseccionX && interseccionY;//Solo hay interseccion si se intersectan en X e Y.
}
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/













/*
object_t * initBarreras(level_setting_t * levelSetting, int cantBarreras, int miniBarrerasY, int miniBarrerasX, ...){
    int vidaMini = levelSetting -> miniBarreraLives;   //Cantidad de vidas de cada minibarrera
    int anchoMini = levelSetting -> anchoMiniBarrera;     //Espacio que ocupa cada minibarrera en x
    int altoMini = levelSetting -> altoMiniBarrera;         //Espacio que ocupa cada minibarrera en y
    int espacioX = ((levelSetting->xMax - levelSetting->xMin + 1)-2*(levelSetting->barreraInicialX) - cantBarreras*anchoMini)/cantBarreras;
    if(((levelSetting->barreraInicialY)*(miniBarrerasY) > levelSetting->yMax) || espacioX <= 0){
        printf("Err in gameLib, initBarreras function: Barrier Out Of Bounds");
        return NULL;
    }
    vector_t posicionBarrera = {levelSetting->barreraInicialX,levelSetting->barreraInicialY};         //Se crea variable que almacenara pos de minibarreras
    va_list typeMiniBarrera;        //Puntero a argumentos variables
    va_start(typeMiniBarrera, miniBarrerasX);    //Se inicializan los argumentos variables, tipo de minibarrera
    object_t * barreras = NULL;               //Se crea variable que almacenara la lista de barreras
    int barrera;                                        //Contador de barreras
    int columna;                                   //Contador de minibarreras en una fila
    int fila;                                      //Contador de filas de minibarreras
    int tipoMiniId;                        //Tipo de minibarrera
    int tiposArray[MAXCANTINPUT];        //Array de tipos de minibarrera ingresados en orden inicializado en 0
    for(barrera=0; barrera < cantBarreras; barrera++){    //Por cada barrera
        for(fila = 0; fila < miniBarrerasY; fila++){      //Por cada fila de minibarreras
            for(columna = 0; columna < miniBarrerasX; columna++){   //Por cada minibarrera
                if(barrera == 0){                           //Si es la primera barrera
                    tipoMiniId = va_arg(typeMiniBarrera, int);    //Se toma el tipo de los argumentos variables de entrada
                    tiposArray[columna+fila*miniBarrerasX] = tipoMiniId;    //Se agrega a la lista el tipo
                }
                else{
                    tipoMiniId = tiposArray[columna+fila*miniBarrerasX];      //Si no es la primera, se recupera el tipo desde el array
                }
                barreras = addObj(barreras, posicionBarrera, tipoMiniId, vidaMini);       //Lo añade a la lista
                posicionBarrera.x += anchoMini;                                                 //Se agrega la siguiente minibarrera a la derecha
            }
            posicionBarrera.y += altoMini;                                                      //Se pasa a la fila de abajo
        }
        posicionBarrera.y = levelSetting->barreraInicialY;                                      //Por cada barrera se resetea la posicion en Y
        posicionBarrera.x += espacioX;                                                          //Por cada barrera se añade un salto en x
    }
    return barreras;                                                                            //Se devuelve la lista de barreras
}  


*/

//FUNCIONES DE ROLES

static objectType_t objtypes[MAX_CANT_OBJTIPOS] = {{.id=NONEOBJTYPEID}};    //Se inicializa un array de objectTypes 

int addObjType(int id, int vel, int ancho, int alto, int initLives, int shootProb, int maxBullets, int balaID, char * sprite1, char * sprite2, char * sprite3){
    if(id == NONEOBJTYPEID){    //Si el id ingresado es 0 entonces deuvuelve error
        printf("Err in gameLib, addObjType function: id cannot be NONEOBJTYPEID = %d, please change the id value in the function call\n", NONEOBJTYPEID);
        return 0;
    }
    int index;  //Se crea un contador
    for(index = 0; index<(MAX_CANT_OBJTIPOS-1) && (objtypes[index]).id != NONEOBJTYPEID && (objtypes[index]).id != id; index++);     //Se recorre el array hasta encontrar el primer elemento vacio
    if(index == MAX_CANT_OBJTIPOS-1){             //Si se excede la cantidad maxima de tipos de objetos, se muestra y se devuele un error
        printf("Err in gameLib, addObjType function: overflow of objtypes array, too many objectTypes = %d\n", MAX_CANT_OBJTIPOS);
        return 0;
    }
    else if((objtypes[index]).id == id){
        printf("Err in gameLib, addObjType function: an object type with the same id = %d was found\n", id);
        return 0;
    }
    else{       //Si no hubo error, rellena el elemento del array
        (objtypes[index]).id=id;
        (objtypes[index]).velocidad=vel;

        (objtypes[index]).ancho=ancho;
        (objtypes[index]).alto=alto;
        (objtypes[index]).initLives=initLives;
        (objtypes[index]).shootProb=shootProb;
        (objtypes[index]).maxBullets=maxBullets;
        (objtypes[index]).balaID=balaID;
        memcpy((objtypes[index]).sprite1, sprite1, MAX_SPRITE_FILE_LENGTH);
        memcpy((objtypes[index]).sprite2, sprite2, MAX_SPRITE_FILE_LENGTH);
        memcpy((objtypes[index]).sprite3, sprite3, MAX_SPRITE_FILE_LENGTH);
        (objtypes[index+1]).id=NONEOBJTYPEID;   //El ultimo lo rellena con vacio
    }
    return 1;
}

int delObjType(int id){
    if(id == NONEOBJTYPEID){    //Si el id ingresado es 0 entonces deuvuelve error
        printf("Err in gameLib, delObjType function: id cannot be NONEOBJTYPEID = %d, please change the id value in the function call\n", NONEOBJTYPEID);
        return 0;
    }
    int index;      //Se crea un contador
    for(index = 0; index<MAX_CANT_OBJTIPOS && (objtypes[index]).id != NONEOBJTYPEID && (objtypes[index]).id != id; index++);      //Se recorre el arreglo hasta encontrar el object type indicado
    if(index == MAX_CANT_OBJTIPOS){             //Si no se encontro 
        printf("Err in gameLib, delObjType function: objectType with %d id not found\n", id);     //Se devuelve un error
        return 0;
    }
    else{       //Si se encontro el objtype a eliminar
        while(index < (MAX_CANT_OBJTIPOS-1) && (objtypes[index]).id != NONEOBJTYPEID){  //Se recorren los ultimos elementos del array (menos el ultimo)
            objtypes[index] = objtypes[index+1];    //Se arrastra el array
            index++;    //Siguiente par de elementos
        }
        return 1;   //Fin de la funcion si no hubo error
    }   
}

objectType_t * getObjType(int id){
    int index;      //Se crea un contador
    for(index = 0; index<MAX_CANT_OBJTIPOS && (objtypes[index]).id != id; index++);      //Se recorre el arreglo hasta encontrar el object type indicado
    if(index == MAX_CANT_OBJTIPOS){             //Si no se encontro 
        printf("Err in gameLib, getObjType function: objectType with %d id not found\n", id);     //Se devuelve un error
        return NULL;
    }
    else{   //Si no hubo error
        return &(objtypes[index]); //Se devuelve un puntero a ese object type
    }
}

void imprimirARRAY(void){
    int index;
    for(index = 0; index<MAX_CANT_OBJTIPOS && (objtypes[index]).id != NONEOBJTYPEID; index++){      //Se recorre el arreglo hasta encontrar el object type indicado
        printf("TIPO N: %d\n\tID: %d\n\tVELOCIDAD: %d\n\tANCHO: %d\n\tALTO: %d\n\tINITLIVES: %d\n\tSHOOTPROB: %d\n\tSPRITE1: %s\n\tSPRITE2: %s\n\tSPRITE3: %s\n",index, (objtypes[index]).id, (objtypes[index]).velocidad, (objtypes[index]).ancho,(objtypes[index]).alto, (objtypes[index]).initLives, (objtypes[index]).shootProb,(objtypes[index]).sprite1,(objtypes[index]).sprite2, (objtypes[index]).sprite3);
    }
}

void printLista(object_t * aux, char * id){
    if (aux != NULL){
        int x;
        for(x = 0; aux->next != NULL; x++){
            printf("Lista ID: %s  -  Nodo %d %p\n", id, x, aux);
            aux = aux -> next;
        }
        printf("Lista ID: %s  - Nodo %d %p\n", id,  x, aux);
        
    }
    else{
        printf("Lista ID: %s VACIA \n", id);
    }
}