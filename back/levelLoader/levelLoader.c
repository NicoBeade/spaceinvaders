#include <stdio.h>
#include <stdint.h>
#include "levelLoader.h"
#include <string.h>
#include <stdlib.h>
#include <dirent.h>


static int getAbsValue(int relativeMode, char * valueReaded, int previousValue);
static int checkOutOfBounds(vector_t posicion, int ancho, int alto, level_setting_t * levelSettings);

#define ISNUM(caracter) (((caracter) >= '0' ) && ((caracter) <= '9' ))


enum paramType {ENTERO, ARCHIVO, BINARIO};      //Identifica el tipo de valor que tiene un parametro
enum estados {START, PARAM, VALUE, SPACE};      //Identifica el estado del lector de archivos


typedef struct{
    char parameter[MAX_PARAM_LETTERS];      //String del parametro
    char value[MAX_VALUE_LETTERS];          //String del valor asociado
}lineaArchivo_t;   
//Almacena una linea de un archivo


static lineaArchivo_t decodedFile[MAX_FILE_ROWS];       //Array de parametros+valores de todo el archivo

void printAliens(object_t ** listAliens){           //Funcion que muestra en la consola una lista
    
    object_t * listaAliens = *listAliens;       //Variable aux
    if(listaAliens == NULL){
        printf("Lista Vacia\n");
        return;
    }
    while (listaAliens != NULL){    //Si la lista no esa vacia
        printf("POSX:%d\t POSY:%d\t LIV:%d\t ANIM:%d\t\n", listaAliens->pos.x, listaAliens->pos.y, listaAliens->lives, listaAliens->animationStatus);   //Imprime todos los campos
        listaAliens=listaAliens->next;  //Siguiente nodo
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
                int ignoreComment = 0;             //Se crea variable auxiliar ingnorar comentario
                if(caracter != ';'){    //Si no es un comentario
                    while(caracter != 0){    //Mientras no sea un un 0 recorre los caracteres de una linea                        
                        switch(state){      //Dependiendo de en donde se encuentre leyendo (parametro, espacio o valor)
                            case START:     //Si es el primer caracter
                                if(caracter == ' ' || caracter == '\t'){    //Si es un espacio o un enter lo ignora
                                    break;
                                }
                                else if(caracter == ';' || caracter == '\r' || caracter == '\n' || caracter == 0 ){   //Si encuentra un comentario o un finalizador de linea
                                    ignoreComment = 1;
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
                        if(lineaStr[letra+1] == 0 || lineaStr[letra+1] == '\n' || lineaStr[letra+1] == '\r' || lineaStr[letra+1] == ';' || lineaStr[letra] == ';'){   //Si el siguiente caracter es un finalizador de la linea, se termina el while
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
                    if(ignoreComment == 0){  //Si la linea no es un comentario se incrementa el index de linea
                        linea++;
                    }
                    else{               //Si es un comentario se vuelve a setear en 0 el ignore comment para que la proxima linea se tome como "normal"
                        ignoreComment = 0;
                    }
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


void clearFileBuffer(void){     //Funcion que borra el buffer del archivo
    int index;              //Variable aux que recorre las filas
    int index2;             //Variable aux que recorre los caracteres de cada parametro en cada fila
    int index3;             //Variable aux que recorre los caracteres de cada valor en cada fila
    for(index = 0; index < MAX_FILE_ROWS; index++){     //Por cada fila
        for(index2 = 0; index2 < MAX_PARAM_LETTERS; index2++){      //Por cada caracter del parametro de esa fila
            decodedFile[index].parameter[index2] = 0;               //Se borra
        }
        for(index3 = 0; index3 < MAX_VALUE_LETTERS; index3++){      //Por cada caracter del valor de esa fila
            decodedFile[index].value[index3] = 0;                   //Se borra
        }
    }
}

void printFile(void){       //Funcion que muestra en consola el archivo
    int index;          //Contador de filas
    for(index = 0; decodedFile[index].parameter[0] != 0; index++){      //Por cada fila
        printf("PARAMETRO: %s VALOR: %s\n", decodedFile[index].parameter, decodedFile[index].value);    //Se muestra la fila
    }
}

int loadAsset(char * file){     //Funcion que carga un asset recibiendo el address de ese archivo
    if(readFile(file) != 0){ //Si no se pudo leer correctamente
        return -1;      //Devuelve error
    }
    int paramNo;        //Variable que cuenta los parametros
    //Se crean variables auxiliares para todos los campos
    int id;
    int vel;
    int ancho;
    int alto;
    int initLives;
    int shootProb;
    int maxBullets;
    int balaID;
    int score;
    int aliado;
    char * sprite1;
    char * sprite2;
    char * sprite3;
    char * sprite4;
    char * shootSound;
    char * deathSound;
    //Se crean variables auxiliares que indican si se encontro cada campo
    char id_found = 0;
    char vel_found = 0;
    char ancho_found = 0;
    char alto_found = 0;
    char initLives_found = 0;
    char shootProb_found = 0;
    char maxBullets_found = 0;
    char balaID_found = 0;
    char score_found = 0;
    char aliado_found = 0;
    char sprite1_found = 0;
    char sprite2_found = 0;
    char sprite3_found = 0;
    char sprite4_found = 0;
    char shootSound_found = 0;
    char deathSound_found = 0;

    for(paramNo = 0; decodedFile[paramNo].parameter[0] != 0; paramNo++){ //Para todos los parametros en el array
        if(id_found == 0 && strcmp(decodedFile[paramNo].parameter, "id") == 0){     //Si no se encontro ese campo y lo escrito en la fila coincide con el nombre del campo
            id = atoi(decodedFile[paramNo].value);                                  //Se guarda en value
            id_found++;                                                             //Se incrementa el contador que indica cuantas veces se encontro el campo
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
        else if(score_found == 0 && strcmp(decodedFile[paramNo].parameter, "score") == 0){
            score = atoi(decodedFile[paramNo].value);
            score_found++;
        }
        else if(aliado_found == 0 && strcmp(decodedFile[paramNo].parameter, "aliado") == 0){
            aliado = atoi(decodedFile[paramNo].value);
            aliado_found++;
        }
        else if(sprite1_found == 0 && strcmp(decodedFile[paramNo].parameter, "sprite1") == 0){
            if(strlen(decodedFile[paramNo].value) >= MAX_SPRITE_FILE_LENGTH){   //Si la direccion del sprite guardado en el archivo es muy larga entonces salta error
                printf("Error in levelLoader.c, loadAsset function : sprite1 file directory overpassed the max of %d characters\n", MAX_SPRITE_FILE_LENGTH);
                return -1;
            }
            sprite1 = strcmp(decodedFile[paramNo].parameter, "NULL")? &(decodedFile[paramNo].value)[0] : NULL;
            sprite1_found++;
        }
        else if(sprite2_found == 0 && strcmp(decodedFile[paramNo].parameter, "sprite2") == 0){
            if(strlen(decodedFile[paramNo].value) >= MAX_SPRITE_FILE_LENGTH){
                printf("Error in levelLoader.c, loadAsset function : sprite2 file directory overpassed the max of %d characters\n", MAX_SPRITE_FILE_LENGTH);
                return -1;
            }
            sprite2 = strcmp(decodedFile[paramNo].parameter, "NULL")? &(decodedFile[paramNo].value)[0] : NULL;
            sprite2_found++;
        }
        else if(sprite3_found == 0 && strcmp(decodedFile[paramNo].parameter, "sprite3") == 0){
            if(strlen(decodedFile[paramNo].value) >= MAX_SPRITE_FILE_LENGTH){
                printf("Error in levelLoader.c, loadAsset function : sprite3 file directory overpassed the max of %d characters\n", MAX_SPRITE_FILE_LENGTH);
                return -1;
            }
            sprite3 = strcmp(decodedFile[paramNo].parameter, "NULL")? &(decodedFile[paramNo].value)[0] : NULL;
            sprite3_found++;
        }
        else if(sprite4_found == 0 && strcmp(decodedFile[paramNo].parameter, "sprite4") == 0){
            if(strlen(decodedFile[paramNo].value) >= MAX_SPRITE_FILE_LENGTH){
                printf("Error in levelLoader.c, loadAsset function : sprite4 file directory overpassed the max of %d characters\n", MAX_SPRITE_FILE_LENGTH);
                return -1;
            }
            sprite4 = strcmp(decodedFile[paramNo].parameter, "NULL")? &(decodedFile[paramNo].value)[0] : NULL;
            sprite4_found++;
        }
        else if(shootSound_found == 0 && strcmp(decodedFile[paramNo].parameter, "shootSound") == 0){
            if(strlen(decodedFile[paramNo].value) >= MAX_SOUND_FILE_LENGTH){
                printf("Error in levelLoader.c, loadAsset function : shootSound file directory overpassed the max of %d characters\n", MAX_SOUND_FILE_LENGTH);
                return -1;
            }
            shootSound = strcmp(decodedFile[paramNo].parameter, "NULL")? &(decodedFile[paramNo].value)[0] : NULL;
            shootSound_found++;
        }
        else if(deathSound_found == 0 && strcmp(decodedFile[paramNo].parameter, "deathSound") == 0){
            if(strlen(decodedFile[paramNo].value) >= MAX_SOUND_FILE_LENGTH){
                printf("Error in levelLoader.c, loadAsset function : deathSound file directory overpassed the max of %d characters\n", MAX_SOUND_FILE_LENGTH);
                return -1;
            }
            deathSound = strcmp(decodedFile[paramNo].parameter, "NULL")? &(decodedFile[paramNo].value)[0] : NULL;
            deathSound_found++;
        }
    }
    //Si todos los campos se encontraron entonces añade el asset a la lista de object types
    if(balaID_found != 1 || maxBullets_found != 1 || shootProb_found != 1 || initLives_found != 1 || alto_found != 1 || ancho_found != 1 || vel_found != 1 || id_found != 1 || score_found != 1 || sprite1_found != 1 || sprite2_found != 1 || sprite3_found != 1 || sprite4_found != 1 ||shootSound_found != 1 || deathSound_found != 1 || aliado_found != 1){   //Si no se encontraron todos los campos devuelve error
        printf("Error in levelLoader.c, loadAsset function : \"%s\" has missing parameters\n", file);
        printf("deathSound_found %d shootSound_found %d  score_found %d \n", deathSound_found, shootSound_found,score_found);
        return -1;
    }
    addObjType(id, vel, ancho, alto, initLives, shootProb, maxBullets, balaID,sprite1, sprite2, sprite3, sprite4, shootSound,deathSound,score, aliado); //Añade el tipo de objeto
    return 0;
}

int loadAllAssets(char * platform, directory_t * directoryStore){    //Carga todos los assets
    if(platform == NULL){
        printf("Error in levelLoader.c, loadAllAssets function : platform cannot be a null pointer\n");
        return -1;
    }

    loadDirectory(ASSETSDIR, directoryStore);       //Carga los archivos del directorio de assets
    int archivoCounter;     //Contador de numero de archivos
    for(archivoCounter = 0; *((*directoryStore)[archivoCounter]) != 0 && archivoCounter < MAX_FILES_IN_FOLDER; archivoCounter++){   //Por cada archivo
        int longitud = strlen(platform);
        if(strncmp((*directoryStore)[archivoCounter], platform, longitud) == 0 && stringEndCmp((*directoryStore)[archivoCounter], ASSETFORMAT)){       //Si el archivo es de la plataforma elegida y tiene el formato indicado
            char direccionAsset[MAX_FILE_NAME]; //Variable auxiar para guardar la direccion del archivo a leer
            strcpy(direccionAsset, ASSETSDIR);  //Se copia el principio de la direccion
            strcat(direccionAsset, "/");    //Se agrega el slash
            strcat(direccionAsset, (*directoryStore)[archivoCounter]);  //Se agrega el final de la direccion(el nombre del archivo)
            loadAsset(direccionAsset);
;       }
    }
     if(archivoCounter >= MAX_FILES_IN_FOLDER){ //Si el contador de archivos supera el maximo de numero de archivos en una carpeta salta error
        printf("Error in levelLoader.c, loadAllAssets function : archivoCounter reached maximum of %d max files in te loaded directory", MAX_FILES_IN_FOLDER);
        return -1; 
    } 

    return 0;
}

int indexAllLevels(char * platform, char * levelsDir, char * levelPrefix, directory_t * directoryStore, level_t levelArray[]){  //Almacena todos los niveles
    if(platform == NULL){
        printf("Error in levelLoader.c, indexAllLevels function : platform cannot be a null pointer\n");
        return -1;
    }
    loadDirectory(levelsDir, directoryStore); //Carga los archivos del directorio de niveles
    int archivoCounter; //Contador de numero de archivos
    int nivel=0;        //Contador de niveles encontrados
    for(archivoCounter = 0; *((*directoryStore)[archivoCounter]) != 0 && archivoCounter < MAX_FILES_IN_FOLDER; archivoCounter++){   //Por cada archivo
        int longitud = strlen(platform);
        int longitudPrefijo = longitud + strlen(levelPrefix); //Cantidad de caracteres que debe ignorar la funcion getLevelNoOfFile
        if(strncmp((*directoryStore)[archivoCounter], platform, longitud) == 0 && stringEndCmp((*directoryStore)[archivoCounter], LEVELFORMAT)){       //Si el archivo es de la plataforma elegida y tiene el formato del nivel
            (levelArray[nivel]).level = getLevelNoOfFile(longitudPrefijo, &((*directoryStore)[archivoCounter][0]), MAX_FILE_NAME, NULL); //Se obtiene el numero de nivel
            strcpy(levelArray[nivel].levelName,(*directoryStore)[archivoCounter]);   //Se copia el nombre
            nivel++;
        }
        if(nivel >= MAX_LEVEL){
            printf("Error in levelLoader.c, loadAllAssets function : level %d reached maximum of %d max levels in the levelArray",nivel, MAX_LEVEL);
            return -1; 
        }
        if(*((*directoryStore)[archivoCounter+1]) == 0){  //Si es el ultimo nivel
            (levelArray[nivel]).lastLevelTrue = 1;  //Se pone verdadero al lastLevel
        }
        
    }
    if(archivoCounter >= MAX_FILES_IN_FOLDER){
        printf("Error in levelLoader.c, loadAllAssets function : archivoCounter reached maximum of %d max files in the loaded directory", MAX_FILES_IN_FOLDER);
        return -1; 
    }
    //Faltaria poner el quicksort

    //AGREGADO////////////////////////////////////////////////////////////
    return 0; 
}

int levelArrayLen(level_t levelArray[]){ //Calcula la longitud del array de niveles
    int contador;
    for(contador = 0; (levelArray[contador]).lastLevelTrue == 0 && contador<MAX_LEVEL ; contador ++);
    if(contador == MAX_LEVEL){
        printf("Error in levelLoader.c, levelArrayLen function : levelArray ender not found");
        return -1; 
    }
    return contador;
}

int getLevelNoOfFile(int prefixLenghtToIgnore, char * fileName, int maxFileLenght, char * nameOut){
    //nameOut is an optional output, set to NULL if not needed
    int contadorChar = prefixLenghtToIgnore; //El contador de char se incrementa hasta pasar la plataforma y el prefijo
    char * fileNameP = fileName + contadorChar;    //Variable auxiliar para recorrer el nombre
    char auxNo[MAX_FILE_NAME];      //Variable auxiliar para almacenar el numero
    int inside=0;                   //Flag auxiliar para indicar si esta adentro del numero
    int contadorDigitos = 0;
    int resultado = 0;  //Variable auxiliar que guarda el resultado
    int numberTrue = 1; //Si el No del nivel es siempre un numero
    if(ISNUM(*fileNameP) || *fileNameP == '-'){   //Si es un numero o un menos entonces ya arranca adentro
        inside = 1;
    }
    while(*fileNameP != 0 && contadorChar < maxFileLenght - 1 && inside != -1){    //Mientras que el caracter no sea el terminador o un punto
        if(*fileNameP == '_' && inside == 0){      //Si es un _ entra adentro del numero
            inside = 1;
        }
        else if(*fileNameP != '_' &&  *fileNameP != '.' && inside == 1){        //Si no es un _ y esta adentro del numero comienza a guardar en la variable auxiliar
            auxNo[contadorDigitos] = *fileNameP;    //Se copia el caracter 
            contadorDigitos++;
            if(!ISNUM(*fileNameP) && *fileNameP != '-'){      //Si no es un numero o un menos
                numberTrue = 0;     //Se setea el flag en 0
                //resultado += (int) *fileNameP;  //Se suma 
            }
        }
        else if((*fileNameP == '_' || *fileNameP == '.' ) && inside == 1){   //Si esta dentro y encuentra un _ o un punto sale
            inside = -1;
            auxNo[contadorDigitos] = 0;     //Agrega el terminador
        }
        fileNameP++;    //Apunta al siguiente
        contadorChar++;
    }
    if(contadorChar >= maxFileLenght - 1){
        printf("Error in levelLoader.c, getLevelNoOfFile function : %s reached maximum of %d max file lenght",fileName, maxFileLenght);
        return -1; 
    }
    else if (*fileNameP == 0){
        printf("Error in levelLoader.c, getLevelNoOfFile function : found a terminator inside the file name %s or missing dot/underscore", fileName);
        return -1; 
    }
    else {
        if(nameOut != NULL){        //Si se ingreso un nameOut, se copia la salida al mismo
            strcpy(nameOut, auxNo);
        }
        if(numberTrue){     //Si se ingreso verdaderamente un numero
            resultado = atoi(auxNo);
        }
        return resultado; //Devuelve el numero
    }
    
}

int loadDirectory(char * carpeta, directory_t * directoryStore){
    struct dirent *directoryEntry;
    DIR *directoryPointer = opendir(carpeta);
    if(directoryPointer == NULL){
        printf("Error in levelLoader.c, loadDirectory function : \"%s\" could not open directory\n", carpeta);
        return -1;
    }
    int fileCounter = 0;
    while ((directoryEntry = readdir(directoryPointer)) != NULL && fileCounter < MAX_FILES_IN_FOLDER){
        if(strlen(directoryEntry->d_name) >= 3){    //Si el archivo es un archivo valido debe tener 3 letras como minimo
            strcpy((*directoryStore)[fileCounter], directoryEntry->d_name);
            fileCounter++;
        } 
    }
    closedir(directoryPointer);   
    if(fileCounter >= MAX_FILES_IN_FOLDER){
        printf("Error in levelLoader.c, loadDirectory function : fileCounter reached maximum of %d max files in folder", MAX_FILES_IN_FOLDER);
        return -1; 
    } 
    return 0;
}


int readAssetModifiers(int paramNo, int AssetID){   //Recibe el nombre del archivo y cambia los campos del asset que se indican en el archivo
    int fila;    //Index de la fila
    objectType_t * assetModified = getObjType(AssetID); //Se busca el asset que se quiere modificar
    if (assetModified == NULL){
        printf("Error in levelLoader.c, readAssetModifiers function Asset not found\n");
        return -1;
    }
    if(strlen(decodedFile[paramNo].parameter) == 0){
        printf("Error in levelLoader.c, readAssetModifiers function : paramNo %d not valid\n",paramNo);
        return -1;
    }
    for(fila = paramNo; (strcmp(decodedFile[fila].parameter, "END") != 0) && (decodedFile[fila].parameter[0] != 0); fila++){    //Por cada fila del archivo a partir de la del parametro
        if(strcmp(decodedFile[fila].parameter, "velocidad") == 0 && strlen(decodedFile[fila].value) > 0){   //Si el parametro es velocidad, la modifica
            assetModified->velocidad = getAbsValue(1,decodedFile[fila].value, assetModified->velocidad);
            printf("Se modifico la velocidad del asset %d con el valor %d\n",AssetID,assetModified->velocidad);
        }
        else if(strcmp(decodedFile[fila].parameter, "ancho") == 0 && strlen(decodedFile[fila].value) > 0){   //Si el parametro es ancho, la modifica
            assetModified->ancho = getAbsValue(1,decodedFile[fila].value, assetModified->ancho);
        }
        else if(strcmp(decodedFile[fila].parameter, "alto") == 0 && strlen(decodedFile[fila].value) > 0){   //Si el parametro es alto, la modifica
            assetModified->alto = getAbsValue(1,decodedFile[fila].value, assetModified->alto);
        }
        else if(strcmp(decodedFile[fila].parameter, "initLives") == 0 && strlen(decodedFile[fila].value) > 0){   //Si el parametro es initLives, la modifica
            assetModified->initLives = getAbsValue(1,decodedFile[fila].value, assetModified->initLives);
        }
        else if(strcmp(decodedFile[fila].parameter, "shootProb") == 0 && strlen(decodedFile[fila].value) > 0){   //Si el parametro es shootProb, la modifica
            assetModified->shootProb = getAbsValue(1,decodedFile[fila].value, assetModified->shootProb);
        }
        else if(strcmp(decodedFile[fila].parameter, "maxBullets") == 0 && strlen(decodedFile[fila].value) > 0){   //Si el parametro es maxBullets, la modifica
            assetModified->maxBullets = getAbsValue(1,decodedFile[fila].value, assetModified->maxBullets);
        }
        else if(strcmp(decodedFile[fila].parameter, "balaID") == 0 && strlen(decodedFile[fila].value) > 0){   //Si el parametro es maxBullets, la modifica
            assetModified->balaID = getAbsValue(1,decodedFile[fila].value, assetModified->balaID);
        }
        else if(strcmp(decodedFile[fila].parameter, "score") == 0 && strlen(decodedFile[fila].value) > 0){   //Si el parametro es maxBullets, la modifica
            assetModified->score = getAbsValue(1,decodedFile[fila].value, assetModified->score);
        }
        else if(strcmp(decodedFile[fila].parameter, "aliado") == 0 && strlen(decodedFile[fila].value) > 0){   //Si el parametro es maxBullets, la modifica
            assetModified->aliado = atoi(decodedFile[fila].value);
        }
        else if(strcmp(decodedFile[fila].parameter, "sprite1") == 0 && strlen(decodedFile[fila].value) > 0){
            if(strlen(decodedFile[fila].value) < MAX_SPRITE_FILE_LENGTH){
                strcpy(assetModified->sprite1,decodedFile[fila].value);//Se copia el path
            }
            else{
                printf("Error in levelLoader.c, readAssetModifiers function : too many characters in sprite 1 %s\n", (decodedFile[fila].value));
                return -1;   
            } 
        }
        else if(strcmp(decodedFile[fila].parameter, "sprite2") == 0 && strlen(decodedFile[fila].value) > 0){
            if(strlen(decodedFile[fila].value) < MAX_SPRITE_FILE_LENGTH){
                strcpy(assetModified->sprite2,decodedFile[fila].value);//Se copia el path
            }
            else{
                printf("Error in levelLoader.c, readAssetModifiers function : too many characters in sprite 2 %s\n", (decodedFile[fila].value));
                return -1;   
            }
        }
        else if(strcmp(decodedFile[fila].parameter, "sprite3") == 0 && strlen(decodedFile[fila].value) > 0){
            if(strlen(decodedFile[fila].value) < MAX_SPRITE_FILE_LENGTH){
                strcpy(assetModified->sprite3,decodedFile[fila].value);//Se copia el path
            }
            else{
                printf("Error in levelLoader.c, readAssetModifiers function : too many characters in sprite 3 %s\n", (decodedFile[fila].value));
                return -1;   
            }
        }
        else if(strcmp(decodedFile[fila].parameter, "sprite4") == 0 && strlen(decodedFile[fila].value) > 0){
            if(strlen(decodedFile[fila].value) < MAX_SPRITE_FILE_LENGTH){
                strcpy(assetModified->sprite4,decodedFile[fila].value);//Se copia el path
            }
            else{
                printf("Error in levelLoader.c, readAssetModifiers function : too many characters in sprite 4 %s\n", (decodedFile[fila].value));
                return -1;   
            }
        }
    }
    if(decodedFile[fila].parameter[0] == 0 && strcmp(decodedFile[fila].parameter, "END") != 0){
        printf("Error in levelLoader.c, readAssetModifiers function : END line not founded\n");
        return -1;
    }
    return fila;
}

int readLevelSettings(int checkAllFields, char * file, level_setting_t * levelSettings){ // Carga los levelsettings de un archivo de nivel, si checkAllFields esta en 1 revisa todos los campos, sino solo los que se encuentren
    if(readFile(file) != 0){ //Si no se pudo leer correctamente
        return -1;      //Devuelve error
    }
    int paramNo;
    int xMin;                   //-xMax: coordenada maxima en x alcanzable.
    int xMax;                   //-xMin: coordenada minima en x alcanzable.
    int yMin;                   //-YMax: coordenada maxima en Y alcanzable.
    int yMax;                   //-YMin: coordenada minima en Y alcanzable.
    
    char xMin_found = 0;
    char xMax_found = 0;
    char yMin_found = 0;
    char yMax_found = 0;
    char saltoX_found = 0;
    char saltoY_found = 0;
    char velBalas_found = 0;
    char margenX_found = 0;
    char margenY_found = 0;
    char velAliens_found = 0;
    char velMothership_found = 0;
    char desplazamientoX_found = 0;
    char desplazamientoY_found = 0;
    char desplazamientoUsr_found = 0;
    char mothershipYpos_found = 0;         //Posicion en Y donde se mueve la mothership (horizontal)
    char mothershipAsset_found = 0;        //Asset correspondiente a la mothership
    char maxMShipXLevel_found = 0;         //Maxima cantidad de motherships por nivel
    /*
    char usrLives_found = 0;
    */


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
        else if((saltoX_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "saltoX") == 0){
            levelSettings->saltoX = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->saltoX);
            saltoX_found++;
        }
        else if((saltoY_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "saltoY") == 0){
            levelSettings->saltoY = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->saltoY);
            saltoY_found++;
        }
        else if((velBalas_found == 0 || !checkAllFields)&& strcmp(decodedFile[paramNo].parameter, "velBalas") == 0){
            levelSettings->velBalas = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->velBalas);
            velBalas_found++;
        }
        else if((margenX_found == 0 || !checkAllFields)&& strcmp(decodedFile[paramNo].parameter, "margenX") == 0){
            levelSettings->margenX = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->margenX);
            margenX_found++;
        }
        else if((margenY_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "margenY") == 0){
            levelSettings->margenY = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->margenY);
            margenY_found++;
        }
        else if((velAliens_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "velAliens") == 0){
            levelSettings->velAliens = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->velAliens);
            velAliens_found++;
        }
        else if((velMothership_found == 0 || !checkAllFields)&& strcmp(decodedFile[paramNo].parameter, "velMothership") == 0){
            levelSettings->velMothership = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->velMothership);
            velMothership_found++;
        }
        else if((desplazamientoX_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "desplazamientoX") == 0){
            levelSettings->desplazamientoX = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->desplazamientoX);
            desplazamientoX_found++;
        }
        else if((desplazamientoY_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "desplazamientoY") == 0){
            levelSettings->desplazamientoY = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->desplazamientoY);
            desplazamientoY_found++;
        }
        else if((desplazamientoUsr_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "desplazamientoUsr") == 0){
            levelSettings->desplazamientoUsr = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->desplazamientoUsr);
            desplazamientoUsr_found++;
        }
        
        else if((maxMShipXLevel_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "maxMShipXLevel") == 0){
            levelSettings->maxMShipXLevel = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->maxMShipXLevel);
            maxMShipXLevel_found++;
        }
        else if((mothershipAsset_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "mothershipAsset") == 0){
            levelSettings->mothershipAsset = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->mothershipAsset);
            mothershipAsset_found++;
        }
        else if((mothershipYpos_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "mothershipYpos") == 0){
            levelSettings->mothershipYpos = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->mothershipYpos);
            mothershipYpos_found++;
        }
        /*
        else if((usrLives_found == 0 || !checkAllFields) && strcmp(decodedFile[paramNo].parameter, "usrLives") == 0){
            levelSettings->usrLives = getAbsValue(!checkAllFields, decodedFile[paramNo].value, levelSettings->usrLives);
            usrLives_found++;
        }
        */
    }
    //Si checkAllFields esta activado y no se encontraron todos los campos tira error
    if(checkAllFields && (xMin_found != 1 || xMax_found != 1 || yMin_found != 1 || yMax_found != 1 || saltoX_found != 1 || saltoY_found != 1 || velBalas_found != 1 || margenX_found != 1 || margenY_found != 1 || velAliens_found != 1 || velMothership_found != 1 || desplazamientoX_found != 1 || desplazamientoY_found != 1 || desplazamientoUsr_found != 1 || maxMShipXLevel_found != 1 || mothershipAsset_found != 1 || mothershipYpos_found != 1)){   //Si no se encontraron todos los campos devuelve error
        printf("Error in levelLoader.c, readLevelSettings function : \"%s\" has missing parameters\n", file);
        return -1;
    }
    //Si se cambian las constantes del display en un nivel que no es el 0, osea durante el juego, tira error
    if(checkAllFields == 0 && (xMax_found || xMin_found || yMax_found || yMin_found)){
        printf("Error in levelLoader.c, readLevelSettings function : Display constants found in a non-zero level\n");
        return -1;
    }
    else if(checkAllFields){
        levelSettings->xMax = xMax;
        levelSettings->xMin = xMin;
        levelSettings->yMax = yMax;
        levelSettings->yMin = yMin;
    }
    return 0;
}

