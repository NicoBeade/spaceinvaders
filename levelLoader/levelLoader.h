#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#define MAX_PARAM_LETTERS 30        //Maximo tamaño de char que ocupa un parametro
#define MAX_VALUE_LETTERS 50        //Maximo tamaño de char que ocupa un valor
#define MAX_FILE_ROWS 100           //Maxima cantidad de filas de un archivo

#define MAX_FILE_ROW_LENGHT 100     //Maximo tamaño de fila para un archivo

#define MAX_FILE_NAME 30        //Maximo tamaño de archivo

#define MAX_FILES_IN_FOLDER 30  //Maxima cantidad de archivos en una carpeta

#define ASSETSDIR "../game/assets"
#define LEVELSDIR "../game/levels"

#define MAX_DIR_LENGTH 60       //Maxaima longitud de un directorio

#define MAX_LEVELS_NO_LENGTH 4 //Maxima cantidad de digitos que ocupa el numero de nivel
typedef char fileName_t[MAX_FILE_NAME];

typedef fileName_t directory_t[MAX_FILES_IN_FOLDER];



void clearFileBuffer(void);
int readFile(char * file);
void printFile(void);
int loadAsset(char * file);
int loadDirectory(char * carpeta, directory_t * directoryStore);
int loadAllAssets(char * platform, directory_t * directoryStore);

#endif