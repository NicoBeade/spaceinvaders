#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/
typedef struct{//Esta estructura define un vector para las coordenadas
    int x;
    int y;
}vector_t;

typedef struct OBJECT{//Cada alien, barrera, bala es un struct de este tipo y se los organizara en listas en funcion de cual de estos es
    vector_t pos;//Posicion en x e y
    types_t type;//Tipo de objeto y categoria dentro del tipo
    int lives;//Cantidad de vidas del objeto, cada objeto podria tener distinta cantidad de vidas
    struct OBJECT * next;//Puntero al siguiente objeto de la lista.
}object_t;

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


//*************DIRECCIONES
#define IZQUIERDA -1        //Constantes utilizadas para indicar la direccion en la que se deben mover los aliens.
#define DERECHA 1
#define ABAJO 2

//*************Definidas en INPUT.h

//*************Definidas en DISPLAY.h
#define X_MAX 7              //Ancho maximo de la pantalla. Vamos a tomar la primer posicion como 0.
#define Y_MAX 7              //Alto maximo de la pantalla

#define MARGEN_X 4          //Margen horizontal en el display
#define MARGEN_Y 1          //Margen vertical en el display

#define DESPLAZAMIENTO_X 1  //Indica cuanto de debe mover una nave en la coordenada x.
#define DESPLAZAMIENTO_Y 1  //Indica cuanto se debe mover una nave en la coordenada y.

#define BULLET_UP -1         //Indica cuanto se debe mover una bala aliada en Y
#define BULLET_DOWN 1      //Indica cuanto se debe mover una bala enemiga en Y

#define ESP_ALIENS_X 1      //Espacio vacio entre los aliens en la coordenada X.
#define TAM_ALIENS_X 3      //Tamano que ocupan los aliens en la coordenada X.
#define ESP_ALIENS_Y 1      //Espacio vacio entre los aliens en la coordenada Y.
#define TAM_ALIENS_Y 3      //Tamano que ocupan los aliens en la coordenada Y.

#define DIST_INICIAL_X 6    //Distancia de los aliens respecto del borde izquierdo de la pantalla al iniciar un nivel, sin offset por nave
#define DIST_INICIAL_Y 4    //Distancia de los aliens respecto del borde superior de la pantalla al iniciar un nivel, sin offset por nave

#define CANT_MAX_ALIENS 12  //Cantidad maxima de aliens, dependiendo del nivel se muestra distinta cantidad, con este tope.
#define CANT_MAX_FILAS 4    //CAntidad maxima de filas, dependiendo del nivel se muestra una cantidad distinta, con este tope.

typedef enum objectTypes {DANIEL, PABLO, NICOLAS, BALA_DANIEL, BALA_PABLO, BALA_NICOLAS, BALA_USUARIO} types_t;
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
object_t* addObj(object_t * firstObj, vector_t setPos, types_t setType, int setLives);   //Agrega un objeto a la lista
object_t * initAliens(int numAliens, int numRows, vector_t firstAlienPos);               //Inicializa la lista completa de aliens usando addObj.
void removeList(object_t* list);                                               //Elimina de heap la lista creada.

void moveAlien (object_t* alien);            //Se encarga de modificar la posicion de los aliens.
static int detectarDireccion(int direccion, object_t* alien);    //Detecta en que direccion se debe mover a los aliens.
int tocaBorde(object_t* alien);  //Detecta si algun alien esta tocando un borde

object_t * moveBala(object_t * ListBalasEnemy, int BalaType, int yMax, int yMin, int velocity);                            //Mueve la bala
object_t * destroyObj(object_t * ListObj, object_t * RipObj);            //Destruye una objeto, recibe puntero a la lista y al objeto que se quiere destruir

