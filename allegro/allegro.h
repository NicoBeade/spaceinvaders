typedef int (*option_t)(void);

typedef struct {//Este struct contiene la informacion necesaria para ejecutar un menu.

	keys_t * keys;
	option_t selectOption[10];//Struct que contiene punteros a funciones que indican que hacer cuando se selecciona una opcion.
    char* textOpciones[10];//Arreglo de punteros a los strings que contienen el texto de cada opcion.
    int cantOpciones;//Cantidad de opciones del menu.
    void (*changeOption)(void* argChangeOption);//Callback a la funcion que cambia la opcion seleccionada.
} menu_t;

typedef struct{

    texto_t ** toText;
    int actualOp;
    int nextOp;
    menu_t * menu;

} changeOptionData_t;

//allegroMenu: Esta funcion se encarga de preparar las listas para mostrar un menu en pantalla
//Como segundo parametro debe recibir el puntero a la lista de textos de allegro
texto_t * allegroMenu(menu_t * data, texto_t * toshow);

//changeOption: Esta funcion se encarga de ejucutar la animacion de cambiar de opcion en el menu
void changeOption(void * data);

/******************************************************************************************************/
//Struct de datos para el thread de allegro
typedef struct ALLEGRO {

    object_t ** nave;
    object_t ** aliens;
    object_t ** balas;
    texto_t ** textToShow;
    keys_t * keys;

} data_allegro_t;

//protoripo de thread principal de allegro
void * allegroThread (void * arg);
/*******************************************************************************************************/