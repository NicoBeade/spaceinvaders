#include <stdint.h>
#include "disdrv.h"
#include "termlib.h"
#include <unistd.h>
/* 
 Se usaran los siguientes recursos de la libreria del display
    -dcoord_t, tipo de dato: struct con un campo x y un campo y
    -disp_init(void), inicializa el display
    -disp_clear(void), limpia el display
    -disp_write(dcoord_t,dlevel_t), escribe en el display
    -D_OFF,D_ON, ctes de prendido y apagado del display
    -disp_update(void), actualiza el display
*/

typedef uint8_t enemy_t [2][3]; //matriz de 2x3, tamanyo de los enemigos


void drawEnemy(dcoord_t, enemy_t); //prototipos  de dibujar y limpiar enemigos
void cleanEnemy(dcoord_t);

enemy_t daniel1 = {{1,0,1},{1,1,1}}; //2 sprites para cada tipo de enemigo
enemy_t daniel2 = {{1,1,1},{1,0,1}};

enemy_t pablo1 ={{1,1,0},{1,0,1}};
enemy_t pablo2 ={{0,1,1},{1,0,1}};

enemy_t nicolas1 ={{1,0,1},{0,1,0}};
enemy_t nicolas2 ={{0,1,0},{1,0,1}};


void drawEnemy(dcoord_t p, enemy_t alien){ //Esta funcion imprime en display un enemigo en un sprite dados en la posicion p
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<=2 ; i++){
        for(j=0 ; j<=1 ; j++){
            if (alien [j][i] == 1){
                pAux.x = p.x+i;
                pAux.y = p.y+j;
                disp_write(pAux,D_ON); //Actualiza el buffer
            }
        }
    }
    disp_update(); //Se imprime per se
}

void cleanEnemy(dcoord_t p){ //Esta funcion borra en display un enemigo (tienen todos el mismo tamanyo) en la posicion p
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<=2 ; i++){
        for(j=0 ; j<=1 ; j++){
            pAux.x = p.x+i;
            pAux.y = p.y+j;
            disp_write(pAux ,D_OFF); //Actualiza el buffer
        }
    }
    disp_update(); //Borra per se
}



/*int main (void){
    dcoord_t p = {5,5};
    disp_init();
    disp_clear();
    drawEnemy(p,daniel1);
    usleep(500*1000);
    cleanEnemy(p);
    drawEnemy(p,daniel2);
    usleep(500*1000);
    cleanEnemy(p);
    drawEnemy(p,pablo1);
    usleep(500*1000);
    cleanEnemy(p);
    drawEnemy(p,pablo2);
    usleep(500*1000);
    cleanEnemy(p);
    drawEnemy(p,nicolas1);
    usleep(500*1000);
    cleanEnemy(p);
    drawEnemy(p,nicolas2);
    usleep(500*1000);
    cleanEnemy(p);
    return 0;
}
*/ //Ejemplo de main 