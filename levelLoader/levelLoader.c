#include <stdio.h>
#include <stdint.h>
#include "levelLoader.h"
#include "../spaceLib/spaceLib.h"
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define ISNUM(caracter) (((caracter) > '0' ) && ((caracter) < '9' ))

enum paramType {ENTERO, ARCHIVO, BINARIO};      //Identifica el tipo de valor que tiene un parametro
enum estados {START, PARAM, VALUE, SPACE};


typedef struct{
    char parameter[MAX_PARAM_LETTERS];      //String del parametro
    char value[MAX_VALUE_LETTERS];          //String del valor asociado
}lineaArchivo_t;   


static lineaArchivo_t decodedFile[MAX_FILE_ROWS];       //Array de parametros+valores de todo el archivo


int readFile(char * file){       //Funcion leer archivo, recibe la direccion
    FILE * filePointer;     //Se crea un puntero al archivo
    if(file == NULL){       //Si la direccion es nula, devuelve error
        printf("Error in levelLoader.c, readFile function: file cannot be null\n");
        return -1;
    }
    else{               //Si se ingreso una direccion valida, se intenta abrir el archivo
        filePointer = fopen(file, "r");
    }
    if(filePointer == NULL){        //Si no se pudo abrir, se devuelve error
        printf("Error in levelLoader.c, readFile function: \"%s\" file open failed\n", file);
        return -1;
    }
    else{           //Si se puede abrir, se lee el archivo
        clearFileBuffer();    //Se limpia el buffer del archivo
        int lineaFile;  //Contador de lineas de archivo
        int linea = 0;              //Variable auxiliar que cuenta las lineas de parametros (sin ;)
        char lineaStr[MAX_FILE_ROW_LENGHT] = {};       //Variable auxiliar para almacenar las lineas del archivo
        void * fgetsReturn = file;     //Se inicializa una variable para recibir el parametro de salida de fgets
        for(lineaFile = 0; fgetsReturn != NULL && lineaFile<MAX_FILE_ROWS; lineaFile++){        //Mientras el retorno de fgets no sea un EOF y no se haya exedido el max de lineas, se lee el archivo
            fgetsReturn = fgets(lineaStr, MAX_FILE_ROW_LENGHT,filePointer);       //Se copia la linea a lineaStr
            if(fgetsReturn){    //Si no es el fin del archivo EOF
                if(lineaStr[MAX_FILE_ROW_LENGHT-1] != 0 && lineaStr[MAX_FILE_ROW_LENGHT-1] != '\n' && lineaStr[MAX_FILE_ROW_LENGHT-1] != '\r'){        //Si se paso la cantidad de lineas devuelve error
                    printf("Error in levelLoader.c, readFile function: \"%s\" too many letters in the %dth row\n", file, lineaFile+1);
                    return -1;
                }
                int letra = 0;          //Se crea variable auxiliar letra
                int arrayIndex = 0;     //Se crea variable auxiliar arrayIndex
                char caracter = lineaStr[0];          //Se crea variable auxiliar caracter
                int state = START;      //Se crea variable auxiliar state
                if(caracter != ';'){    //Si no es un comentario
                    while(caracter != 0){    //Mientras no sea un un 0 recorre los caracteres de una linea
                        switch(state){      //Dependiendo de en donde se encuentre leyendo (parametro, espacio o valor)
                            case START:     //Si es el primer caracter
                                if(caracter == ' ' || caracter == '\t' || lineaStr[letra] == '\n' || lineaStr[letra] == '\r'){    //Si es un espacio o un enter lo ignora
                                    break;
                                }
                                else{                   //Si no es un espacio se encuentra en el estado lectura de parametro
                                    state = PARAM;
                                }
                            case PARAM:     //Si esta leyendo el parametro
                                if(lineaStr[letra] == 0 || lineaStr[letra] == '\n' || lineaStr[letra] == '\r' || lineaStr[letra] == ';'){   //Si la linea llego a su fin
                                    decodedFile[linea].parameter[arrayIndex] = 0;       //Se guarda el terminador en el array del parametro correspondiente 
                                }
                                if(caracter == ' ' || caracter == '\t'){    //Si encuentra un espacio
                                    decodedFile[linea].parameter[arrayIndex] = 0; //Agrega el terminador
                                    arrayIndex = 0;     //Reinicia el contador que guarda los caracteres en el array
                                    state = SPACE;      //Pasa al estado espacio
                                }
                                else{       //Si no es ninguno de los caracteres finalizadores entonces guarda ese caracter
                                    decodedFile[linea].parameter[arrayIndex] = caracter;        //Guarda el caracter
                                    arrayIndex++;       //Incrementa el contador que almacena caracteres en el array
                                }
                                break;
                            case VALUE:     //Si esta leyendo el valor
                                if(lineaStr[letra] == 0 || lineaStr[letra] == '\n' || lineaStr[letra] == '\r' || lineaStr[letra] == ';'){   //Si la linea  llego a su fin
                                    decodedFile[linea].value[arrayIndex] = 0;       //Se guarda el terminador
                                }
                                if((caracter == ' ' || caracter == '\t') && (lineaStr[letra+1] == ' ' || lineaStr[letra+1] == '\t' || lineaStr[letra+1] == 0 || lineaStr[letra+1] == '\n' || lineaStr[letra+1] == '\r' || lineaStr[letra+1] == ';')){
                                    //Si ve un espacio y el caracter que le sigue es un finalizador de la linea o un espacio, entonces deja de guardar el valor
                                    decodedFile[linea].value[arrayIndex] = 0; //Agrega el terminador
                                    arrayIndex = 0; 
                                    state = SPACE;
                                }
                                else{   //Si no encontro un caso de finalizacion, guarda el caracter en el string de valor
                                    decodedFile[linea].value[arrayIndex] = caracter;
                                    arrayIndex++;   
                                }
                                break;
                            case SPACE:
                                if(lineaStr[letra] == 0 || lineaStr[letra] == '\n' || lineaStr[letra] == '\r' || lineaStr[letra] == ';'){   //Si la linea  llego a su fin
                                    state = START;
                                }
                                if(caracter != ' ' && caracter != '\t'){    //Si el caracter es distinto del espacio estando en el estado espacio
                                    state = VALUE;  //Se pasa al estado value
                                    decodedFile[linea].value[0] = caracter;
                                    arrayIndex++;
                                }
                                break;
                        }
                        if(lineaStr[letra+1] == 0 || lineaStr[letra+1] == '\n' || lineaStr[letra+1] == '\r' || lineaStr[letra+1] == ';'){   //Si el siguiente caracter es un finalizador de la linea, se termina el while
                            caracter = 0;
                        }
                        else{   //Si la linea no finalizo se incrementa el index de la lietra y se lee un nuevo caracter
                            letra++;          
                            caracter = lineaStr[letra]; //Se lee un nuevo caracter
                        }
                        if(arrayIndex >= MAX_PARAM_LETTERS-1 && state == PARAM){ //Si supera la cantidad de caracteres permitidos en el array de parametro devuelve error
                            printf("Error in levelLoader.c, readFile function: \"%s\" too many letters in parameter of the %dth row\n", file, lineaFile+1);
                            fclose(filePointer);
                            return -1;
                        }
                        if(arrayIndex >= MAX_VALUE_LETTERS-1 && state == VALUE){ //Si supera la cantidad de caracteres permitidos en el array de valor devuelve error
                            printf("Error in levelLoader.c, readFile function: \"%s\" too many letters in value of the %dth row\n", file, lineaFile+1);
                            fclose(filePointer);
                            return -1;
                        }
                    }
                    linea++;
                }
            }
        }
        if(lineaFile == MAX_FILE_ROWS){
            printf("Error in levelLoader.c, readFile function: \"%s\" too many lines in the file\n", file);
            return -1;
        }
    }
    fclose(filePointer);
    return 0;
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
        printf("PARAMETRO: %s VALOR: %s\n", decodedFile[index].parameter, decodedFile[index].value);
    }
}