//Todas las funciones devuelven puntero a la lista, en el caso de que sea la ultima bala elimina la lista devolviendo NULL
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                __   __                  ___   _         _             _            
                                \ \ / /  __ _   _ _     / __| | |  ___  | |__   __ _  | |  ___   ___
                                 \ V /  / _` | | '_|   | (_ | | | / _ \ | '_ \ / _` | | | / -_) (_-<
                                  \_/   \__,_| |_|      \___| |_| \___/ |_.__/ \__,_| |_| \___| /__/
                                                                                                                                                      
 * 
 ******************************************************************************************************************************************/
//int timerTick;  //Esta variable es un "contador" que se decrementa cada cierto tiempo y con ella se controla cada cuanto tiempo se
                //ejecuta cada accion del programa.

//int velAliens;  //Determina que tan rapido se moveran los aliens

int vidas;      //Indica las vidas restantes del usuario puede ser un campo del struct

//int aliensRestantes;    //Indica cuantos aliens quedan en la partida

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

int main(void) {
    
    int aliensTotales = 12;
    int filasAliens = 4;

    vector_t firstAlienPos = {DIST_INICIAL_X, DIST_INICIAL_Y};

    object_t* listAlien;

    listAlien = initAliens(aliensTotales, filasAliens, firstAlienPos);

//************************************* Esta seccion del codigo se usa para probar que funcionen las listas *****************************
    int i = 1;
    object_t* prueba = listAlien;
    while(prueba != NULL){
        printf("Alien %d: x: %d ; y: %d ; tipo: %d ; vidas: %d\n", i, prueba -> pos.x, prueba -> pos.y, prueba -> type, prueba -> lives);
        i++;
        prueba = prueba -> next;
    }

//****************************************************************************************************************************************

    removeList(listAlien);

    return 0;
}



/*******************************************************************************************************************************************
 * 
                 ___                      _                                _             _     _   _                   
                | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___      /_\   | | (_)  ___   _ _    ___
                | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_)    / _ \  | | | | / -_) | ' \  (_-<
                |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___|   /_/ \_\ |_| |_| \___| |_||_| /__/ 

 * 
 ******************************************************************************************************************************************/

object_t* addObj(object_t * firstObj, vector_t setPos, int setType, int setLives){
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

object_t * initAliens(int numAliens, int numRows, vector_t firstAlienPos){
/*Utiliza la funcion addAlien para crear la lista con todos los aliesn al empezar un nivel. Devuelve un puntero al primer elemento de la lista.
    Recibe como parametros: el numero total de aliens, el numero de filas de aliens y la posicion inicial del primer alien.
*/
	int row;//Variables para desplazamiento
	int col;
	vector_t alienPos = firstAlienPos;//Se almacena la posicion del primer alien. Porque la coordenada x de esta variable sera utilizada mas adelante en la funcion.
	object_t * alienList = NULL;
	for(row = 0; row < numRows; row++){//Realiza la inicializacion
		for(col = 0; col < numAliens/numRows; col++){
			alienList = addObj(alienList, alienPos,row+1,numRows - row);//Agrega un alien a la lista.
			alienPos.x += TAM_ALIENS_X + ESP_ALIENS_X;//Avanza la coordenada X al siguiente alien
		}
		alienPos.y += TAM_ALIENS_Y + ESP_ALIENS_Y; //Cuando llega a un borde lateral aumenta la coordenada Y.
		alienPos.x = firstAlienPos.x;//Y reinicio la coordenada X.
	}
	return alienList;
}

void removeList(object_t* list){
/*Esta funcion se encarga de liberar del heap la lista creada de los aliens*/
    if(list != NULL){
        object_t * lastObj = list; //Se crean dos punteros auxiliares
        object_t * nextObj;
        do {
            nextObj = lastObj -> next; //Se apunta al siguiente nodo
            free(lastObj); //Se libera la memoria dinamica del nodo a eliminar
            lastObj = nextObj;
        } while (nextObj != NULL);
    }
}

void moveAlien (object_t* alien){
/* Esta funcion se encarga de mover la posicion de los aliens teniendo en cuenta para ello la variable direccion.
    Recibe como parametro el puntero al primer alien de la lista.
*/
    int static direccion = DERECHA; //Determina la direccion en la que se tienen que mover los aliens en el proximo tick
    int vx, vy;//Variables temporales utilizadas para incrementar o decrementar las componentes x e y del vector coordenadas.

    //while(1){
        //if( !(timerTick % velAliens)){

            direccion = detectarDireccion(direccion, alien); //Modifica la variable de direccion en funcion al estado actual de la direccion

            switch (direccion){//Primero detecta en que sentido debemos mover las naves.
                case IZQUIERDA:
                    vx = -DESPLAZAMIENTO_X;
                    vy = 0;
                    break;
                case DERECHA:
                    vx = DESPLAZAMIENTO_X;
                    vy = 0;
                    break;
                case ABAJO:
                    vx = 0;
                    vy = DESPLAZAMIENTO_Y;
                    break;
                default:
                    break;
            }
            object_t* auxiliar = alien;
            while (auxiliar != NULL){//Mueve los aliens uno por uno

                auxiliar->pos.x += vx;//Modifica su posicion en x e y
                auxiliar->pos.y += vy;

                auxiliar = auxiliar -> next;
            }
            
        //}
    //}
}


static int detectarDireccion (int direccion, object_t* alien){
/* Esta funcion se encarga de modificar la variable direccion. Es llamada solo por la funcion moveAlien.
    Recibe como parametro la variable direccion y detecta si alguno de los aliens se encuentra en un borde del mapa y en base a eso modificar
    esta variable. Ademas, si toca el borde inferior, pone la variable vidas en 0, pues si los aliens llegan a la parte inferior el usuario perdera.
    Se va a implementar una maquina de estados que en base al estado actual de la direccion y del borde tocado, deduce la direccion resultante.
    Los estados son las direcciones y ya estan definidos en un enum previo.
*/
    switch(direccion){
        
        case DERECHA: //Si se viene moviendo para la derecha
            if (tocaBorde(alien) == DERECHA){ //y toca el borde derecho
                return ABAJO; //se mueve hacia abajo
            }
            else {
                return DERECHA; //si no, sigue moviendose para la derecha
            }
            break;

        case IZQUIERDA: //Si se viene moviendo para la izquierda
            if (tocaBorde(alien) == IZQUIERDA){ //y toca el borde izquierdo
                return ABAJO; //se mueve hacia abajo
            }
            else {
                return IZQUIERDA; //si no, sigue moviendose para la izquierda
            }
            break;

        case ABAJO: //Si se viene moviendo para abajo
            if (tocaBorde(alien) == ABAJO){ //Si algun alien toca el suelo, el jugador pierde la partida
                vidas = 0;
            }
            if (tocaBorde(alien) == DERECHA){ //Si esta tocando el borde derecho, se mueve hacia la izquierda
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

int tocaBorde(object_t* alien){
    int borde = 0;
    while ((alien->next != NULL) && (borde != ABAJO)){ //mientras no se haya llegado al final de la lista o no se haya detectado suelo
        if (alien->pos.x <= 0 + MARGEN_X){ //deteccion borde izquierdo
            borde = IZQUIERDA;
        }
        else if (alien->pos.x >= X_MAX - MARGEN_X){ //deteccion borde derecho
            printf("Llego al borde");
            borde = DERECHA;
        }
        if (alien->pos.y >= Y_MAX - MARGEN_Y){ //deteccion de suelo
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
