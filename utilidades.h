/**********************************************************************************************************************************************************
 * 
                                         _     _   _   _       _               _                    _     
                                        | |   (_) | | (_)     | |             | |                  | |    
                                 _   _  | |_   _  | |  _    __| |   __ _    __| |   ___   ___      | |__  
                                | | | | | __| | | | | | |  / _` |  / _` |  / _` |  / _ \ / __|     | '_ \ 
                                | |_| | | |_  | | | | | | | (_| | | (_| | | (_| | |  __/ \__ \  _  | | | |
                                 \__,_|  \__| |_| |_| |_|  \__,_|  \__,_|  \__,_|  \___| |___/ (_) |_| |_|
                                                                                            
 * 
 ***********************************************************************************************************************************************************
*   Este .h contiene tipos de datos y constantes que son utilizados en varios archivos del programa.
*
 **********************************************************************************************************************************************************/

 #ifndef UTILIDADES_H
 #define UTILIDADES_H

/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/
typedef enum objectTypes {DANIEL, PABLO, NICOLAS, NAVE, BARRERA, BALA_DANIEL, BALA_PABLO, BALA_NICOLAS, BALA_USUARIO} types_t;

typedef struct{//Esta estructura define un vector para las coordenadas
    int x;
    int y;
}vector_t;

typedef struct OBJECT{//Cada alien, barrera, bala es un struct de este tipo y se los organizara en listas en funcion de cual de estos es
    vector_t pos;//Posicion en x e y
    types_t type;//Tipo de objeto y categoria dentro del tipo
    int lives;//Cantidad de vidas del objeto, cada objeto podria tener distinta cantidad de vidas
    char animationStatus;//Estado de la animacion de cada nave, puede ser distinto para cada nave
    struct OBJECT * next;//Puntero al siguiente objeto de la lista.
}object_t;

typedef struct {//Cada alien, barrera, bala es un struct de este tipo y se los organizara en listas en funcion de cual de estos es
    vector_t pos;//Posicion en x e y
    types_t type;//Tipo de objeto y categoria dentro del tipo
    unsigned char maxBullets;//Cantidad maxima de balas
    object_t * listaBalasUsr;
}nave_t;

//DANIEL tiene 1 vida, PABLO 2 vidas y NICOLAS 3 vidas.

typedef struct{
    int xMin;                   //-xMax: coordenada maxima en x alcanzable.
    int xMax;                   //-xMin: coordenada minima en x alcanzable.
    int yMin;                   //-YMax: coordenada maxima en Y alcanzable.
    int yMax;                   //-YMin: coordenada minima en Y alcanzable.
    int saltoX;                 //-saltoX: distancia entre naves en x
    int saltoY;                 //-saltoy: distancia entre naves en y (linea)
    int maxEnemyBullets;        //-maxEnemyBullets: cantidad maxima de balas enemigas concurrentes
    int shootProbDani;          //-shootProbDani: probabilidad de disparo de naves Daniel en cada tick en procentaje
    int shootProbPablo;         //-shootProbPablo: probabilidad de disparo de naves Pablo en cada tick en procentaje
    int shootProbNico;          //-shootProbNico: probabilidad de disparo de naves Nicolas en cada tick en procentaje
    int distInicialX;           //-distInicialX: coordenada en X de la nave del centro de la primera fila
    int distInicialY;           //-distInicialY: coordenada en Y de la nave del centro de la primera fila
    int initUsrLives;           //-initUsrLives: Vidas del usuario en ese nivel
    int initDanielLives;        //-initUsrLives: Vidas de la nave enemiga Daniel en ese nivel
    int initPabloLives;         //-initUsrLives: Vidas de la nave enemiga Pablo en ese nivel
    int initNicolasLives;       //-initUsrLives: Vidas de la nave enemiga Nicolas en ese nivel
    vector_t centerNaveOffset   //-centerOffset: distancia de offset desde la esquina superior izq de la nave hasta el centro
    //ESPACIO PARA BARRERAS
    //AAA BARRERAS
}level_setting_t;
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

//*************TIMER
#define U_SEC2M_SEC 1000    //Conversion de micro segundos a milisegundos.

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

 #endif//utilidades.h