int loadAsset(char * file){
    if(readFile(file) != 0){ //Si no se pudo leer correctamente
        return -1;      //Devuelve error
    }
    int paramNo;        //Variable que cuenta los parametros

    int id;
    int vel;
    int ancho;
    int alto;
    int initLives;
    int shootProb;
    int maxBullets;
    int balaID;
    char id_found = 0;
    char vel_found = 0;
    char ancho_found = 0;
    char alto_found = 0;
    char initLives_found = 0;
    char shootProb_found = 0;
    char maxBullets_found = 0;
    char balaID_found = 0;

    for(paramNo = 0; decodedFile[paramNo].parameter[0] != 0; paramNo++){ //Para todos los parametros en el array
        if(id_found == 0 && strcmp(decodedFile[paramNo].parameter, "id") == 0){
            id = atoi(decodedFile[paramNo].value);
            id_found++;
        }
        else if(vel_found == 0 && strcmp(decodedFile[paramNo].parameter, "vel") == 0){
            vel = atoi(decodedFile[paramNo].value);
            vel_found++;
        }
        else if(ancho_found == 0 && strcmp(decodedFile[paramNo].parameter, "ancho") == 0){
            ancho = atoi(decodedFile[paramNo].value);
            ancho_found++;
        }
        else if(alto_found == 0 && strcmp(decodedFile[paramNo].parameter, "alto") == 0){
            alto = atoi(decodedFile[paramNo].value);
            alto_found++;
        }
        else if(initLives_found == 0 && strcmp(decodedFile[paramNo].parameter, "initLives") == 0){
            initLives = atoi(decodedFile[paramNo].value);
            initLives_found++;
        }
        else if(shootProb_found == 0 && strcmp(decodedFile[paramNo].parameter, "shootProb") == 0){
            shootProb = atoi(decodedFile[paramNo].value);
            shootProb_found++;
        }
        else if(maxBullets_found == 0 && strcmp(decodedFile[paramNo].parameter, "maxBullets") == 0){
            maxBullets = atoi(decodedFile[paramNo].value);
            maxBullets_found++;
        }
        else if(balaID_found == 0 && strcmp(decodedFile[paramNo].parameter, "balaID") == 0){
            balaID = atoi(decodedFile[paramNo].value);
            balaID_found++;
        }
    }
    if(balaID_found != 1 || maxBullets_found != 1 || shootProb_found != 1 || initLives_found != 1 || alto_found != 1 || ancho_found != 1 || vel_found != 1 || id_found != 1){   //Si no se encontraron todos los campos devuelve error
        printf("Error in levelLoader.c, loadAsset function : \"%s\" has missing parameters\n", file);
        return -1;
    }
    addObjType(id, vel, ancho, alto, initLives, shootProb, maxBullets, balaID); //Añade el tipo de objeto
    return 0;
}

