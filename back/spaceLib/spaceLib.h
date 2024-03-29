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
*
 **********************************************************************************************************************************************************/

 #ifndef SPACELIB_H
 #define SPACELIB_H

#include <stdint.h>


//*************DIRECCIONES
#define IZQUIERDA -1        //Constantes utilizadas para indicar la direccion en la que se deben mover los aliens.
#define DERECHA 1
#define ABAJO 2
#define MAX_SPRITE_FILE_LENGTH 100
#define MAX_SOUND_FILE_LENGTH 100
#define MOTHERSHIP_CREATED 1
#define TIRADAS_DE_PROBABILIDAD 30
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

typedef struct OBJECT{//Cada alien, barrera, bala y la nave del usuario es un struct de este tipo y se los organizara en listas en funcion de cual de estos es.
    vector_t pos;//Posicion en x e y
    int type;//Tipo de objeto y categoria dentro del tipo
    int lives;//Cantidad de vidas del objeto, cada objeto podria tener distinta cantidad de vidas
    char animationStatus;//Estado de la animacion de cada nave, puede ser distinto para cada nave
    struct OBJECT * next;//Puntero al siguiente objeto de la lista
}object_t;
 

//-----------------------------------------------------------------LEVEL SETTINGS---------------------------------------------------------------------------------
// Este struct contiene toda la informacion necesaria para crear un nivel.
typedef struct{
    int xMin;                   //-xMax: coordenada maxima en x alcanzable.
    int xMax;                   //-xMin: coordenada minima en x alcanzable.
    int yMin;                   //-YMax: coordenada maxima en Y alcanzable.
    int yMax;                   //-YMin: coordenada minima en Y alcanzable.
    int saltoX;                 //-saltoX: distancia entre naves en x
    int saltoY;                 //-saltoy: distancia entre naves en y (linea)

    int margenX;                //-margenX: margen que queda libre en la pantalla (los aliens no pueden pasar de este margen)
    int margenY;                //-margenY: margen que queda libre en la pantalla (los aliens no pueden pasar de este margen)

    int desplazamientoX;        //Cantidad de unidades que se mueven los aliens en X por tick
    int desplazamientoY;        //Cantidad de unidades que se mueven los aliens en Y por tick
    int desplazamientoUsr;      //-desplazamientoUsr: cantidad de unidades que se mueve el usuario por tick.
    int velAliens;              //Velocidad Aliens por tick
    int velMothership;          //Velocidad nave nodriza por tick
    int velBalas;               //Velocidad Balas por tick

    int mothershipYpos;         //Posicion en Y donde se mueve la mothership (horizontal)
    int mothershipAsset;        //Asset correspondiente a la mothership
    int maxMShipXLevel;         //Maxima cantidad de motherships por nivel
}level_setting_t;

typedef struct {//Este struct se utiliza para obtener la entrada del usuario.

	int x;
	int y;
	uint8_t press;
} keys_t;

typedef struct{
	int id;             //Identificador del tipo de objeto
    int velocidad;      //Cantidad de distancia que se mueve el objeto
	int ancho;          //Ancho del objeto
	int alto;           //Alto del objeto
	int initLives;      //Vidas iniciales del tipo de objeto
	int shootProb;      //Probabilidad de disparo del tipo de objeto
    int maxBullets;     //Cantidad maxima de balas del escuadron
    int balaID;         //ID del objectType de la bala asociada a este objeto
    int sprite1;        //Sprite 1
    int sprite2;        //Sprite 2
    int sprite3;        //Sprite 3
    int sprite4;        //Sprite 4
    int score;          //Almacena el puntaje del usuario y cuanto puntaje vale matar a cada alien
    char shootSound[MAX_SOUND_FILE_LENGTH]; //Sonido de disparo
    char deathSound[MAX_SOUND_FILE_LENGTH]; //Sonido de muerte
    int aliado; //1 si es aliado, 0 si es enemigo
}objectType_t;

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

#define MAX_CANT_OBJTIPOS 30 // Maxima cantidad de tipos de objetos que se pueden ingresar
#define NONEOBJTYPEID 100	//	Tipo de objeto nulo/terminador

//*************EVENTOS DE SPACELIB
enum EVENTOS_SPACELIB {  FASTER_ALIENS = 1,WON_LEVEL,LOST_LEVEL,
                        SL_COLISION_ALIEN_TOCADO,SL_COLISION_ALIEN_MUERTO,SL_COLISION_USER_TOCADO,SL_COLISION_USER_MUERTO,SL_COLISION_MOTHERSHIP_MUERTA,SL_COLISION_BARRERA_TOCADA,SL_COLISION_BARRERA_MUERTA, SL_COLISION_BALAS,
                        SL_MOTHERSHIP_APARECE,
                        SL_MOVIMIENTO_ALIENS,
                        SL_BALA_USER,SL_BALA_ALIEN,
              };
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
object_t* addObj(object_t * firstObj, vector_t setPos, int setTypeId, int setLives);   //Agrega un objeto a la lista0
char moveAlien(level_setting_t*  levelSettings, object_t ** alienList, int* direccion);//Esta funcion es llamada por el thread y es la
                                                                                                     //la encargada de modificar la posicion de los aliens.
int mothershipCreator(object_t **mothershipListPointer, level_setting_t * levelSettings);


//*****************BALAS
object_t * moveBala(object_t ** ListBalasEnemy, level_setting_t * levelSetting);
char shootBala(object_t ** listaNaves, object_t ** listaBalas, level_setting_t * levelSetting);


//*****************USUARIO
int moveNaveUsuario(object_t ** naveUsuario, level_setting_t* levelSettings, int direccion); //Se encarga de actualizar la posicion de la nave del usuari


//*****************COLLIDER
signed char* collider(level_setting_t * levelSettings, object_t ** alienList, object_t ** usrList, object_t ** barrerasList, object_t ** balasEnemigas, object_t ** balasUsr, object_t ** motherShip, int nivelActual, int* scoreReal, int* scoreInstantaneo);//Recorre todas las listas para chequear si hubo colisiones


//*****************OBJTYPES
void imprimirARRAY(void);																							//Muestra el array de tipos de objetos en stdout
objectType_t * getObjType(int id);																					//Devuelve el puntero al tipo de objeto deseado	
int addObjType(int id, int vel, int ancho, int alto, int initLives, int shootProb, int maxBullets, int balaID, int sprite1, int sprite2, int sprite3, int sprite4, char * deathSound, char * shootSound, int score, int aliado);
int delObjType(int id);			                                                                             //Elimina de heap la lista creada.    																					//Elimina un tipo de objeto
int delAllObjType(void);

//*****************OBJETOS
object_t* removeList(object_t* lista); 
object_t * destroyObj(object_t * ListObj, object_t * RipObj);


//*****************GENERAL
void printLista(object_t * lista, char * id);
void imprimirLevelSetting(level_setting_t * levelSettings);
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


 #endif//spaceLib.h
