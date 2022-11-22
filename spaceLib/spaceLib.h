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

 #ifndef SPACELIB_H
 #define SPACELIB_H

#include <stdint.h>

//*************DIRECCIONES
#define IZQUIERDA -1        //Constantes utilizadas para indicar la direccion en la que se deben mover los aliens.
#define DERECHA 1
#define ABAJO 2
#define MAX_SPRITE_FILE_LENGTH 100
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
    struct OBJECT * next;//Puntero al siguiente objeto de la lista.
}object_t;


//-----------------------------------------------------------------LEVEL SETTINGS---------------------------------------------------------------------------------
// Este struct contiene toda la informacion necesaria para crear un nivel.
typedef struct{
    //Informaciones de tamaño (definido por el front end)
    int xMin;                   //-xMax: coordenada maxima en x alcanzable.
    int xMax;                   //-xMin: coordenada minima en x alcanzable.
    int yMin;                   //-YMax: coordenada maxima en Y alcanzable.
    int yMax;                   //-YMin: coordenada minima en Y alcanzable.
    int saltoX;                 //-saltoX: distancia entre naves en x
    int saltoY;                 //-saltoy: distancia entre naves en y (linea)
    int distInicialX;           //-distInicialX: coordenada en X de la nave del centro de la primera fila
    int distInicialY;           //-distInicialY: coordenada en Y de la nave del centro de la primera fila
    //int barreraInicialX;        //-barreraInicialX: coordenada en X de la primera barrera
    //int barreraInicialY;        //-barreraInicialY: coordenada en Y de la primera barrera
    int anchoUsr;               //-anchoUsr: Ancho de la nave del usuario
    //int altoUsr;                //-altoUsr: Alto de la nave del usuario
    //int anchoAlien;             //-anchoNave: Ancho de las naves enemigas
    //int altoAlien;              //-altoNave: Alto de las naves enemigas
    //int anchoMiniBarrera;       //-anchoMiniBarrera: Ancho de las minibarreras
    //int altoMiniBarrera;        //-altoMiniBarrera: Alto de las minibarreras
    int margenX;                //-margenX: margen que queda libre en la pantalla (los aliens no pueden pasar de este margen)
    int margenY;                //-margenY: margen que queda libre en la pantalla (los aliens no pueden pasar de este margen)
    int disInicialUsrX;         //-distInicialUsrX: distancia inicial del usuario al iniciar un nivel en la coordenada X.
    int disInicialUsrY;         //-distInicialUsrY: distancia inicial del usuario al iniciar un nivel en la coordenada Y.

    //Datos para la jugabilidad
    //int maxUsrBullets;          //-maxUsrBullets: cantidad maxima de balas de la nave del usuario concurrentes
    //int maxEnemyBullets;        //-maxEnemyBullets: cantidad maxima de balas enemigas concurrentes
    //int shootProbDani;          //-shootProbDani: probabilidad de disparo de naves Daniel en cada tick en procentaje
    //int shootProbPablo;         //-shootProbPablo: probabilidad de disparo de naves Pablo en cada tick en procentaje
    //int shootProbNico;          //-shootProbNico: probabilidad de disparo de naves Nicolas en cada tick en procentaje
    //int initUsrLives;           //-initUsrLives: Vidas del usuario en ese nivel
    //int initDanielLives;        //-initUsrLives: Vidas de la nave enemiga Daniel en ese nivel
    //int initPabloLives;         //-initUsrLives: Vidas de la nave enemiga Pablo en ese nivel
    //int initNicolasLives;       //-initUsrLives: Vidas de la nave enemiga Nicolas en ese nivel
    //int miniBarreraLives;       //-miniBarreraLives: Vidas de cada minibarrera
    int desplazamientoX;        //Cantidad de unidades que se mueven los aliens en X por tick
    int desplazamientoY;        //Cantidad de unidades que se mueven los aliens en Y por tick
    int desplazamientoUsr;        //-desplazamientoUsr: cantidad de unidades que se mueve el usuario por tick.
}level_setting_t;

typedef struct{//Contiene el estado del juego.

    unsigned char pantallaActual;//Indica si el juego se encuentra en partida o en un menu.
    unsigned char nivelActual;//Indica el nivel que esta en juego.
    unsigned char menuActual;//Indica el menu que esta corriendo.
    unsigned char menuAnterior;//Almacena temporalmente el menu anterior.
    unsigned char inGame;//Indica si se deben correr los threads que ejecutan el juego.
    unsigned char exitStatus;//Flag utilizado para saber cuando salir del programa. Si es 0 se debe salir del programa.
}gameStatus_t;

