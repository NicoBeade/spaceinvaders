 #ifndef ALLEGRO_H
 #define ALLEGRO_H

typedef int (*option_t)(void);

typedef struct TEXTO
{
    char* texto;
    int posx;
    int posy;

    struct TEXTO * next;
}texto_t;

typedef struct {//Este struct contiene la informacion necesaria para ejecutar un menu.

	keys_t * keys;
	option_t selectOption[10];//Struct que contiene punteros a funciones que indican que hacer cuando se selecciona una opcion.
    char* textOpciones[10];//Arreglo de punteros a los strings que contienen el texto de cada opcion.
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

//allegroMenu: Esta funcion se encarga de preparar las listas para mostrar un menu en pantalla
//Como segundo parametro debe recibir el puntero a la lista de textos de allegro
texto_t * allegroMenu(menu_t * data, texto_t * toshow);

//changeOption: Esta funcion se encarga de ejucutar la animacion de cambiar de opcion en el menu
void changeOption(void * data);

/******************************************************************************************************/
typedef struct {

    object_t ** alienList;
    object_t ** UsrList;
    object_t ** barrerasList;
    object_t ** balasUsr;
    object_t ** balasAlien;
    object_t ** mothershipList;

}punteros_t;

//Struct de datos para el thread de allegro
typedef struct ALLEGRO {

    punteros_t punteros;
    texto_t ** textToShow;
    keys_t * keys;

}data_allegro_t;

//protoripo de thread principal de allegro
void * allegroThread (void * arg);

int selectPlayInicio(void);
int selectLevels(void);
int selectVolume(void);
int selectQuitGame(void);
int selectResume(void);
int selectRestartLevel(void);
int selectMainMenu(void);
int selectDificulty(void);
/*******************************************************************************************************/

#endif