static int getAbsValue(int relativeMode, char * valueReaded, int previousValue){   //Funcion que devuelve el valor de un valor corregido segun si es relativo o absoluto
    enum states {ABSOLUTE,RELATIVE_MUL, RELATIVE_ADD, RELATIVE_OPP, RELATIVE_DIV};   
    int state = ABSOLUTE;
    int value, number;
    if(!strncmp(valueReaded,"*=",2)){   //Se fija si los primeros dos caracteres de valueReaded es un modificador relativo de tipo multiplicacion
        state = RELATIVE_MUL;       //Es multiplicacion
    }
    else if(!strncmp(valueReaded,"+=",2)){
        state = RELATIVE_ADD;       //Es adicion
    }
    else if(!strcmp(valueReaded,"-")){
        state = RELATIVE_OPP;       //Es el opuesto
    }
    else if(!strncmp(valueReaded,"/=",2)){
        state = RELATIVE_DIV;       //Es division
    }
    if(relativeMode == 0 && (state == RELATIVE_MUL || state == RELATIVE_ADD || state == RELATIVE_OPP || state == RELATIVE_DIV)){   //Si el modo relativo no estaba activado y recibe valores en modo relativo tira error
        printf("Warning in levelLoader.c, getAbsValue function : found a relative modificator  \"%s\"  with relativeMode off, Value = 0 vas set\n", valueReaded);
        return 0;
    }
    
    switch(state){
        case RELATIVE_MUL:  //Si es una multiplicacion relativa, se lee el numero esquivando el *= y se guarda el valor como el valor previo por el numero
            number = atoi(valueReaded+2);
            value = previousValue*number;
            break;
        case RELATIVE_ADD:
            number = atoi(valueReaded+2);
            value = previousValue+number;
            break;
        case RELATIVE_DIV:
            number = atoi(valueReaded+2);
            value = previousValue/number;
            break;
        case RELATIVE_OPP:  //Si es el opuesto se devuelve el valor previo cambiado de signo
            value = -previousValue; 
            break;
        case ABSOLUTE:      //Si es el v
            number = atoi(valueReaded);
            value = number;
    }
    return value;
}