typedef struct {//Este struct se utiliza para obtener la entrada del usuario.

	int x;
	int y;
	uint8_t press;
} keys_t;

typedef struct {//Este struct contiene la informacion necesaria para ejecutar el juego.

	keys_t * keys;
    object_t** naveUsr;
    object_t** balasUsr;
    level_setting_t* levelSettings;
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
} game_t;

typedef struct{
	int id;             //Identificador del tipo de objeto
    int velocidad;      //Cantidad de distancia que se mueve el objeto
	int ancho;          //Ancho del objeto
	int alto;           //Alto del objeto
	int initLives;      //Vidas iniciales del tipo de objeto
	int shootProb;      //Probabilidad de disparo del tipo de objeto
    int maxBullets;     //Cantidad maxima de balas del escuadron
    int balaID;         //ID del objectType de la bala asociada a este objeto
    char sprite1[MAX_SPRITE_FILE_LENGTH];    //Sprite 1
    char sprite2[MAX_SPRITE_FILE_LENGTH];    //Sprite 2
    char sprite3[MAX_SPRITE_FILE_LENGTH];    //Sprite 3
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


//*************PANTALLAS
enum PANTALLAS { MENU , SAVE_SCORE, START_LEVEL , IN_GAME, DESTROY_LEVEL, QUIT_GAME};//Determinan un valor para cada pantalla

//*************MENUES
enum MENUES_VALUE { MENU_INICIO , MENU_PAUSA, MENU_WON_LEVEL, MENU_LOST_LEVEL, MENU_LEADERBOARD, MENU_LEVELS, START_LEVEL_MENU};//Determinan un valor para cada menu

//*************ESTADO DE PARTIDA
enum GAME_VALUE { WON_LEVEL = 1, LOST_LEVEL, FASTER_ALIENS };//Se utilizan para determinar cuando se gana o pierde una partida.
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
object_t * initAliens(object_t * listAliens, level_setting_t * levelSetting, char * str, ...); //Inicializa la lista completa de aliens usando addObj.                                                     
char moveAlien(level_setting_t*  levelSettings, object_t ** alienList, int* direccion);//Esta funcion es llamada por el thread y es la
                                                                                                     //la encargada de modificar la posicion de los aliens.
void printLista(object_t * lista, char * id);
//*****************BALAS
object_t * initBarreras(level_setting_t * levelSetting, int cantBarreras, int miniBarrerasY, int miniBarrerasX, ...);
object_t * destroyObj(object_t * ListObj, object_t * RipObj);
object_t * moveBala(object_t ** ListBalasEnemy, level_setting_t * levelSetting);
object_t * shootBala(object_t * listaNaves, object_t * listaBalas, level_setting_t * levelSetting);


//*****************USUARIO
void moveNaveUsuario(object_t ** naveUsuario, level_setting_t* levelSettings, int direccion); //Se encarga de actualizar la posicion de la nave del usuari


//*****************COLLIDER
char collider(level_setting_t * levelSettings, object_t ** alienList, object_t ** usrList, object_t ** barrerasList, object_t ** balasEnemigas, object_t ** balasUsr); //Recorre todas las listas para chequear si hubo colisiones
int collision(vector_t balaPos, int balaType, vector_t objectPos, int objectType); //Detecta si hubo colision entre dos objetos.

//*****************OBJTYPES
void imprimirARRAY(void);																							//Muestra el array de tipos de objetos en stdout
objectType_t * getObjType(int id);																					//Devuelve el puntero al tipo de objeto deseado	
int addObjType(int id, int vel, int ancho, int alto, int initLives, int shootProb, int maxBullets, int balaID, char * sprite1, char * sprite2, char * sprite3);	//Añade un tipo de objeto
int delObjType(int id);			
object_t* removeList(object_t* lista);                                                                               //Elimina de heap la lista creada.    																					//Elimina un tipo de objeto

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


//Esta macro devuelve un 1 si es una letra mayuscula
#define CHECK_LETRA(c)  (((c)>='A' && (c)<= 'Z')? 1 : 0)

#define ASCII2HEXA(c)    ( ( (c) >= '0' && (c) <= '9' ) ? (c) - '0' : (c) - 'A' + 0xA )   //Convierte un numero de ascii a hexadecimal.
#define MAXCANTINPUT    27 //Cantidad de letras en el abecedario + el espacio
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


 #endif//spaceLib.h
