#include "spaceLib.h"
#include <stdlib.h>
void iniciarNivel1(void);
int main(void) {

    level_setting_t settings;
    settings.desplazamientoX = 1;
    settings.desplazamientoY = 1;
    settings.disInicialUsrX = 7;
    settings.disInicialUsrY = 13;
    settings.distInicialX = 2;
    settings.distInicialY = 2;
    settings.margenX = 1;
    settings.margenY = 1;
    settings.saltoX = 1;
    settings.saltoY = 3;
    settings.xMax = 15;
    settings.xMin = 0;
    settings.yMax = 15;
    settings.yMin = 0;
    #define NICOLAS 1
    #define PABLO 2
    #define DANIEL 3
    addObjType(NICOLAS, 1, 3, 2, 1, 30, 4);
    addObjType(PABLO, 1, 3, 2, 2, 40, 4);
    addObjType(DANIEL, 1, 3, 2, 3, 50, 4);
    printf("pepe\n");
    object_t* listAlien = NULL;//Puntero al primer elemento de la lista de los aliens.
    listAlien = initAliens(listAlien, &settings, "20403", NICOLAS, PABLO, DANIEL);
    printf("george\n");
//************************************* Esta seccion del codigo se usa para probar que funcionen las listas *****************************
    int i = 1;
    object_t* prueba = listAlien;
    while(prueba != NULL){
        printf("Alien %d: x: %d ; y: %d ; tipo: %d ; vidas: %d\n", i, prueba -> pos.x, prueba -> pos.y, prueba -> type, prueba -> lives);
        i++;
        prueba = prueba -> next;
    }
}

void iniciarNivel1(void){

}