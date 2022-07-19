#include "sprites.h"
#include "disdrv.h"
#include "utilidades.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

extern int velDispAnimation = 100;

int offsetAlfabeto(char caracter);
void swipeCharacter(halfDisp_t* lowerDispMenu, caracteres_t caracter, int direccion);

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

typedef struct{

    char* msg;//Mensaje a mostrar
    halfDisp_t* lowerDispMenu;//Contenido de la parte inferior del display.
    int direccion;//Direccion en la que se debe mostrar el primer barrido.
    int changeAnimation;//Indica cuando salir del thread.
}argTextAnimMenu_t;


void* textAnimMenu(void* argTextAnimMenu){
    
    int i,j, offset;
    
    //Primero imprimimos las primeras 4 letras.
    for(i = 0 ; i < 4 ; i ++){

        offset = offsetAlfabeto((((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[i]);
        swipeCharacter(((argTextAnimMenu_t*)argTextAnimMenu) -> lowerDispMenu, *(alfabeto[offset]), ((argTextAnimMenu_t*)argTextAnimMenu) -> direccion);
        printf("Caracter: %c\n", (((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[i]);
        int x;
        for(x = 0 ; x < 8 ; x++){
            printf("%c\n", (*(((argTextAnimMenu_t*)argTextAnimMenu) -> lowerDispMenu)) [x][15]);
        }
    }   
    usleep(500 * U_SEC2M_SEC);//Espera medio segundo.
    

    do{//Barre el texto hasta que se le indique lo contrario.
        for(j = i ; (((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[j] != '\0' ; j++){//Barre todas las letras del texto.

            offset = offsetAlfabeto((((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[j]);
            swipeCharacter(((argTextAnimMenu_t*)argTextAnimMenu) -> lowerDispMenu, *(alfabeto[offset]), IZQUIERDA);
        }
        i = 0;//Reinicia el proceso.
    }
    while(((argTextAnimMenu_t*)argTextAnimMenu) -> changeAnimation);

    pthread_exit(0);
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
    return offset;
}

void swipeCharacter(halfDisp_t* lowerDispMenu, caracteres_t caracter, int direccion){
//Esta funcion agrega un caracter completo al buffer.
    int i, fil, col, colInicialB, colFinalB, colInicialL, colFinalL;
    
    colInicialB = (direccion == DERECHA) ? 14 : 1;//Define a partir de que columna se realiza el barrido.
    colFinalB = (direccion == DERECHA) ? -1 : 16;//Define la ultima columna que se barrera.
    colInicialL = (direccion == DERECHA) ? 3 : 0;//Define a partir de que columna se muestra la nueva letra.
    colFinalL = (direccion == DERECHA) ? -1 : 4;//Define a la ultima columna de la letra que se mostrara.

    for(i = colInicialL ; i != colFinalL ; i -= direccion){//Recorre todas las columnas del caracter.

        for(col = colInicialB ; col != colFinalB ; col -= direccion){//Barre el display hacia un costado.
            usleep(velDispAnimation * U_SEC2M_SEC);//Indica a que velocidad se debe hacer el barrido.
            
            for(fil = 0 ; fil < 8 ; fil++){
                (*lowerDispMenu)[fil][col + direccion] = (*lowerDispMenu)[fil][col];//Realiza el barrido.
            }
        }

        for(fil = 0 ; fil < 8 ; fil++){//Ingresa la columna de la nueva letra al buffer.
            (*lowerDispMenu)[fil][colFinalB + direccion] = caracter[fil][i];
        }
        printHalfDisp(*lowerDispMenu, 'I');//Muetra el contenido en el display.
    }
    
}


int main(void){

    disp_init();
    disp_clear();

    pthread_t testAnimT;

    halfDisp_t lowerDispMenu = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    argTextAnimMenu_t argTextAnimMenu = { "Nashe    ",  &lowerDispMenu, IZQUIERDA, 1};

    pthread_create(&testAnimT, NULL, textAnimMenu, &argTextAnimMenu);
    printf("Se creo el thread\n");

    pthread_join(testAnimT, NULL);

    return 0;
}