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


/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/
typedef struct{//Este es el tipo de dato que recibe el thread de moveAlien
    object_t ** alien;//Necesita un puntero al primer elemento de la lista de los aliens.
    int desplazamientoX;//Y cuanto se deben mover las naves en cada tick.
    int desplazamientoY;
    int xMax;//Coordenadas maximas del display.
    int margenX;//Margen maxiom al que pueden llegar las naves respecto del borde del display.
    int yMax;
    int margenY;
    int tamAlienX;//Hitbox del alien en la coordenada X.
	object_t ** listBalaAliens;//Puntero a la lista de las balas de los aliens.
	level_setting_t * levelSettings;//Puntero a las settings del nivel.
}argMoveAlien_t;

typedef struct{
	object_t ** balasUsr;
	object_t ** balasEnemigas;
	int velocidadPablo;
	int velocidadDaniel;
	int velocidadNicolas;
	int velocidadUsr;
	int yMax;
	int yMin;
}argMoveBala_t;

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

typedef struct{//Datos necesarios para iniciar un nivel.
    int xMin;                   //-xMax: coordenada maxima en x alcanzable.
    int xMax;                   //-xMin: coordenada minima en x alcanzable.
    int yMin;                   //-YMax: coordenada maxima en Y alcanzable.
    int yMax;                   //-YMin: coordenada minima en Y alcanzable.
    int saltoX;                 //-saltoX: distancia entre naves en x
    int saltoY;                 //-saltoy: distancia entre naves en y (linea)
    int maxUsrBullets;          //-maxUsrBullets: cantidad maxima de balas de la nave del usuario concurrentes
    int maxEnemyBullets;        //-maxEnemyBullets: cantidad maxima de balas enemigas concurrentes
    int shootProbDani;          //-shootProbDani: probabilidad de disparo de naves Daniel en cada tick en procentaje
    int shootProbPablo;         //-shootProbPablo: probabilidad de disparo de naves Pablo en cada tick en procentaje
    int shootProbNico;          //-shootProbNico: probabilidad de disparo de naves Nicolas en cada tick en procentaje
    int distInicialX;           //-distInicialX: coordenada en X de la nave del centro de la primera fila
    int distInicialY;           //-distInicialY: coordenada en Y de la nave del centro de la primera fila
    int barreraInicialX;        //-barreraInicialX: coordenada en X de la primera barrera
    int barreraInicialY;        //-barreraInicialY: coordenada en Y de la primera barrera
    int initUsrLives;           //-initUsrLives: Vidas del usuario en ese nivel
    int initDanielLives;        //-initUsrLives: Vidas de la nave enemiga Daniel en ese nivel
    int initPabloLives;         //-initUsrLives: Vidas de la nave enemiga Pablo en ese nivel
    int initNicolasLives;       //-initUsrLives: Vidas de la nave enemiga Nicolas en ese nivel
    int anchoUsr;               //-anchoUsr: Ancho de la nave del usuario
    int altoUsr;                //-altoUsr: Alto de la nave del usuario
    int anchoNave;              //-anchoNave: Ancho de las naves enemigas
    int altoNave;               //-altoNave: Alto de las naves enemigas
    int anchoMiniBarrera;       //-anchoMiniBarrera: Ancho de las minibarreras
    int altoMiniBarrera;        //-altoMiniBarrera: Alto de las minibarreras
    int miniBarreraLives;       //-miniBarreraLives: Vidas de cada minibarrera
}level_setting_t;

typedef struct 
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

//*************TIPOS DE OBJETOS

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
object_t* addObj(object_t * firstObj, vector_t setPos, types_t setType, int setLives);   //Agrega un objeto a la lista
object_t * initAliens(object_t * listAliens, level_setting_t * levelSetting, char * str, ...); //Inicializa la lista completa de aliens usando addObj.
void removeAlienList(object_t* listAlien);                                               //Elimina de heap la lista creada.

void * moveAlien(void* alien);                                                                //Se encarga de modificar la posicion de los aliens.

//*****************BALAS
object_t * initBarreras(level_setting_t * levelSetting, int cantBarreras, int miniBarrerasY, int miniBarrerasX, ...);
object_t * destroyObj(object_t * ListObj, object_t * RipObj);
object_t * moveBala(object_t * ListBalasEnemy, int BalaType, int yMax, int yMin, int velocity);
object_t * shootBala(object_t * listaNaves, object_t * listaBalas, level_setting_t * levelSetting);
void * moveBalaThread(void * argMoveBala);
//*****************USUARIO
void moveNaveUsuario(object_t * naveUsuario, int desplazamiento, int xMax, int tamAliensX); //Se encarga de actualizar la posicion de la nave del usuari

void * moveBalaThread(void * argMoveBala);
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


/*******************************************************************************************************************************************
 * 
                                             ___         _                           
                                            | __| __ __ | |_   ___   _ _   _ _    ___
                                            | _|  \ \ / |  _| / -_) | '_| | ' \  (_-<
                                            |___| /_\_\  \__| \___| |_|   |_||_| /__/                                                                                                                      
 * 
 ******************************************************************************************************************************************/

extern unsigned int timerTick;   //Variable del timer utilizada para saber cuando se deben ejecutar los threads.
extern int velAliens;   /*Determina que tan rapido se moveran los aliens. La conversion es: si velAliens = 1, entonces moveAlien se ejecuta cada 10mS
                                                                        Para ejecutar velAliens cada 1s velAliens debe valer 100.*/
extern int velBalas;	//Velocidad a la que se mueven las balas.
extern sem_t semaforo;
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


 #endif//spacelib.h
