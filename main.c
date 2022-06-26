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

typedef struct ALIEN{//Cada uno de los aliens sera una estructura de este tipo. Todos los aliens estan en una lista
    vector_t pos;//Posicion del alien
    int type;//Tipo de nave
    int lives;//Cantidad de vidas del alien (Distintos tipos de alien pueden tener mas o menos vidas)
    struct ALIEN * next;//Puntero al siguiente alien de la lista.
}alien_t;

typedef struct BALA{//Cada uno de las balas sera una estructura de este tipo. Todas las balas estan en una lista
    vector_t pos;//Posicion de la bala
    int type;//Tipo de bala, coincide con la nave que lo disparo.
    struct BALA * next;//Puntero al siguiente alien de la lista.
}bala_t;
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
#define X_MAX 7              //Ancho maximo de la pantalla
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

enum alienTypes {NODRIZA, SUPERIOR, MEDIO, INFERIOR};
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
alien_t * addAlien(alien_t * firstAlien, vector_t setPos, int setType, int setLives);    //Agrega un alien a la lista.
alien_t * initAliens(int numAliens, int numRows, vector_t firstAlienPos);               //Inicializa la lista completa de aliens usando addAlien.
void removeAlienList(alien_t* listAlien);                                               //Elimina de heap la lista creada.

void moveAlien (alien_t* alieN);            //Se encarga de modificar la posicion de los aliens.
static int detectarDireccion(int direccion, alien_t* alien);    //Detecta en que direccion se debe mover a los aliens.
int tocaBorde(alien_t* alien);  //Detecta si algun alien esta tocando un borde

bala_t* addBala(bala_t * firstBala, vector_t setPos, int SetType);       //Añade una bala
bala_t * moveBalaEnemy(bala_t * ListBalasEnemy, int BalaType);          //Mueve una unidad todas las balas enemigas del tipo indicado
bala_t * moveBalaUsr(bala_t * ListBalaUsr);                             //Mueve una unidad la bala del usuario
bala_t * destroyBala(bala_t * ListBala, bala_t * RipBala);              //Destruye una bala, recibe puntero a la lista y a la bala que se quiere destruir
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

    alien_t* listAlien;

    listAlien = initAliens(aliensTotales, filasAliens, firstAlienPos);

//************************************* Esta seccion del codigo se usa para probar que funcionen las listas *****************************
    int i = 1;
    alien_t* prueba = listAlien;
    while(prueba != NULL){
        printf("Alien %d: x: %d ; y: %d ; tipo: %d ; vidas: %d\n", i, prueba -> pos.x, prueba -> pos.y, prueba -> type, prueba -> lives);
        i++;
        prueba = prueba -> next;
    }

//****************************************************************************************************************************************

    removeAlienList(listAlien);

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

alien_t* addAlien(alien_t * firstAlien, vector_t setPos, int setType, int setLives){
/* Esta funcion se encarga de agregar un nuevo alien a la lista, inicializando su posicion, tipo y cantidad de vidas.
    Devuelve un puntero al primer elemento de la lista.
*/	
	alien_t * newAlien = malloc(sizeof(alien_t));//Agrega el nuevo alien

	if(newAlien == NULL){//Si no se puede hacer el malloc indica error.
		printf( "No se ha podido agregar el elemento a la lista.\n");
		return NULL; //error
	}

    if(firstAlien != NULL){//Si no es el primero de la lista debe avanzar hasta el ultimo elemento.
        alien_t * lastAlien = firstAlien;//Se almacena el puntero al primer elemento.

		while(lastAlien -> next != NULL){
			lastAlien = lastAlien -> next;
		}
        lastAlien -> next = newAlien;
	}
    else{//Si es el primero de la lista debemos devolver ese puntero.
        firstAlien = newAlien;
    }

	newAlien -> pos = setPos;//Asigna los valores indicados en los distitntos campos del alien.
	newAlien -> type = setType;
	newAlien -> lives = setLives;
    newAlien -> next = NULL;

	return firstAlien;//Devuelve un puntero al primer elemento.
}

alien_t * initAliens(int numAliens, int numRows, vector_t firstAlienPos){
/*Utiliza la funcion addAlien para crear la lista con todos los aliesn al empezar un nivel. Devuelve un puntero al primer elemento de la lista.
    Recibe como parametros: el numero total de aliens, el numero de filas de aliens y la posicion inicial del primer alien.
*/
	int row;//Variables para desplazamiento
	int col;
	vector_t alienPos = firstAlienPos;//Se almacena la posicion del primer alien. Porque la coordenada x de esta variable sera utilizada mas adelante en la funcion.
	alien_t * alienList = NULL;
	for(row = 0; row < numRows; row++){//Realiza la inicializacion
		for(col = 0; col < numAliens/numRows; col++){
			alienList = addAlien(alienList, alienPos,row+1,numRows - row);//Agrega un alien a la lista.
			alienPos.x += TAM_ALIENS_X + ESP_ALIENS_X;//Avanza la coordenada X al siguiente alien
		}
		alienPos.y += TAM_ALIENS_Y + ESP_ALIENS_Y; //Cuando llega a un borde lateral aumenta la coordenada Y.
		alienPos.x = firstAlienPos.x;//Y reinicio la coordenada X.
	}
	return alienList;
}

