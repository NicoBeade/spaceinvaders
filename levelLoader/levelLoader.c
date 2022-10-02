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

void printAliens(object_t ** listAliens){
    
    object_t * listaAliens = *listAliens;
    if(listaAliens == NULL){
        printf("ERROR PETE\n");
        return;
    }
    while (listaAliens != NULL){
        printf("POSX:%d\t POSY:%d\t LIV:%d\t ANIM:%d\t\n", listaAliens->pos.x, listaAliens->pos.y, listaAliens->lives, listaAliens->animationStatus);
        listaAliens=listaAliens->next;
    }
}


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
    char sprite1[MAX_SPRITE_FILE_LENGTH];
    char sprite2[MAX_SPRITE_FILE_LENGTH];
    char sprite3[MAX_SPRITE_FILE_LENGTH];

    char id_found = 0;
    char vel_found = 0;
    char ancho_found = 0;
    char alto_found = 0;
    char initLives_found = 0;
    char shootProb_found = 0;
    char maxBullets_found = 0;
    char balaID_found = 0;
    char sprite1_found = 0;
    char sprite2_found = 0;
    char sprite3_found = 0;

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
        else if(sprite1_found == 0 && strcmp(decodedFile[paramNo].parameter, "sprite1") == 0){
            if(strlen(decodedFile[paramNo].value) >= MAX_SPRITE_FILE_LENGTH){
                printf("Error in levelLoader.c, loadAsset function : sprite1 file directory overpassed the max of %d characters\n", MAX_SPRITE_FILE_LENGTH);
                return -1;
            }
            memcpy(sprite1, decodedFile[paramNo].value, MAX_SPRITE_FILE_LENGTH);
            sprite1_found++;
        }
        else if(sprite2_found == 0 && strcmp(decodedFile[paramNo].parameter, "sprite2") == 0){
            if(strlen(decodedFile[paramNo].value) >= MAX_SPRITE_FILE_LENGTH){
                printf("Error in levelLoader.c, loadAsset function : sprite1 file directory overpassed the max of %d characters\n", MAX_SPRITE_FILE_LENGTH);
                return -1;
            }
            memcpy(sprite2, decodedFile[paramNo].value, MAX_SPRITE_FILE_LENGTH);
            sprite2_found++;
        }
        else if(sprite3_found == 0 && strcmp(decodedFile[paramNo].parameter, "sprite3") == 0){
            if(strlen(decodedFile[paramNo].value) >= MAX_SPRITE_FILE_LENGTH){
                printf("Error in levelLoader.c, loadAsset function : sprite1 file directory overpassed the max of %d characters\n", MAX_SPRITE_FILE_LENGTH);
                return -1;
            }
            memcpy(sprite3, decodedFile[paramNo].value, MAX_SPRITE_FILE_LENGTH);
            sprite3_found++;
        }
    }
    if(balaID_found != 1 || maxBullets_found != 1 || shootProb_found != 1 || initLives_found != 1 || alto_found != 1 || ancho_found != 1 || vel_found != 1 || id_found != 1 || sprite1_found != 1 || sprite2_found != 1 || sprite3_found != 1 ){   //Si no se encontraron todos los campos devuelve error
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

int readLevelSettings(char * file, level_setting_t * levelSettings){
    if(readFile(file) != 0){ //Si no se pudo leer correctamente
        return -1;      //Devuelve error
    }
    int paramNo;
    int xMin;                   //-xMax: coordenada maxima en x alcanzable.
    int xMax;                   //-xMin: coordenada minima en x alcanzable.
    int yMin;                   //-YMax: coordenada maxima en Y alcanzable.
    int yMax;                   //-YMin: coordenada minima en Y alcanzable.
    int saltoX;                 //-saltoX: distancia entre naves en x
    int saltoY;                 //-saltoy: distancia entre naves en y (linea)
    int distInicialX;           //-distInicialX: coordenada en X de la nave del centro de la primera fila
    int distInicialY;           //-distInicialY: coordenada en Y de la nave del centro de la primera fila
    int anchoUsr;               //-anchoUsr: Ancho de la nave del usuario
    int margenX;                //-margenX: margen que queda libre en la pantalla (los aliens no pueden pasar de este margen)
    int margenY;                //-margenY: margen que queda libre en la pantalla (los aliens no pueden pasar de este margen)
    int disInicialUsrX;         //-distInicialUsrX: distancia inicial del usuario al iniciar un nivel en la coordenada X.
    int disInicialUsrY;         //-distInicialUsrY: distancia inicial del usuario al iniciar un nivel en la coordenada Y.
    int desplazamientoX;        //Cantidad de unidades que se mueven los aliens en X por tick
    int desplazamientoY;        //Cantidad de unidades que se mueven los aliens en Y por tick
    int desplazamientoUsr;        //-desplazamientoUsr: cantidad de unidades que se mueve el usuario por tick

    char xMin_found = 0;
    char xMax_found = 0;
    char yMin_found = 0;
    char yMax_found = 0;
    char saltoX_found = 0;
    char saltoY_found = 0;
    char distInicialX_found = 0;
    char distInicialY_found = 0;
    char anchoUsr_found = 0;
    char margenX_found = 0;
    char margenY_found = 0;
    char disInicialUsrX_found = 0;
    char disInicialUsrY_found = 0;
    char desplazamientoX_found = 0;
    char desplazamientoY_found = 0;
    char desplazamientoUsr_found = 0;


    for(paramNo = 0; decodedFile[paramNo].parameter[0] != 0; paramNo++){ //Para todos los parametros en el array
        if(xMin_found == 0 && strcmp(decodedFile[paramNo].parameter, "xMin") == 0){
            xMin = atoi(decodedFile[paramNo].value);
            xMin_found++;
        }
        else if(xMax_found == 0 && strcmp(decodedFile[paramNo].parameter, "xMax") == 0){
            xMax = atoi(decodedFile[paramNo].value);
            xMax_found++;
        }
        else if(yMin_found == 0 && strcmp(decodedFile[paramNo].parameter, "yMin") == 0){
            yMin = atoi(decodedFile[paramNo].value);
            yMin_found++;
        }
        else if(yMax_found == 0 && strcmp(decodedFile[paramNo].parameter, "yMax") == 0){
            yMax = atoi(decodedFile[paramNo].value);
            yMax_found++;
        }
        else if(saltoX_found == 0 && strcmp(decodedFile[paramNo].parameter, "saltoX") == 0){
            saltoX = atoi(decodedFile[paramNo].value);
            saltoX_found++;
        }
        else if(saltoY_found == 0 && strcmp(decodedFile[paramNo].parameter, "saltoY") == 0){
            saltoY = atoi(decodedFile[paramNo].value);
            saltoY_found++;
        }
        else if(distInicialX_found == 0 && strcmp(decodedFile[paramNo].parameter, "distInicialX") == 0){
            distInicialX = atoi(decodedFile[paramNo].value);
            distInicialX_found++;
        }
        else if(distInicialY_found == 0 && strcmp(decodedFile[paramNo].parameter, "distInicialY") == 0){
            distInicialY = atoi(decodedFile[paramNo].value);
            distInicialY_found++;
        }
        else if(anchoUsr_found == 0 && strcmp(decodedFile[paramNo].parameter, "anchoUsr") == 0){
            anchoUsr = atoi(decodedFile[paramNo].value);
            anchoUsr_found++;
        }
        else if(margenX_found == 0 && strcmp(decodedFile[paramNo].parameter, "margenX") == 0){
            margenX = atoi(decodedFile[paramNo].value);
            margenX_found++;
        }
        else if(margenY_found == 0 && strcmp(decodedFile[paramNo].parameter, "margenY") == 0){
            margenY = atoi(decodedFile[paramNo].value);
            margenY_found++;
        }
        else if(disInicialUsrX_found == 0 && strcmp(decodedFile[paramNo].parameter, "disInicialUsrX") == 0){
            disInicialUsrX = atoi(decodedFile[paramNo].value);
            disInicialUsrX_found++;
        }
        else if(disInicialUsrY_found == 0 && strcmp(decodedFile[paramNo].parameter, "disInicialUsrY") == 0){
            disInicialUsrY = atoi(decodedFile[paramNo].value);
            disInicialUsrY_found++;
        }
        else if(desplazamientoX_found == 0 && strcmp(decodedFile[paramNo].parameter, "desplazamientoX") == 0){
            desplazamientoX = atoi(decodedFile[paramNo].value);
            desplazamientoX_found++;
        }
        else if(desplazamientoY_found == 0 && strcmp(decodedFile[paramNo].parameter, "desplazamientoY") == 0){
            desplazamientoY = atoi(decodedFile[paramNo].value);
            desplazamientoY_found++;
        }
        else if(desplazamientoUsr_found == 0 && strcmp(decodedFile[paramNo].parameter, "desplazamientoUsr") == 0){
            desplazamientoUsr = atoi(decodedFile[paramNo].value);
            desplazamientoUsr_found++;
        }
    }
    if(xMin_found != 1 || xMax_found != 1 || yMin_found != 1 || yMax_found != 1 || saltoX_found != 1 || saltoY_found != 1 || distInicialX_found != 1 || distInicialY_found != 1 || anchoUsr_found != 1 || margenX_found != 1 || margenY_found != 1 || disInicialUsrX_found != 1 || disInicialUsrY_found != 1 || desplazamientoX_found != 1 || desplazamientoY_found != 1 || desplazamientoUsr_found != 1){   //Si no se encontraron todos los campos devuelve error
        printf("Error in levelLoader.c, readLevel function : \"%s\" has missing parameters\n", file);
        return -1;
    }
    levelSettings->anchoUsr = anchoUsr;
    levelSettings->desplazamientoUsr = desplazamientoUsr;
    levelSettings->desplazamientoX = desplazamientoX;
    levelSettings->desplazamientoY = desplazamientoY;
    levelSettings->disInicialUsrX = disInicialUsrX;
    levelSettings->disInicialUsrY = disInicialUsrY;
    levelSettings->distInicialX = distInicialX;
    levelSettings->distInicialY = distInicialY;
    levelSettings->margenX = margenX;
    levelSettings->margenY = margenY;
    levelSettings->saltoX = saltoX;
    levelSettings->saltoY = saltoY;
    levelSettings->xMax = xMax;
    levelSettings->xMin = xMin;
    levelSettings->yMax = yMax;
    levelSettings->yMin = yMin;
    return 0;
}

int loadLevel(int levelNo, level_setting_t * levelSettings, char * platform, object_t ** listaAliens, object_t ** listaUsr, object_t ** listaBarreras){
    char levelFile[MAX_DIR_LENGTH+MAX_FILE_NAME];
    if(platform == NULL){
        printf("Error in levelLoader.c, loadLevel function : platform cannot be NULL\n");
        return -1;
    }
    if(levelNo < 0){
        printf("Error in levelLoader.c, loadLevel function : level cannot be negative\n");
        return -1;
    }
    if(strlen(platform) != 3){
        printf("Error in levelLoader.c, loadLevel function : platform must have 3 letters\n");
        return -1;
    }
    if(levelNo > MAX_LEVEL){
        printf("Error in levelLoader.c, loadLevel function : level number reached max\n");
        return -1;
    }
    if(levelSettings == NULL && levelNo){
        printf("Error in levelLoader.c, loadLevel function : NULL pointer in a level > 0\n");
        return -1;
    }
    sprintf(levelFile, "%s/%s%s%d%s", LEVELSDIR, platform, "_level", levelNo,".level"); //Genera el string del archivo a leer
    if(levelNo == 0 && readLevelSettings(levelFile,  levelSettings) == -1){ //Si el nivel es el cero, carga el levelSettings
        return -1;
    }
    else{
        if(readFile(levelFile) == -1){      //Lee el archivo
            if(levelNo == 0){
                return -1;                      //Si no se pudo leer termina la ejecucion
            }
            else{
                return -2;                  //Si no se encontro mas niveles termina
            }
        }
        else{                   //Si se pudo leer el archivo comienza a leer los npcs
            int fila = 0;        //Se crea una variable fila
            while(decodedFile[fila].parameter[0] != 0){ //Para todos los parametros en el array
                if(strcmp(decodedFile[fila].parameter, "ALIEN") == 0){  //Si es un alien
                    fila++;     //Se incrementa la fila
                    
                    object_t alien;                     //Buffer del alien a leer
                    fila = readObj(fila, &alien);       //Lee el alien
                    if(fila == -1){    //Si no se pudo leer termina
                        return -1;
                    }                    
                    objectType_t * objType = getObjType(alien.type);        //Se recupera el tipo de alien
                    (* listaAliens) = addObj((* listaAliens), alien.pos, alien.type, objType->initLives);    //Se agrega a la lista
                    printf("george\n");
                }
                else if(strcmp(decodedFile[fila].parameter, "BARRERA") == 0){  //Si es una barrera
                    fila++;     //Se incrementa la fila
                    object_t barrera;                     //Buffer de la barrera a leer
                    fila = readObj(fila, &barrera);       //Lee la barrera
                    if(fila == -1){    //Si no se pudo leer termina
                        return -1;
                    }    
                    objectType_t * objType = getObjType(barrera.type);        //Se recupera el tipo de barrera
                    (* listaBarreras) = addObj((* listaBarreras), barrera.pos, barrera.type, objType->initLives);    //Se agrega a la lista
                }
                else if(strcmp(decodedFile[fila].parameter, "USUARIO") == 0){  //Si es una barrera
                    fila++;     //Se incrementa la fila
                    object_t usuario;                     //Buffer de la barrera a leer
                    fila = readObj(fila, &usuario);       //Lee el usuario
                    if(fila == -1){    //Si no se pudo leer termina
                        return -1;
                    }    
                    objectType_t * objType = getObjType(usuario.type);        //Se recupera el tipo de barrera
                    (* listaUsr) = addObj((* listaUsr), usuario.pos, usuario.type, objType->initLives);    //Se agrega a la lista
                }
                fila++;
            }
        }
    }
    return 0;
}

int readObj(int paramNo, object_t * objOut){
    int type_found = 0;
    int posX_found = 0;
    int posY_found = 0;
    int fila;
    if(decodedFile[paramNo].parameter == NULL){
        printf("Error in levelLoader.c, readObj function : paramNo %d not valid\n",paramNo);
        return -1;
    }
    for(fila = paramNo; (strcmp(decodedFile[fila].parameter, "END") != 0) && (decodedFile[fila].parameter[0] != 0); fila++){
        if(type_found == 0 && strcmp(decodedFile[fila].parameter, "type") == 0){
            if(getObjType(atoi(decodedFile[fila].value)) == NULL){       //Si no se encuentra el tipo de objeto se sale de la funcion y devuelve error
                printf("Error in levelLoader.c, readObj function : type %s not valid\n",decodedFile[fila].value);
                return -1;
            }
            else{           //Si se encontro el tipo de objeto se almacena
                objOut->type = atoi(decodedFile[fila].value);   
                type_found++;     
            }
        }
        else if(posX_found == 0 && strcmp(decodedFile[fila].parameter, "posX") == 0){        //Si el parametro es la posicion en X
            (objOut->pos).x = atoi(decodedFile[fila].value);        //Se guarda
            posX_found++;       //Se incrementa el contador
        }
        else if(posY_found == 0 && strcmp(decodedFile[fila].parameter, "posY") == 0){        //Si el parametro es la posicion en y
            (objOut->pos).y = atoi(decodedFile[fila].value);        //Se guarda
            posY_found++;       //Se incrementa el contador
        }
    }
    if(decodedFile[fila].parameter[0] == 0 && strcmp(decodedFile[fila].parameter, "END") != 0){
        printf("Error in levelLoader.c, readObj function : END line not founded\n");
        return -1;
    }
    if(type_found != 1 || posX_found != 1 || posY_found != 1){   //Si no se encontraron todos los campos devuelve error
        printf("Error in levelLoader.c, readObj function : file has missing parameters: type_found:%d, posX_found:%d, posY_found: %d, in line number %d\n", type_found, posX_found, posY_found, fila);
        return -1;
    }
    return fila;
}

/*
int main (){
    //loadAsset("../game/assets/test.asset");
    //imprimirARRAY();
    directory_t carpetaAssets = {};
    loadDirectory("../game/assets", &carpetaAssets);
    loadAllAssets("rpi", &carpetaAssets);
    imprimirARRAY();
    clearFileBuffer();
    readFile("../game/levels/rpi_level1.level");
    object_t alien;

    printf("%s\t%s\n", decodedFile[0].parameter,decodedFile[0].value);
    printf("%s\t%s\n", decodedFile[1].parameter,decodedFile[1].value);
    printf("%s\t%s\n", decodedFile[2].parameter,decodedFile[2].value);
    printf("%s\t%s\n", decodedFile[3].parameter,decodedFile[3].value);
    printf("%s\t%s\n", decodedFile[4].parameter,decodedFile[4].value);
    printf("%s\t%s\n", decodedFile[5].parameter,decodedFile[5].value);
    printf("%s\t%s\n", decodedFile[6].parameter,decodedFile[6].value);
    printf("%s\t%s\n", decodedFile[7].parameter,decodedFile[7].value);
    printf("%s\t%s\n", decodedFile[8].parameter,decodedFile[8].value);
    printf("%s\t%s\n", decodedFile[9].parameter,decodedFile[9].value);
    printf("%s\t%s\n", decodedFile[10].parameter,decodedFile[10].value);
    printf("%s\t%s\n", decodedFile[11].parameter,decodedFile[11].value);
    printf("%s\t%s\n", decodedFile[12].parameter,decodedFile[12].value);
    printf("%s\t%s\n", decodedFile[13].parameter,decodedFile[13].value);
    printf("%s\t%s\n", decodedFile[14].parameter,decodedFile[14].value);
    printf("%s\t%s\n", decodedFile[15].parameter,decodedFile[15].value);
    printf("%s\t%s\n", decodedFile[16].parameter,decodedFile[16].value);
    printf("%s\t%s\n", decodedFile[17].parameter,decodedFile[17].value);
    printf("%s\t%s\n", decodedFile[18].parameter,decodedFile[18].value);
    printf("%s\t%s\n", decodedFile[19].parameter,decodedFile[19].value);
    printf("%s\t%s\n", decodedFile[20].parameter,decodedFile[20].value);

    level_setting_t levelSettings;
        object_t * listaAliens = NULL;
    loadLevel(0, &levelSettings, "rpi",&listaAliens,NULL,NULL);

    loadLevel(1, &levelSettings, "rpi",&listaAliens,NULL,NULL);
    //readLevel("../game/levels/rpi_level0.level", &levelSettings);
    /*
    printf("%d\n", levelSettings.anchoUsr); 
    printf("%d\n", levelSettings.desplazamientoUsr);
    printf("%d\n", levelSettings.desplazamientoX);
    printf("%d\n", levelSettings.desplazamientoY);
    printf("%d\n", levelSettings.disInicialUsrX);
    printf("%d\n", levelSettings.disInicialUsrY);
    printf("%d\n", levelSettings.distInicialX);
    printf("%d\n", levelSettings.distInicialY);
    printf("%d\n", levelSettings.margenX);
    printf("%d\n", levelSettings.margenY);
    printf("%d\n", levelSettings.saltoX);
    printf("%d\n", levelSettings.saltoY);
    printf("%d\n", levelSettings.xMax);
    printf("%d\n", levelSettings.xMin);
    printf("%d\n", levelSettings.yMax);
    printf("%d\n", levelSettings.yMin);
    
    printAliens(&listaAliens);
    
}
*/



