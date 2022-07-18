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

void * moveAlienThread(void* argMoveAlien){
/* 
    Este thread se encarga de mover la posicion de los aliens teniendo en cuenta para ello la variable direccion.
*/
    object_t * auxiliar;
    int static direccion = DERECHA; //Determina la direccion en la que se tienen que mover los aliens en el proximo tick
    int vx, vy;//Variables temporales utilizadas para incrementar o decrementar las componentes x e y del vector coordenadas.

    while(1){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velAliens) == 0 ){

            sem_wait(&semaforo);

            if(*(((argMoveAlien_t*)argMoveAlien) -> alienList) == NULL){
                printf("Error. AlienList cannot be NULL pointer in thread ""moveAlien"".\n");
            }

            direccion = detectarDireccion(direccion, ((argMoveAlien_t*)argMoveAlien) -> levelSettings, *(((argMoveAlien_t*)argMoveAlien) -> alienList) );  //Modifica la variable de direccion en funcion al estado actual de la direccion

            switch (direccion){//Primero detecta en que sentido debemos mover las naves.
                case IZQUIERDA:
                    vx = - DESPLAZAMIENTO_X_L(argMoveAlien);
                    vy = 0;
                    break;
                case DERECHA:
                    vx = DESPLAZAMIENTO_X_L(argMoveAlien);
                    vy = 0;
                    break;
                case ABAJO:
                    vx = 0;
                    vy = DESPLAZAMIENTO_Y_L(argMoveAlien);
                    break;
                default:
                    break;
            }
            auxiliar = *(((argMoveAlien_t*)argMoveAlien) -> alienList);
            while (auxiliar != NULL){//Mueve los aliens uno por uno

                auxiliar->pos.x += vx;//Modifica su posicion en x e y
                auxiliar->pos.y += vy;
                auxiliar->animationStatus++;
                auxiliar = auxiliar -> next;
            }

            sem_post(&semaforo);
        }
    }
    pthread_exit(0);
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
            break;
    } 
    return 0;
}

