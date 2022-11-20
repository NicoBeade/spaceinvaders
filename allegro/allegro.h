 #ifndef ALLEGRO_H
 #define ALLEGRO_H

 #include "../spaceLib/spaceLib.h"

/******************************************************************
 * 
 *                          Structs
 * 
*******************************************************************/
enum audios {aUsrDeath, aShoot, audioMax};

typedef int (*option_t)(void);

typedef struct TEXTO
{
    char* texto;
    int posx;
    int posy;

    struct TEXTO * next;
}texto_t;

typedef struct AUDIO
{
    int audioId;

    struct AUDIO * next;
}audio_t;

typedef struct {//Este struct contiene la informacion necesaria para ejecutar un menu.

	keys_t * keys;
	option_t selectOption[21];//Struct que contiene punteros a funciones que indican que hacer cuando se selecciona una opcion.
    char textOpciones[21][50];//Arreglo de punteros a los strings que contienen el texto de cada opcion.
    int cantOpciones;//Cantidad de opciones del menu.
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
    void (*changeOption)(void* argChangeOption);//Callback a la funcion que cambia la opcion seleccionada.
    
}menu_t;

typedef struct{

    texto_t ** toText;
    int actualOp;
    int nextOp;
    menu_t * menu;

}changeOptionData_t;

typedef struct {

    object_t ** alienList;
    object_t ** UsrList;
    object_t ** barrerasList;
    object_t ** balasUsr;
    object_t ** balasAlien;
    object_t ** mothershipList;

}punteros_t;

//Struct de datos para el thread de allegro
typedef struct {

    punteros_t punteros;
    texto_t ** textToShow;
    audio_t ** audioToPlay;
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
texto_t * allegroMenu(menu_t * data, texto_t * toshow);
texto_t * emptyText(texto_t * firstText);
texto_t* addText(texto_t * firstObj, char * texto, int posx, int posy);
audio_t* addAudio(audio_t * firstObj, int audioId);

//changeOption: Esta funcion se encarga de ejucutar la animacion de cambiar de opcion en el menu
void changeOption(void * data);

//Opciones de menu
/**************************************/
int selectPlayInicio(void);
int selectLevels(void);
int selectVolume(void);
int selectQuitGame(void);
int selectResume(void);
int selectRestartLevel(void);
int selectMainMenu(void);
int selectDificulty(void);
/**************************************/

#endif