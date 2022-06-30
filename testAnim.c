#include "sprites.h"
#include "disdrv.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


void printLetter(caracteres_t letter){
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

void printFullDisp(fullDisp_t displaySprite){
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


/*int main (){
    disp_init();
    int i;
    for (i=0;i<42;i++){
        disp_clear();
        printLetter(*(*(alfabeto+i)));
        usleep(500*1000);
    }
    return 0;

}*/ //main para mostrar letras

int main (){
    disp_init();
    printFullDisp(alienSpaceInvaders1);
    disp_update();
}