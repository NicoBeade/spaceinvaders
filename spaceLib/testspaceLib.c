#include "spaceLib.h"
#include <stdlib.h>
#include <stdio.h>
void iniciarNivel1(void);
int main(void) {

    level_setting_t settings;
    settings.desplazamientoX = 1;
    settings.desplazamientoY = 1;
    settings.disInicialUsrX = 9;
    settings.disInicialUsrY = 1;
    settings.distInicialX = 2;
    settings.distInicialY = 2;
    settings.margenX = 1;
    settings.margenY = 1;
    settings.saltoX = 4;
    settings.saltoY = 3;
    settings.xMax = 15;
    settings.xMin = 0;
    settings.yMax = 15;
    settings.yMin = 0;
    #define NICOLAS 1
    #define PABLO 2
    #define DANIEL 3
    #define BALANICOLAS 4
    #define BALAPABLO 5
    #define BALADANIEL 6
    addObjType(NICOLAS, 1, 3, 2, 1, 30, 4, BALANICOLAS);
    addObjType(PABLO, 1, 3, 2, 2, 40, 4, BALAPABLO);
    addObjType(DANIEL, 1, 3, 2, 3, 50, 4, BALADANIEL);
    addObjType(BALANICOLAS, 1, 1, 3, 1, 0, 0, NONEOBJTYPEID);
    addObjType(BALAPABLO, 1, 1, 3, 1, 0, 0, NONEOBJTYPEID);
    addObjType(BALADANIEL, 1, 1, 3, 1, 0, 0, NONEOBJTYPEID);
    imprimirARRAY();
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
    object_t* balas = NULL;
    shootBala(listAlien,balas,&settings);
    object_t* pruebab = balas;
    while(pruebab != NULL){
        printf("Bala %d: x: %d ; y: %d ; tipo: %d ; vidas: %d\n", i, prueba -> pos.x, prueba -> pos.y, prueba -> type, prueba -> lives);
        i++;
        prueba = prueba -> next;
    }
}

void iniciarNivel1(void){

}