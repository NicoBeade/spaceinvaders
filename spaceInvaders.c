#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#define RASPI

#ifdef RASPI
#define SIGUIENTE ((*(menu->keys))->x == 1)
#define ANTERIOR  ((*(menu->keys))->x == -1)
#endif

#ifdef ALLEGRO
#define SIGUIENTE ((*(menu->keys))->y == 1)
#define ANTERIOR  ((*(menu->keys))->y == -1)
#endif

#define PRESS     ((*(menu->keys))->press == 1)

typedef struct {//Este struct se utiliza para obtener la entrada del usuario

	uint8_t x;
	uint8_t y;
	uint8_t press;

} keys_t;

keys_t keys;


typedef int (*option_t)(void);//

typedef struct {

	keys_t ** keys;
	option_t selectOption[10];//Struct que contiene punteros a funciones que indican que hacer cuando se selecciona una opcion.
    int cantOpciones;//Cantidad de opciones del menu.
    int exitStatus;//Esta variable se utiliza para saber cuando hay que salir del thread.
    void (*changeOption)(int direccion);//Callback a la funcion que cambia la opcion seleccionada.

} menu_t;



static void* menuHandler(void * data){

	menu_t * menu = (menu_t *) data;

    int select = 0;

    while(menu -> exitStatus){

        if (SIGUIENTE){
            //Animacion
            select += 1;
            if(menu->selectOption[select] == NULL){
                select = 0;
            }
            (menu -> changeOption)(1);
        }

        if (ANTERIOR){
            //Animacion
            select -= 1;
            if(select < 0){
                select = (menu -> cantOpciones) - 1;
            }
            (menu -> changeOption)(-1);
        }

        if (PRESS){
            menu -> exitStatus = (menu->selectOption[select])();
        }
    }
    
    pthread_exit(0);
}

//Reset

    //Salir al nivel reseteado

//Resume

    //Salir al nivel

//Volume

    //Salir al menu de volumen

//Home

    //Salir al menu principal

//Score

    //Mostrar en pantalla el puntaje

/*

    thread (handler)
    struct de los datos de thread
    punteros para mostrar las cosas




static void* levelHandler(void * data){

	menu_t * menu = (menu_t *) data;

	opcion_t * opcionActual = menu->opciones[0];

	if (DISPARAR){
		//Animacion
		disparar();
	}

	if (MOVDERECHA){
		//Animacion
		moverder();
	}

	if (PRESS){
		*opcionActual();
	}

}

*/