int loadLevel(int levelNo, level_t levelArray[], level_setting_t * levelSettings, char * platform, object_t ** listaAliens, object_t ** listaUsr, object_t ** listaBarreras){
    char levelFile[MAX_DIR_LENGTH+MAX_FILE_NAME];
    int level0True =  levelNo? 0 : 1;
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
    int levelIndex = getLevelNoOfArray(levelArray, levelNo);    //Se carga el index del nivel
    if(levelIndex == -1){
        printf("Error in levelLoader.c, loadLevel function : Level %d not found\n", levelNo);
        return -1;
    }
    sprintf(levelFile, "%s/%s", LEVELSDIR, &(levelArray[levelIndex].levelName[0])); //Genera el string del archivo a leer
    if(readLevelSettings(level0True, levelFile,  levelSettings) == -1){ //Carga el levelSettings
        printf("LEVEL SETTING ATRODEN YMAX %d, YMIN %d, XMAX %d, XMIN %d\n", levelSettings->yMax, levelSettings->yMin, levelSettings->xMax, levelSettings->xMin);
        printf("Error in levelLoader.c, loadLevel function : readLevelSettings failed at level No %d\n", levelNo);
        return -1;
    }
    else{
        if(readFile(levelFile) == -1){      //Lee el archivo
            printf("Error in levelLoader.c, loadLevel function : readFile failed at level No %d\n", levelNo);
            if(levelNo == 0){       
                return -1;                      //Si no se pudo leer termina la ejecucion
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
                    if(objType == NULL){
                        printf("Error in levelLoader.c, loadLevel function : Alien Asset (with type %d) not found in line %d of file %s\n",alien.type, fila, levelFile);
                        return -1;
                    }
                    (* listaAliens) = addObj((* listaAliens), alien.pos, alien.type, objType->initLives);    //Se agrega a la lista
                    if(checkOutOfBounds(alien.pos,objType->ancho,objType->alto,levelSettings)){
                        printf("Warning in levelLoader.c, loadLevel function : Alien Asset (with type %d) is out of bounds (x = %d | y = %d)\n", objType->id, alien.pos.x, alien.pos.y);
                    }
                }
                else if(strcmp(decodedFile[fila].parameter, "BARRERA") == 0){  //Si es una barrera
                    fila++;     //Se incrementa la fila
                    object_t barrera;                     //Buffer de la barrera a leer
                    fila = readObj(fila, &barrera);       //Lee la barrera
                    if(fila == -1){    //Si no se pudo leer termina
                        return -1;
                    }    
                    objectType_t * objType = getObjType(barrera.type);        //Se recupera el tipo de barrera
                    if(objType == NULL){
                        printf("Error in levelLoader.c, loadLevel function : Barrera Asset (with type %d) not found in line %d of file %s\n",barrera.type, fila, levelFile);
                        return -1;
                    }
                    (* listaBarreras) = addObj((* listaBarreras), barrera.pos, barrera.type, objType->initLives);    //Se agrega a la lista
                    if(checkOutOfBounds(barrera.pos,objType->ancho,objType->alto,levelSettings)){
                        printf("Warning in levelLoader.c, loadLevel function : Barrera Asset (with type %d) is out of bounds (x = %d | y = %d)\n", objType->id, barrera.pos.x, barrera.pos.y);
                    }
                }
                else if(strcmp(decodedFile[fila].parameter, "USUARIO") == 0){  //Si es el usuario
                    fila++;     //Se incrementa la fila
                    object_t usuario;                     //Buffer del usuario a leer
                    fila = readObj(fila, &usuario);       //Lee el usuario
                    if(fila == -1){    //Si no se pudo leer termina
                        return -1;
                    }    
                    objectType_t * objType = getObjType(usuario.type);        //Se recupera el tipo de usuario
                    if(objType == NULL){
                        printf("Error in levelLoader.c, loadLevel function : Usuario Asset (with type %d) not found in line %d of file %s\n", usuario.type, fila, levelFile);
                        return -1;
                    }
                    (* listaUsr) = addObj((* listaUsr), usuario.pos, usuario.type, objType->initLives);    //Se agrega a la lista
                    if(checkOutOfBounds(usuario.pos,objType->ancho,objType->alto,levelSettings)){
                        printf("Error in levelLoader.c, loadLevel function : Usuario Asset (with type %d) is out of bounds (x = %d | y = %d)\n", usuario.type, usuario.pos.x, usuario.pos.y);
                    }
                }
                /*
                else if(strcmp(decodedFile[fila].parameter, "MOTHERSHIP") == 0){  //Si es la nave nodria
                    fila++;     //Se incrementa la fila
                    object_t mothership;                     //Buffer de la nave nodriza a leer
                    fila = readObj(fila, &mothership);       //Lee la nave nodriza
                    if(fila == -1){    //Si no se pudo leer termina
                        return -1;
                    }    
                    objectType_t * objType = getObjType(mothership.type);        //Se recupera el tipo de nave nodriza
                    (* listMothership) = addObj((* listMothership), mothership.pos, mothership.type, objType->initLives);    //Se agrega a la lista
                }
                */
                else if(strcmp(decodedFile[fila].parameter, "ASSET_MODIFY") == 0){  //Si se quiere modificar un asset
                    int assetID = atoi(decodedFile[fila].value);
                    if(assetID == 0){
                        printf("Error in levelLoader.c, loadLevel function : Asset identificator not valid in file %s\n", levelFile);
                        return -1;
                    }
                    fila = readAssetModifiers(fila+1, assetID); //Se leen los modificadores del asset
                    printf("ASSETMODIFIER\n");
                    if(fila == -1){
                        printf("Error in levelLoader.c, loadLevel function : Couldn't modify Asset %d in file %s\n", assetID, levelFile);
                        return -1;
                    }
                }
                fila++;
            }
        }
    }
    return 0;
}