int loadAllAssets(char * platform, directory_t * directoryStore){    //Carga todos los assets
    if(platform == NULL){
        printf("Error in levelLoader.c, loadAllAssets function : platform cannot be a null pointer\n");
        return -1;
    }
    loadDirectory(ASSETSDIR, directoryStore);       //Carga los archivos del directorio de assets
    int archivoCounter;     //Contador de numero de archivos
    for(archivoCounter = 0; *((*directoryStore)[archivoCounter]) != 0; archivoCounter++){   //Por cada archivo
        int longitud = strlen(platform);
        if(strncmp((*directoryStore)[archivoCounter], platform, longitud) == 0){       //Si el archivo es de la plataforma elegida
            char direccionAsset[MAX_FILE_NAME]; //Variable auxiar para guardar la direccion del archivo a leer
            strcpy(direccionAsset, ASSETSDIR);  //Se copia el principio de la direccion
            strcat(direccionAsset, "/");    //Se agrega el slash
            strcat(direccionAsset, (*directoryStore)[archivoCounter]);  //Se agrega el final de la direccion(el nombre del archivo)
            loadAsset(direccionAsset);
        }
    }
    return 0;
}

int loadDirectory(char * carpeta, directory_t * directoryStore){
    struct dirent *directoryEntry;
    DIR *directoryPointer = opendir(carpeta);
    if(directoryPointer == NULL){
        printf("Error in levelLoader.c, loadDirectory function : \"%s\" could not open directory\n", carpeta);
        return -1;
    }
    int fileCounter = 0;
    while ((directoryEntry = readdir(directoryPointer)) != NULL){
        if(strlen(directoryEntry->d_name) >= 3){    //Si el archivo es un archivo valido debe tener 3 letras como minimo
            strcpy((*directoryStore)[fileCounter], directoryEntry->d_name);
            fileCounter++;
        }
    }
    closedir(directoryPointer);    
    return 0;
}



int main (){
    //loadAsset("../game/assets/test.asset");
    //imprimirARRAY();
    //directory_t carpetaAssets = {};
    //loadDirectory("../game/assets", &carpetaAssets);
    //loadAllAssets("rpi", &carpetaAssets);
    //imprimirARRAY();
    readFile("testfile.txt");
    printFile();
}

