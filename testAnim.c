#include "sprites.h"
#include "disdrv.h"
#include "utilidades.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern int velDispAnimation;

void printLetter(caracteres_t letter, char xInicial, char yInicial){ //imprime una letra barriendo los 32 pixeles de una matriz de 8X4
    int i,j;
    dcoord_t punto;
    for (i = yInicial ; i < 8 + yInicial ; i++){
        for (j = xInicial ; j < 4 + xInicial ; j++){
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


void* textAnimMenu(char*msg, halfDisp_t* lowerDispMenu, int direccion){
    
    int i,j, offset;
    int dim = strlen(msg+4);
    
    //Primero imprimimos las primeras 4 letras.
    for(i = 0 ; i < 4 ; i ++){

        usleep(10 * U_SEC2M_SEC * velDispAnimation);

        offset = offsetAlfabeto(msg[i]);  
        swipeCharacter   
    }   
    //strcpy()

    //para rleferenciar la ultima columna que hay que copiar en el buffer
    //Idea: voy a tener un contador que contara la columna maxima del texto que se esta imprimiendo
    //Ese contador va a ir de 0 a strlen(msg)*4-1 
    //Para cada valor del contador tendra que llenar el buffer completo. Para esto se usaran dos funciones auxiliiares
    //  -zeroBuffer, recibe la columna del buffer y le carga 0s
    //  -letterBuffer, recibe la columna del buffer y la columna de la letra, y la carga

    //Como cada letra mide 4 columnas, para saber en que columna se encuentra se hace /4
    //Para indicar que columna de la letra se tiene que cargar en el buffer, se hace un %4

    //Para cada valor del contador, se debe correr un for que copie en el buffer 16 columnas
    //Este nuevo contador debe ir desde i-15 hasta i asi se copian 16 columnas en el buffer

    for(i=0; i<strlen(msg)*4 ; i++){




        
        
    }


}

int offsetAlfabeto(char caracter){
//Esta funcion se utiliza para obtener el offset necesario para acceder al string "alfabeto".
    int offset;
    
    if(caracter >= 'a' && caracter <= 'z'){//Si es una letra minuscula.
        offset = caracter - 'a' + OFFSETLETRA;
    }
    else if(caracter >= 'A' && caracter <= 'Z'){//Si es una letra mayuscula.
        offset = caracter - 'A' + OFFSETLETRA;
    }
    else if(caracter >= '0' && caracter <= '9'){//Si es un numero.
        offset = caracter - '0' + OFFSETNUM;
    }
    else if(caracter == '-'){//Si es un guion.
        offset = OFFSETCHARESP;
    }
    else if(caracter == '.'){//Si es un punto.
        offset = OFFSETCHARESP + 1;
    }
    else if(caracter == ' '){//Si es un espacio.
        offset = OFFSETCHARESP + 2;
    }
}

void swipeCharacter(halfDisp_t* lowerDispMenu, caracteres_t caracter, int direccion){
//Esta funcion agrega un caracter completo al buffer
    int fil, col;

    for(fil = 0 ; fil < 8 ; fil++){//Copia cada fila y columna del sprite del caracter en el buffer.
        for(col = 0 ; col < 4 ; col++){
            buffer[fil][col + xInicial] = caracter[fil][col];
        }
    }
}