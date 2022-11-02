//Pruebas de la impresion del puntaje en la raspi, barrido de letras etc



#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "displayRaspi.h"
#include <stdio.h>



caracteres_t alienChar1 ={
    {0,0,1,0},
    {0,0,0,1},
    {0,0,1,1},
    {0,1,1,0},
    {1,1,1,1},
    {1,0,1,1},
    {1,0,1,0},
    {0,0,0,1},
};

caracteres_t alienChar2 ={
    {0,0,0,0},
    {0,0,0,1},
    {1,1,1,1},
    {1,1,1,0},
    {1,1,1,1},
    {1,1,1,1},
    {0,0,0,0},
    {1,0,1,1},
};

caracteres_t alienChar3 ={
    {1,0,0,0},
    {0,0,0,0},
    {1,0,0,0},
    {1,1,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
};

caracteres_t letraA ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraB ={ 
    {0,0,0,0},
    {1,1,0,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraC ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraD ={
    {0,0,0,0},
    {1,1,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraE ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraF ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraG ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraH ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraI ={
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraJ ={
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraK ={
    {0,0,0,0},
    {1,0,0,0},
    {1,0,1,0},
    {1,1,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraL ={
    {0,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraM ={
    {0,0,0,0},
    {1,1,1,0},
    {1,1,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraN ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraO ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraP ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraQ ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraR ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,1,0,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraS ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraT ={
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraU ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraV ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraW ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,1,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraX ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,1,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraY ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraZ ={
    {0,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,1,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra0 ={
    {0,0,0,0},
    {0,1,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra1 ={
    {0,0,0,0},
    {0,1,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra2 ={
    {0,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra3 ={
    {0,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra4 ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra5 ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,0,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra6 ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra7 ={
    {0,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra8 ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra9 ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraGuion ={
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraPunto ={
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraEspacio ={
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t flechaScore ={
    {0,0,0,0},
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,1,0,0},
    {1,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t* alfabeto [43] =  {&letraA,&letraB,&letraC,&letraD,&letraE,&letraF,&letraG,&letraH,
&letraI,&letraJ,&letraK,&letraL,&letraM,&letraN,&letraO,&letraP,&letraQ,&letraR,&letraS,&letraT,
&letraU,&letraV,&letraW,&letraX,&letraY,&letraZ,&letra0,&letra1,&letra2,&letra3,&letra4,&letra5,
&letra6,&letra7,&letra8,&letra9,&letraGuion,&letraPunto,&letraEspacio,&alienChar1,&alienChar2,&alienChar3,&flechaScore};


#define FIRSTROW(a) (((a)==1)? 0: 7)

//**********************************************************************************************************************
//**********************************************************************************************************************
//**********************************************************************************************************************
//**********************************************************************************************************************
//**********************************************************************************************************************
//**********************************************************************************************************************

void barridoLetra(char, char, int sentido, dcoord_t);
void printLetra(caracteres_t);
void copyMatrix(char,uint8_t [8][4]);
int offsetAlfabeto(char );
void sweepMatrix(uint8_t [8][4], int );
void addRow(uint8_t [8][4], uint8_t [4], int );


int main (){
    dcoord_t coordenada = {0,0};
    barridoLetra('f','e',-1,coordenada);
    return 0;
}


void barridoLetra (char letraUno, char letraDos, int sentido, dcoord_t coordenada ){
    int i;
    uint8_t matriz [8][4];
    uint8_t matrizCopy [8][4];
    copyMatrix(letraUno, matriz);
    copyMatrix(letraDos, matrizCopy);
    printLetra(matriz);
    usleep(2000000);
    for (i = 0; i<8 ; i++){
        sweepMatrix(matriz, sentido);
        addRow(matriz,matrizCopy[FIRSTROW(sentido)+i*sentido],sentido);
        printLetra(matriz);
        usleep(2000000);
    }
    return;
}

void addRow(uint8_t matriz [8][4], uint8_t rowToAdd [4], int sentido){
    int i;
    for (i=0; i<4; i++){
        matriz[FIRSTROW(-sentido)][i]=rowToAdd[i];
    }
    return;
}

void sweepMatrix(uint8_t matriz [8][4], int sentido){
    int i,j;
    for (j=0; j<7; j++){
        for(i=0; i<4; i++){
            matriz[FIRSTROW(sentido)+j*sentido][i]= matriz[FIRSTROW(sentido)+sentido+j*sentido][i];
            //matriz[j][i]= matriz[j+1][i];
        }
    }
}

void copyMatrix(char letra, uint8_t matriz [8][4]){
    int i,j;
    for (j=0; j<8; j++){
        for(i=0; i<4; i++){
            matriz[j][i]= (*(alfabeto[offsetAlfabeto(letra)]))[j][i];
        }
    }

    return;
}

void printLetra(caracteres_t letra){
    int i,j;
    for(j = 0; j<8; j++){
        for(i = 0; i<4 ; i++){
            printf("%s", letra[j][i]?"██":"  ");
        }
        printf("\n");
    }
    printf("\n\n");
    return;
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
    else if(caracter == '>'){//Si es un espacio.
        offset = OFFSETCHARESP + 6;
    }
    return offset;
}
