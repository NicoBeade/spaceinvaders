#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"

static int locateInLeaderboard(leaderboard_t, int score); //localiza la fila en la que debe ir el puntaje
static int detectThreeLetter(char*); //AUX. detecta si el nombre ingresado tiene mas de tres letras
void printLeaderboard(leaderboard_t); //AUX. imprime el arreglo parseado de scores


/*int main (void){
    leaderboard_t leaderboard ; //array de punteros a string
    int score = 198;
    char* name = "fac";

    parseScore(leaderboard);
    addScore(leaderboard, score, name);
    //printLeaderboard(leaderboard);
    saveScore(leaderboard);

    return 0;
}*/

void parseScore(leaderboard_t leaderboard){ 
    //parsea el archivo del score almacenandolo en un array de strings 
    //necesita que antes de invocarla de haya creado una char leaderboard [LEADERBOARD][ROWSIZE]
    FILE* file = fopen(FILENAME_SCORE, "r");
    if (file == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    int fileRow;
    char buffer [15];
    for (fileRow = 0; fileRow<LEADERBOARD; fileRow++){    //para cada linea de las 10 del leaderboard copia el contenido
        fgets(buffer,15,file);                            //y le recorta el \n, caracter final no deseado en cada linea.
        strcpy(leaderboard[fileRow],buffer);
        strtok(leaderboard[fileRow],"\n");
    }
    fclose(file);
}

void saveScore(leaderboard_t leaderboard){ //sobreescribe el archivo del puntaje con la nueva tabla
    FILE* file = fopen(FILENAME_SCORE, "w");
    if (file == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    char scoreToPrint [ROWSIZE];

    for (int i = 0; i<LEADERBOARD; i++){ //para cada jugador del top, imprime una linea con la info correspondiente
        strcpy(scoreToPrint,leaderboard[i]);
        strcat(scoreToPrint,"\n");
        fputs(scoreToPrint,file);
    }

    fclose(file);

    return;
}

int addScore(leaderboard_t leaderboard, int score, char* name){ //anyade un puntaje a la tabla
    char scoreToAdd [ROWSIZE];
    char aux [ROWSIZE];
    sprintf(scoreToAdd, "%d %s", score, name); //guarda en un string appendeado el puntaje y el nombre
    int posInTop;
    

    if(detectThreeLetter(name)){ //detecta si el nombre tiene 3 letras. Si no, no guarda el score
        printf("Error en addScore. Nombre de mas de 3 letras\n");
        return 1;                   
    }

    posInTop=locateInLeaderboard(leaderboard, score); //guarda la posicion en la que se debe anyadir el puntaje nuevo


    if(posInTop==-1){ //si la posicion es -1 significa que no entro en el top del leaderboard
        return 1;
    }

    int i;

    for(i = posInTop; i<LEADERBOARD; i++){ //para cada lugar de la tabla desde la posicion encontrada, se agrega el nuevo score
        strcpy(aux,leaderboard[i]);        //y se desplaza al resto. El ultimo puntaje de la tabla queda almacenado en un aux (que se pierde)
        strcpy(leaderboard[i],scoreToAdd);
        strcpy(scoreToAdd,aux);
    }
    
    return 0; //devuelve 0 si se agrego sin problemas el nuevo puntaje
}

static int locateInLeaderboard(leaderboard_t leaderboard, int score){
    int posInTop;
    int located = 0;
    int scoreInLeaderboard;
    char stringAux [ROWSIZE];
    
    for (posInTop = 0; posInTop<LEADERBOARD && located==0; posInTop++){
        strcpy(stringAux,leaderboard[posInTop]);
        strtok(stringAux," ");         //se queda nada mas con los numeros de tal renglon
        scoreInLeaderboard = atoi(stringAux);

        if(scoreInLeaderboard<score){              //compara el score con el score del leaderboard
            located = 1;                           //si el score a agregar es mayor al comparado entonces se localizo la posicion
        }                              
    }

    if(located == 0){
        printf("No se encontro posicion en el top\n");
        return -1;
    } 

    return posInTop-1;
}

static int detectThreeLetter(char* name){        
    //Detecta si las primeras tres letras del puntero son distintas de 0 y la cuarta es 0   
    
    if (!*name || !*(name+1) || !*(name+2) || *(name+3)){
        return 1;
    }

    return 0;
}

void printLeaderboard(leaderboard_t leaderboard){
    for (int i = 0; i<10; i++){
        printf("%s\n", leaderboard[i]);
    }

    return;
}











        