static int tocaBorde(level_setting_t * levelSettings, object_t * alien){
/* Esta funcion detecta si alguna de las naves toco algun borde, teniendo en cuenta todas las posibles combinaciones.
    Devuelve que borde fue tocado.
*/
    int borde = 0;
    while ((alien->next != NULL) && (borde != ABAJO)){ //mientras no se haya llegado al final de la lista o no se haya detectado suelo
        if (alien->pos.x <= 0 + MARGEN_X_L(levelSettings)){ //deteccion borde izquierdo
            borde = IZQUIERDA;
        }
        else if (alien->pos.x > X_MAX_L(levelSettings) - MARGEN_X_L(levelSettings) - ANCHO_ALIEN_L(levelSettings)){ //deteccion borde derecho
            printf("Llego al borde");
            borde = DERECHA;
        }
        if (alien->pos.y >= Y_MAX_L(levelSettings) - MARGEN_Y_L(levelSettings)){ //deteccion de suelo
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

void * moveBalaThread(void * argMoveBala){

    while(1){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % velBalas) == 0 ){
            sem_wait(&semaforo);
            object_t * balas = NULL;
            //printf("%d   %d", Y_MAX_L(argMoveBala), Y_MIN_L(argMoveBala));
            //printf("%p   ", BALAS_ENEMIGAS_L(argMoveBala));.
            //printf("GEORGE %p   ",  BALAS_ENEMIGAS_L(argMoveBala));
            BALAS_ENEMIGAS_L(argMoveBala) = moveBala( BALAS_ENEMIGAS_L(argMoveBala), NICOLAS, Y_MAX_L(argMoveBala), Y_MIN_L(argMoveBala), ((argMoveBala_t*) argMoveBala) -> velocidadNicolas);
            BALAS_ENEMIGAS_L(argMoveBala) = moveBala(BALAS_ENEMIGAS_L(argMoveBala), BALA_PABLO, Y_MAX_L(argMoveBala), Y_MIN_L(argMoveBala), ((argMoveBala_t*) argMoveBala) -> velocidadPablo);
            //printf("%p   ", ((argMoveBala_t*) argMoveBala) -> balasEnemigas);
            BALAS_ENEMIGAS_L(argMoveBala) = moveBala(BALAS_ENEMIGAS_L(argMoveBala), BALA_DANIEL, Y_MAX_L(argMoveBala), Y_MIN_L(argMoveBala), ((argMoveBala_t*) argMoveBala) -> velocidadDaniel);
            //printf("%p   ", ((argMoveBala_t*) argMoveBala) -> balasEnemigas);
            BALAS_USR_L(argMoveBala) = moveBala(BALAS_USR_L(argMoveBala), BALA_USUARIO, Y_MAX_L(argMoveBala), Y_MIN_L(argMoveBala), ((argMoveBala_t*) argMoveBala) -> velocidadUsr);
            //printf("%p        ", ((argMoveBala_t*) argMoveBala) -> balasEnemigas);
            //printf("%p\n", ((argMoveBala_t*) argMoveBala) -> balasUsr);
            sem_post(&semaforo);
        } 
    }
}

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

object_t * shootBala(object_t * listaNaves, object_t * listaBalas, level_setting_t * levelSetting){
    int balasActuales = countList(listaBalas);                  //Se cuenta la cantidad de balas activas
    int balasDisponibles;                                       //Se crea una variable con la cantidad de balas que se puede disparar
    if(listaNaves -> type == NAVE){                             //Si es una nave de usuario
        balasDisponibles = levelSetting -> maxUsrBullets - balasActuales;   //La cantidad de balas disponibles es la resta entre las maximas y las actuales
    }
    else{                                                       //Si es una nave enemiga
        balasDisponibles = levelSetting -> maxEnemyBullets - balasActuales; //La cantidad de balas disponibles es la resta entre las maximas y las actuales
    }
    object_t * nave = listaNaves;                               //Se crea un puntero a la lista de naves
    object_t * bala = listaBalas;                               //Se crea un puntero a la lista de balas
    int probabilidad;                                           //
    int tipoBala;
    while(balasDisponibles > 0 && nave != NULL){
        switch(nave -> type){
            case DANIEL:
                probabilidad = levelSetting->shootProbDani;
                tipoBala = BALA_DANIEL;
                break;
            case PABLO:
                probabilidad = levelSetting->shootProbPablo;
                tipoBala = BALA_PABLO;
                break;
            case NICOLAS:
                probabilidad = levelSetting->shootProbNico;
                tipoBala = BALA_NICOLAS;
                break;
            case NAVE:
                probabilidad = 100;
                tipoBala = BALA_USUARIO;
                break;
            default:
                return NULL;
        }
        if((rand()%100) < probabilidad){
            vector_t posicionBala;
            if(tipoBala == BALA_USUARIO){
                posicionBala.x = nave->pos.x - (levelSetting->anchoUsr)/2;
                posicionBala.y = nave->pos.y - (levelSetting->altoUsr)/2;
            }
            else{
                posicionBala.x = nave->pos.x + (levelSetting->anchoNave)/2;
                posicionBala.y = nave->pos.y + (levelSetting->altoNave)/2;
            }
            bala = addObj(bala, posicionBala, tipoBala, 1);
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

void moveNaveUsuario(object_t * naveUsuario, int desplazamiento, int xMax, int tamNaveX){
/* Esta funcion se llama como callback por los threads que manejan el input tanto en allegro como en la raspberry. Se encarga de actualizar
    la posicion de la nave del usuario.
*/

    if( !((naveUsuario -> pos.x <= 10 && desplazamiento < 0) || ((naveUsuario -> pos.x >= xMax - tamNaveX -10 ) && desplazamiento > 0)) ){//Chequea que no este en los bordes.
        naveUsuario -> pos.x += desplazamiento;//Desplaza la nave
    }   
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/



object_t * initBarreras(level_setting_t * levelSetting, int cantBarreras, int miniBarrerasY, int miniBarrerasX, ...){
    int vidaMini = levelSetting -> miniBarreraLives;   //Cantidad de vidas de cada minibarrera
    int anchoMini = levelSetting -> anchoMiniBarrera;     //Espacio que ocupa cada minibarrera en x
    int altoMini = levelSetting -> altoMiniBarrera;         //Espacio que ocupa cada minibarrera en y
    int espacioX = ((levelSetting->xMax - levelSetting->xMin + 1)-2*(levelSetting->barreraInicialX) - cantBarreras*anchoMini)/cantBarreras;
    if(((levelSetting->barreraInicialY)*(miniBarrerasY) > levelSetting->yMax) || espacioX <= 0){
        printf("Err Barrier Out Of Bounds");
        return NULL;
    }
    vector_t posicionBarrera = {levelSetting->barreraInicialX,levelSetting->barreraInicialY};         //Se crea variable que almacenara pos de minibarreras
    va_list typeMiniBarrera;        //Puntero a argumentos variables
    va_start(typeMiniBarrera, miniBarrerasX);    //Se inicializan los argumentos variables, tipo de minibarrera
    object_t * barreras = NULL;               //Se crea variable que almacenara la lista de barreras
    int barrera;                                        //Contador de barreras
    int columna;                                   //Contador de minibarreras en una fila
    int fila;                                      //Contador de filas de minibarreras
    types_t tipoMiniId;                        //Tipo de minibarrera
    types_t tiposArray[MAXCANTINPUT];        //Array de tipos de minibarrera ingresados en orden inicializado en 0
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




//FUNCIONES DE ROLES

static objectType_t objtypes[MAX_CANT_OBJTIPOS] = {{.id=NONEOBJTYPEID}};

int addObjType(int id, int vel, int ancho, int alto, int initLives, int shootProb, int distInitX, int distInitY){
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
        (objtypes[index]).alto=initLives;
        (objtypes[index]).shootProb=shootProb;
        (objtypes[index]).distInicialX=distInitX;
        (objtypes[index]).distInicialY=distInitY;
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
        printf("TIPO N: %d\n\tID: %d\n\tVELOCIDAD: %d\n\tANCHO: %d\n\tALTO: %d\n\tINITLIVES: %d\n\tSHOOTPROB: %d\n\tDISTINITX: %d\n\tDISTINITY: %d\n",index, (objtypes[index]).id, (objtypes[index]).velocidad, (objtypes[index]).ancho,(objtypes[index]).alto, (objtypes[index]).initLives, (objtypes[index]).shootProb, (objtypes[index]).distInicialX, (objtypes[index]).distInicialY);
    }
}