static int checkOutOfBounds(vector_t posicion, int ancho, int alto, level_setting_t * levelSettings){       //Devuelve 1 si el objeto esta out of bounds
    return( posicion.x +1 < levelSettings->xMin || posicion.x + ancho > levelSettings->xMax +1 || posicion.y + alto > levelSettings->yMax +1 || posicion.y +1 < levelSettings->yMin );
}

int readObj(int paramNo, object_t * objOut){
    int type_found = 0;
    int posX_found = 0;
    int posY_found = 0;
    int fila;
    if(strlen(decodedFile[paramNo].parameter) == 0){
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

int getLevelNoOfArray(level_t levelArray[], int levelNumber){
    if(levelArray == NULL){
        printf("Error in levelLoader.c, getLevelNoOfArray function : levelArray is NULL\n");
        return -1;
    }
    int nivel;  //Se crea un contador de niveles
    for(nivel = 0; levelArray[nivel].lastLevelTrue != 1 && nivel < MAX_LEVEL; nivel++){
        if (levelArray[nivel].level == levelNumber){
            return nivel;
        }
    }
    printf("Error in levelLoader.c, getLevelNoOfArray function : level %d not found in the array\n", levelNumber);
     return -1;
}


int stringEndCmp(char * string, char * end){    //Devuelve 1 si dos strings terminan de la misma manera
    int lenString = strlen(string); //variable auxiliar que guarda la longitud del string
    int lenEnd = strlen(end);       //Variable auxiliar que guarda la longitud del final a comparar
    return !strncmp(string + lenString-lenEnd, end, lenEnd) && (lenEnd <= lenString); //Devuelve 1 si el sufijo es igual y si es mas corto que el string
}


#ifdef LEVELTESTER
int main(int argc, char *argv[]){
    char gameDirectory[MAX_DIR_LENGTH] = {};
    if(argc != 2){
        printf("Error no se ingreso el directorio /game correctamente\n");
        return -1;
    }
    strcpy(gameDirectory, argv[1]);
    printf("Carpeta Game ingresada %s\n", gameDirectory);
    char levelsDirec[MAX_DIR_LENGTH+MAX_DIR_LENGTH] = {};
    char assetDirec[MAX_DIR_LENGTH+MAX_DIR_LENGTH] = {};
    sprintf(levelsDirec,"%s/levels", gameDirectory);
    sprintf(assetDirec,"%s/assets", gameDirectory);
    object_t * alienList = NULL;
    object_t * UsrList = NULL;
    object_t * barrerasList = NULL;
    level_setting_t levelSettings;
    #define MAX 100
    printf("Bienvenido al lector de niveles....\n");
    char plataforma[MAX]= "123";
    do{
        if(strlen(plataforma) != 3){
            printf("Error introduzca plataforma valida\n");
        }
        printf("Introduzca la plataforma\n");
        scanf("%s", plataforma);  
    }
    while(strlen(plataforma) != 3);
    directory_t carpetaNiveles = {};
    level_t levelArray[100];
    indexAllLevels(plataforma,levelsDirec, "_level", &carpetaNiveles, levelArray);
    imprimirNIVELES(levelArray);
    int nivel;
    int nivel_found = 0;
    do{
        if(strlen(plataforma) != 3){
            printf("Error introduzca plataforma valida\n");
        }
        printf("Seleccione el numero de nivel a leer\n");
        scanf("%d", &nivel);  
        int contador;
        for(contador = 0; levelArray[contador].lastLevelTrue != 1 && nivel_found == 0; contador++){
            if(levelArray[contador].level == nivel){
                nivel_found = 1;
            }
        }
        if(nivel_found == 0){
            printf("Nivel no valido\n");
        }
    }
    while(nivel_found == 0);
    int levelStatus;
    directory_t carpetaAssets = {};
    loadDirectory(assetDirec, &carpetaAssets);   
    if(loadAllAssets(plataforma, &carpetaAssets) == -1){
        printf("Error no se pudieron cargar los assets\n");
        return -1;
        }
    else{
        printf("\n\n...Assets cargados correctamente...\n\n");
    }
    levelStatus = loadLevel(0, levelArray, &levelSettings, &(plataforma[0]), &alienList, &UsrList, &barrerasList);
    if(levelStatus == -1){
        printf("Error no se encontro el nivel 0\n");
        return -1;
        }
    else{
        printf("\n\n...Nivel 0 cargado correctamente...\n\n");
    }
    levelStatus = loadLevel(nivel,levelArray, &levelSettings, &(plataforma[0]), &alienList, &UsrList, &barrerasList);
    if(levelStatus == -1){
        printf("Error no se encontro el nivel %d\n", nivel);
        return -1;
    }
    else{
        printf("\n\n...Nivel %d cargado correctamente...\n\n", nivel);
    }
    char matrix[80][80]={};
    int maxX, maxY;
    int divisor = 1;
    if(strcmp(plataforma,"rpi") == 0){
        maxX = 15;
        maxY = 15;
    }
    else if(strcmp(plataforma,"lnx") == 0){
        maxX = 80;
        maxY = 60;
        divisor = 10;
    }
 
    object_t * lista = alienList;
    objectType_t * objectType;
    
    while(lista != NULL){
        objectType = getObjType(lista->type);
        int pintX;
        int pintY;
        for(pintY = (lista->pos.y)/divisor; pintY < (objectType->alto)/divisor + lista->pos.y && pintY <= maxY; pintY++){
           //printf("%p %d\n\n", lista, pintY);
            for(pintX = (lista->pos.x)/divisor; pintX < (objectType->ancho)/divisor + lista->pos.x && pintX <= maxX; pintX++){
            matrix[pintY][pintX]++;
            //printf("        %p %d\n\n", lista, pintX);
            }
        }
       // printf("it 1\n");
        lista = lista->next;
    }
    lista = UsrList;
    while(lista != NULL){
        objectType = getObjType(lista->type);
        int pintX;
        int pintY;
        for(pintY = (lista->pos.y)/divisor; pintY < (objectType->alto)/divisor + lista->pos.y && pintY <= maxY; pintY++){
           //printf("%p %d\n\n", lista, pintY);
            for(pintX = (lista->pos.x)/divisor; pintX < (objectType->ancho)/divisor + lista->pos.x && pintX <= maxX; pintX++){
            matrix[pintY][pintX]++;
            //printf("        %p %d\n\n", lista, pintX);
            }
        }
       // printf("it 1\n");
        lista = lista->next;
    }
    lista = barrerasList;
    while(lista != NULL){
        objectType = getObjType(lista->type);
        int pintX;
        int pintY;
        for(pintY = (lista->pos.y)/divisor; pintY < (objectType->alto)/divisor + lista->pos.y && pintY <= maxY; pintY++){
           //printf("%p %d\n\n", lista, pintY);
            for(pintX = (lista->pos.x)/divisor; pintX < (objectType->ancho)/divisor + lista->pos.x && pintX <= maxX; pintX++){
            matrix[pintY][pintX]++;
            //printf("        %p %d\n\n", lista, pintX);
            }
        }
       // printf("it 1\n");
        lista = lista->next;
    }
    int fila;
    int columna;
        for(fila = 0; fila <= maxX; fila++){
            for(columna = 0; columna <= maxY; columna++){
                printf(" %d", matrix[fila][columna]);
            }
            printf("\n");
        }
    imprimirARRAY();
    printf("\n\n");
    imprimirLevelSetting(&levelSettings);
}
#endif



void imprimirNIVELES(level_t levelArray[]){ //Funcion que imprime los datos de cada nivel
    int level;  //Contador de niveles
    for(level = 0; levelArray[level].lastLevelTrue != 1 && level <= MAX_LEVEL; level++){    //Para cada nivel
        printf("Nivel %d:  .level= %d \t .levelName= %s\n", level, levelArray[level].level, levelArray[level].levelName);   //Se muestra en consola
    }
    if(level >= MAX_LEVEL){ //Si hay mas niveles que el maximo disponible tira error
        printf("Error in levelLoader.c, imprimirNIVELES function : terminator of levels: lastLevelTrue not found");
    }
}

