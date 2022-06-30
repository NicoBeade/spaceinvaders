#include "sprites.h"
#include "disdrv.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void printLetter(caracteres_t letter){ //imprime una letra barriendo los 32 pixeles de una matriz de 8X4
    int i,j;
    dcoord_t punto;
    for (i=0; i<8; i++){
        for (j=0; j<4; j++){
            punto.x=j;
            punto.y=i;
            if (letter [i][j]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    disp_update();
}

void printFullDisp(fullDisp_t displaySprite){ //imprime toda la pantalla barriendo los 256 pixeles de una matriz de 16x16
    int i,j;
    dcoord_t punto;
    for (i=0; i<16; i++){
        for (j=0; j<16; j++){
            punto.x=j;
            punto.y=i;
            if (displaySprite [i][j]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    disp_update();
}

void printHalfDisp(halfDisp_t halfDispSprite, char mitad){ //imprime la mitad de la pantalla barriendo los 128 pixeles de una matriz
    int i,j,offset;                                        //se puede elegir cual mitad del display ingresando I (inf)o S (sup)
    if (mitad=='I'){
        offset=8;
    }
    else if(mitad=='S'){
        offset=0;
    }
    else {
        printf("Letra no valida para la impresion de medio display\n");
        return;
    }
    dcoord_t punto;
    for (i=0; i<8; i++){
        for (j=0; j<16; j++){
            punto.x=j;
            punto.y=i+offset;
            if (halfDispSprite [i][j]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    disp_update();
}


void swipeTextDisp(char*msg){
    uint8_t buffer [8][16];
    int i,j;
    int dim = strlen(msg+4);
    strcpy()

    //para referenciar la ultima columna que hay que copiar en el buffer
    //Idea: voy a tener un contador que contara la columna maxima del texto que se esta imprimiendo
    //Ese contador va a ir de 0 a strlen(msg)*4-1 
    //Para cada valor del contador tendra que llenar el buffer completo. Para esto se usaran dos funciones auxiliiares
    //  -zeroBuffer, recibe la columna del buffer y le carga 0s
    //  -letterBuffer, recibe la columna del buffer y la columna de la letra, y la carga

    //Como cada letra mide 4 columnas, para saber en que columna se encuentra se hace /4
    //Para indicar que columna de la letra se tiene que cargar en el buffer, se hace un %4

    //Para cada valor del contador, se debe correr un for que copie en el buffer 16 columnas
    //Este nuevo contador debe ir desde i-15 hasta i asi se copian 16 columnas en el buffer

    for(i=0; i<strlen(msg)*4; i++){




        
        
    }







}