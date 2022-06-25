#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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
#define IZQUIERDA -1    //Constantes utilizadas para indicar la direccion en la que se deben mover los aliens.
#define DERECHA 1
#define ABAJO 1

#define ESP_ALIENS_X 1      //Espacio vacio entre los aliens en la coordenada X.
#define TAM_ALIENS_X 3      //Tamano que ocupan los aliens en la coordenada X.
#define ESP_ALIENS_Y 1      //Espacio vacio entre los aliens en la coordenada Y.
#define TAM_ALIENS_Y 3      //Tamano que ocupan los aliens en la coordenada Y.

enum alienTypes {nodriza, superior, medio, inferior};
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
alien_t* addAlien(alien_t * firstAlien, vector_t setPos, int setType, int setLives);    //Agrega un alien a la lista.
alien_t * initAliens(int numAliens, int numRows, vector_t firstAlienPos);   //Inicializa la lista completa de aliens usando addAlien.
void removeAlienList(int aliensTotales, alien_t* listAlien);    //Elimina de heap la lista creada.

//void moveAlien (alien_t* alien, int direccion); //Se encarga de modificar la posicion de los aliens.
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

//int direccion;  //Determina la direccion en la que se tienen que mover los aliens en el proximo tick

//int vidas;      //Indica las vidas restantes del usuario

//int xMax;       //Ancho maximo de la pantalla
//int yMax;       //Alto maximo de la pantalla

int aliensTotales;  //Indica con cuantos aliens comienza el juego
//int aliensRestantes;    //Indica cuantos aliens quedan en la partida
int filasAliens; //Cantidad total de filas de aliens
int distInicialX = 6;    //Distancia de los aliens respecto del borde izquierdo de la pantalla al iniciar un nivel.
int distInicialY = 4;    //Distancia de los aliens respecto del borde superior de la pantalla al iniciar un nivel.
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

int main(void) {
    
    aliensTotales = 12;
    filasAliens = 4;

    vector_t firstAlienPos = {distInicialX, distInicialY};

    alien_t* listAlien;

    listAlien = initAliens(aliensTotales, filasAliens, firstAlienPos);

//************************************* Esta seccion del codigo se usa para probar que funcionen las listas *****************************
    int i = 1;
    alien_t* prueba = listAlien;
    do{
        printf("Alien %d: x: %d ; y: %d ; tipo: %d ; vidas: %d\n", i, prueba -> pos.x, prueba -> pos.y, prueba -> type, prueba -> lives);
        i++;
        prueba = prueba -> next;
    }
    while(prueba != NULL);
//****************************************************************************************************************************************

    removeAlienList(aliensTotales, listAlien);

    return 0;
}


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
	int row;//VAriables para desplazamiento
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

void removeAlienList(int aliensTotales, alien_t* listAlien){
/*Esta funcion se encarga de liberar del heap la lista creada de los aliens*/

    if(aliensTotales > 1){
        removeAlienList(aliensTotales - 1, listAlien->next);
    }
    free(listAlien);
    #ifdef DEBUG
    printf("Se elimino el alien: %d\n", aliensTotales);
    #endif
}

/*
void moveAlien (alien_t* alien, int direccion){
/* Este thread se encarga de mover la posicion de los aliens teniendo en cuenta la variable direccion para ello. Ademas, cuando se toca alguno
    de los bordes puede modificar el valor de direccion si se toca uno de los laterales o poner en 0 la variable vidas si se llega al borde
    inferior.
*
    while(1){
        if( !(timerTick % velAliens)){
            int vx, vy;//Variables temporales utilizadas para incrementar o decrementar las componentes x e y del vector coordenadas.

            switch (direccion){//Primero detecta en que sentido debemos mover las naves.
                case IZQUIERDA:
                    vx=-1;
                    vy=0;
                    break;
                case DERECHA:
                    vx=1;
                    vy=0;
                    break;
                case ABAJO:
                    vx=0;
                    vy=-1;
                    break;
            }
            int i = 1;
            while(i){//Mueve los aliens uno por uno

                alien->pos.x += vx;//Modifica su posicion en x e y
                alien->pos.y += vy;

                if( (alien->pos.x == 0) && (direccion == IZQUIERDA)){//Todos los casos posibles de que las naves toquen alguno de los bordes.
                    direccion = ABAJO;
                }
                else if( (alien->pos.x == 0) && (direccion == ABAJO)){
                    direccion = DERECHA;
                }
                else if( (alien->pos.x == xMax) && (direccion == DERECHA)){
                    direccion = ABAJO;
                }
                else if( (alien->pos.x == xMax) && (direccion == ABAJO)){
                    direccion = IZQUIERDA;
                }
                else if(alien->pos.y == yMax){
                    vidas = 0;
                }

                if(alien->next == NULL){//Si el puntero next es NULL quiere decir que es el ultimo elemento de la lista, entonces debe salir del loop.
                    i = 0;
                }
                else{//Sino termino apunta al siguiente elemento.
                    alien = alien->next;
                }
            }
        }
    }
}
*/
