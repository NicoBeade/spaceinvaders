 #ifndef ALLEGRO_H
 #define ALLEGRO_H

 #include "../spaceLib/spaceLib.h"

/******************************************************************
 * 
 *                          Structs
 * 
*******************************************************************/
enum SONIDOS {COLISION_ALIEN_TOCADO = 1,COLISION_ALIEN_MUERTO,COLISION_USER_TOCADO,COLISION_USER_MUERTO,COLISION_MOTHERSHIP_MUERTA,COLISION_BARRERA_TOCADA,COLISION_BARRERA_MUERTA,
              MOTHERSHIP_APARECE,
              MOVIMIENTO_ALIENS,
              BALA_USER,BALA_ALIEN,
              SELECT_MENU,SWAP_MENU,ERROR_MENU,SWEEP_LETRA,
              SAVED_SCORE, AUDIOMAX};

enum MUSICA {MUSICA_MENU = AUDIOMAX ,MUSICA_JUEGO, MUSICAMAX};

typedef int (*option_t)(void);

typedef struct TEXTO
{
    char* texto;
    int fuente;
    int posx;
    int posy;

    struct TEXTO * next;
}texto_t;

typedef struct SPRITE
{
    char* direccion;
    int posx;
    int posy;

    struct SPRITE * next;
}sprite_t;


typedef struct
{
    texto_t * textoList;
    sprite_t * spriteList;

}TextObj_t;

//Datos de un menu
typedef struct {//Este struct contiene la informacion necesaria para ejecutar un menu.

	keys_t * keys;
	option_t selectOption[21];//Struct que contiene punteros a funciones que indican que hacer cuando se selecciona una opcion.
    option_t backMenuAnterior;//Puntero a la funcion que vuelve un menu hacia atras.
    char textOpciones[21][50];//Arreglo de punteros a los strings que contienen el texto de cada opcion.
    char spritesDir[40][50];
    int cantOpciones;//Cantidad de opciones del menu.
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
    void (*changeOption)(void* argChangeOption);//Callback a la funcion que cambia la opcion seleccionada.

    void (*audioCallback)(int);
    
}menu_t;

//Datos de changeOption
typedef struct{

    texto_t ** toText; //Lista a textos
    sprite_t ** toSprite; //Lista de sprites
    int actualOp;      //Opcion acutal
    int nextOp;        //Nueva opcion
    menu_t * menu;     //Datos del menu actual

}changeOptionData_t;

//Estes struct se encarga de guardar los punteros a todas las listas
typedef struct {

    object_t ** alienList;
    object_t ** UsrList;
    object_t ** barrerasList;
    object_t ** balasUsr;
    object_t ** balasAlien;
    object_t ** mothershipList;
    sprite_t ** screenObjects;

}punteros_t;

//Struct de datos para el thread de allegro
typedef struct {

    punteros_t punteros;
    texto_t ** textToShow;
    keys_t * keys;

}data_allegro_t;

/******************************************************************
 * 
 *                          Prototipos
 * 
*******************************************************************/

//protoripo de thread principal de allegro
void * allegroThread (void * arg);


//allegroMenu: Esta funcion se encarga de preparar las listas para mostrar un menu en pantalla
//Como segundo parametro debe recibir el puntero a la lista de textos de allegro
TextObj_t * allegroMenu(menu_t * data, TextObj_t * lists);
//changeOption: Esta funcion se encarga de ejucutar la animacion de cambiar de opcion en el menu
void changeOption(void * data);


//allegroScore: Esta funcion se encarga de preparar las listas para mostrar el menu de savescore
TextObj_t * allegroScore(TextObj_t * lists, char* scoreActual, char letras[15][2]);
//changeLetra: Se encarga de realizar la animacion de cambio de letras en el menu de score
void changeLetra(char letras[15][2], int letraActual, int dir);
//changeCol: se encarga de cambiar de columna en el menu de score
//texto_t * changeCol(texto_t * toshow, int nextOp);
sprite_t * changeCol(sprite_t * toshow, int nextOp);

//allegroScore: Esta funcion se encarga de preparar las listas para mostrar el menu de liderboard
TextObj_t * allegroLiderboard(menu_t * data, TextObj_t * lists);

//limpia una lista de textos_t
texto_t * emptyText(texto_t * firstText); 
//añade un texto_t a una lista
texto_t* addText(texto_t * firstObj, char * texto, int fuente, int posx, int posy);

sprite_t* addSprite(sprite_t * firstObj, char * direccion, int posx, int posy);

sprite_t * emptySprite(sprite_t * firstSprite);

void playAudioAllegro(int id);

texto_t * levelAllegro(texto_t * toText, char * score, char * vidas );
void refreshDatos( char * toScore, char * toVidas, int score, int vidas);

#endif