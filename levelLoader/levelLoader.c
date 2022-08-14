#include <stdio.h>
#include <stdint.h>
#include "levelLoader.h"

#define ISNUM(caracter) (((caracter) > '0' ) && ((caracter) < '9' ))

enum paramType {ENTERO, ARCHIVO, BINARIO};      //Identifica el tipo de valor que tiene un parametro
enum estados {PARAM, VALUE, SPACE};


typedef struct{
    char parameter[MAX_PARAM_LETTERS];      //String del parametro
    char value[MAX_VALUE_LETTERS];          //String del valor asociado
}lineaArchivo_t;   

static lineaArchivo_t decodedFile[MAX_FILE_ROWS];       //Array de parametros+valores de todo el archivo

int readFile(char * file){       //Funcion leer archivo, recibe la direccion
    FILE * filePointer;     //Se crea un puntero al archivo
    if(file == NULL){       //Si la direccion es nula, devuelve error
        printf("Error in levelLoader.c: file cannot be null");
        return -1;
    }
    else{               //Si se ingreso una direccion valida, se intenta abrir el archivo
        filePointer = fopen(file, "r");
    }
    if(filePointer == NULL){        //Si no se pudo abrir, se devuelve error
        printf("Error in levelLoader.c: \"%s\" file open failed", file);
        return -1;
    }
    else{           //Si se puede abrir, se lee el archivo
        clearFileBuffer;    //Se limpia el buffer del archivo
        int linea;  //Contador de lineas de archivo
        char lineaStr[MAX_FILE_ROW_LENGHT] = {};       //Variable auxiliar para almacenar las lineas del archivo
        void * fgetsReturn = file;     //Se inicializa una variable para recibir el parametro de salida de fgets
        for(linea = 0; fgetsReturn != NULL && linea<MAX_FILE_ROWS; linea++){        //Mientras el retorno de fgets no sea un EOF y no se haya exedido el max de lineas, se lee el archivo
            fgetsReturn = fgets(lineaStr, MAX_FILE_ROW_LENGHT,filePointer);       //Se copia la linea a lineaStr
            if(fgetsReturn){    //Si no es el fin del archivo EOF
                if(lineaStr[MAX_FILE_ROW_LENGHT-1] != 0 && lineaStr[MAX_FILE_ROW_LENGHT-1] != '\n' && lineaStr[MAX_FILE_ROW_LENGHT-1] != '\r'){        //Si se paso la cantidad de lineas devuelve error
                    printf("Error in levelLoader.c: \"%s\" too many letters in the %d row", file, linea);
                    return -1;
                }
                int letra = 0;          //Se crea variable auxiliar letra
                int arrayIndex = 0;     //Se crea variable auxiliar arrayIndex
                char caracter = lineaStr[0];          //Se crea variable auxiliar caracter
                int state = PARAM;      //Se crea variable auxiliar state
                //int comillas;           //Variable auxiliar comillas
                while(caracter != 0){    //Mientras no sea un un 0
                    switch(state){
                        case PARAM:
                            if(lineaStr[letra+1] == 0 || lineaStr[letra+1] == '\n' || lineaStr[letra+1] == '\r'){
                                decodedFile[linea].value[arrayIndex] = 0;
                                arrayIndex++;
                            }
                            if(caracter == ' '){
                                decodedFile[linea].parameter[arrayIndex] = 0; //Agrega el terminador
                                arrayIndex = 0;
                                state = SPACE;
                            }
                            else{
                                decodedFile[linea].parameter[arrayIndex] = caracter;
                                arrayIndex++;
                            }
                            break;
                        case VALUE:
                            if(lineaStr[letra+1] == 0 || lineaStr[letra+1] == '\n' || lineaStr[letra+1] == '\r'){
                                decodedFile[linea].value[arrayIndex] = 0;
                                arrayIndex++;
                            }
                            if(caracter == ' '){
                                decodedFile[linea].parameter[arrayIndex] = 0; //Agrega el terminador
                                arrayIndex = 0;
                                state = SPACE;
                            }
                            else{
                                decodedFile[linea].value[arrayIndex] = caracter;
                                arrayIndex++;
                            }
                            break;
                        case SPACE:
                            //if(comillas){   //Si encuentra comillas durante la lectura de espacios hubo error
                                //   printf("Error in levelLoader.c: \"%s\", %d row, \"\" not closed properly or space added inside a parameter or value", file, linea);
                                //   return -1;
                            // }
                            if(caracter != ' '){    //Si el caracter es distinto del espacio
                                state = VALUE;  //Se pasa al estado value
                                decodedFile[linea].value[0] = caracter;
                                arrayIndex++;
                            }
                            break;
                    }
                    if(lineaStr[letra+1] == 0 || lineaStr[letra+1] == '\n' || lineaStr[letra+1] == '\r'){
                        caracter = 0;
                    }
                    else{
                        letra++;
                        caracter = lineaStr[letra];
                    }
                }
            }
        }
        if(linea == MAX_FILE_ROWS){
            printf("Error in levelLoader.c: \"%s\" too many lines in the file", file);
            return -1;
        }
    }
    fclose(filePointer);
}

void clearFileBuffer(void){
    int index;   
    int index2;
    int index3;
    for(index = 0; index < MAX_FILE_ROWS; index++){
        for(index2 = 0; index2 < MAX_PARAM_LETTERS; index2++){
            decodedFile[index].parameter[index2] = 0;
        }
        for(index3 = 0; index3 < MAX_VALUE_LETTERS; index3++){
            decodedFile[index].value[index3] = 0;
        }
    }
}

void printFile(void){
    int index;
    for(index = 0; decodedFile[index].parameter[0] != 0; index++){
        printf("PARAMETRO: %s VALOR: %s", decodedFile[index].parameter, decodedFile[index].value);
    }
}

