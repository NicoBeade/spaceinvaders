#include "sprites.h"
#include "disdrv.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


void printLetter(caracteres_t* letter){
    int i,j;
    dcoord_t punto;
    for (i=0; i<8; i++){
        for (j=0; j<4; j++){
            punto.x=j;
            punto.y=i;
            if ((*letter) [j][i]==1){
                disp_write(punto,D_ON);
                putchar('*');
            }
            else {
                putchar(' ');
            }
        }
        putchar('\n');
    }
    disp_update();
}


int main (){
    disp_init();
    int i;
    for (i=0;i<42;i++){
        disp_clear();
        printLetter(*(alfabeto+i));
        usleep(500*1000);
    }
    return 0;

}