void removeAlienList(alien_t* listAlien){
/*Esta funcion se encarga de liberar del heap la lista creada de los aliens*/
    if(listAlien != NULL){
        alien_t * lastAlien = listAlien; //Se crean dos punteros auxiliares
        alien_t * nextAlien;
        do {
            nextAlien = lastAlien -> next; //Se apunta al siguiente nodo
            free(lastAlien); //Se libera la memoria dinamica del nodo a eliminar
            lastAlien = nextAlien;
        } while (nextAlien != NULL);
    }
}

void moveAlien (alien_t* alien){
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
            alien_t* auxiliar = alien;
            while (auxiliar != NULL){//Mueve los aliens uno por uno

                auxiliar->pos.x += vx;//Modifica su posicion en x e y
                auxiliar->pos.y += vy;

                auxiliar = auxiliar -> next;
            }
            
        //}
    //}
}


static int detectarDireccion (int direccion, alien_t* alien){
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

int tocaBorde(alien_t* alien){
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

bala_t* addBala(bala_t * firstBala, vector_t setPos, int setType){
/* Esta funcion se encarga de agregar una nueva bala a la lista, inicializando su posicion.
    Devuelve un puntero al primer elemento de la lista.
*/	
	bala_t * newBala = malloc(sizeof(bala_t));//Agrega el nuevo alien

	if(newBala == NULL){//Si no se puede hacer el malloc indica error.
		printf( "No se ha podido agregar el elemento a la lista.\n");
		return NULL; //error
	}

    if(firstBala != NULL){//Si no es el primero de la lista debe avanzar hasta el ultimo elemento.
        bala_t * lastBala = firstBala;//Se almacena el puntero al primer elemento.

		while(lastBala -> next != NULL){
			lastBala = lastBala -> next;
		}
        lastBala -> next = newBala;
	}
    else{//Si es el primero de la lista debemos devolver ese puntero.
        firstBala = newBala;
    }

	newBala -> pos = setPos;//Asigna los valores indicados en los distitntos campos del alien.
    newBala -> type = setType; 
    newBala -> next = NULL;

	return firstBala;//Devuelve un puntero al primer elemento.
}


bala_t * moveBalaEnemy(bala_t * ListBalasEnemy, int BalaType){      //Mueve las balas enemigas de un tipo especifico.
    bala_t * Bala = ListBalasEnemy;
    bala_t * newList = ListBalasEnemy;
    if(Bala != NULL){
        do{
            if(Bala -> type == BalaType){
                if((Bala -> pos.y) <= (Y_MAX - BULLET_DOWN)){   //Si la bala continua dentro del display 
                    Bala -> pos.y += BULLET_DOWN;           //Se mueve la bala hacia abajo
                }
                else{                                       //Si la bala se fue del display se elimina
                    newList = destroyBala(ListBalasEnemy, Bala);
                }
            }
            Bala = Bala -> next;
        }while(Bala != NULL);
    }
    return newList;
}

bala_t * moveBalaUsr(bala_t * ListBalaUsr){
    bala_t * ListBala = ListBalaUsr;
    if(ListBala != NULL){                    //Si la bala aliada existe
        if(ListBala -> pos.y < BULLET_UP){      //Si la bala esta en el limite del mapa
            free(ListBala);                  //Se elimina la bala
            ListBala = NULL;                //Se elimina la lista
        }
        else{                                                 //Si la bala esta dentro de los limites, se mueve
            ListBalaUsr -> pos.y += BULLET_UP;                
        }
    }
    return ListBala;                        //Devuelve la lista
}

bala_t * destroyBala(bala_t * ListBala, bala_t * RipBala){
    bala_t * Bala = ListBala;
    if(Bala != NULL && RipBala != NULL){        //Si la lista y la bala existe
        if(Bala != RipBala){                    //Si la bala no es la unica en la lista
            bala_t * PreBala = Bala;            //Bala anterior a la que se va a eliminar
            while(PreBala -> next != RipBala){     //Recorre la lista hasta encontrar la bala anterior a la que se quiere destruir
                PreBala = PreBala -> next;
            }
            PreBala -> next = RipBala -> next;      //Se asigna la siguiente bala
            free(RipBala);                          //Se libera la memoria de la bala
        }
        else{                                       //Si la bala a eliminar es la primera
            free(RipBala);
            Bala = RipBala -> next;                 //Devuelve puntero a la segunda bala si es que existe
        }
    }
    return Bala;
}