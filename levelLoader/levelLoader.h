#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include "../spaceLib/spaceLib.h"
#define MAX_PARAM_LETTERS 30        //Maximo tamaño de char que ocupa un parametro
#define MAX_VALUE_LETTERS 50        //Maximo tamaño de char que ocupa un valor
#define MAX_FILE_ROWS 100           //Maxima cantidad de filas de un archivo

#define MAX_FILE_ROW_LENGHT 100     //Maximo tamaño de fila para un archivo

#define MAX_FILE_NAME 50        //Maximo tamaño de archivo

#define MAX_FILES_IN_FOLDER 100  //Maxima cantidad de archivos en una carpeta

#define ASSETSDIR "../game/assets"
#define LEVELSDIR "../game/levels"

#define LEVELPREFIX "_level"
#define ASSETPREFIX "_"

#define LEVELFORMAT ".level"
#define ASSETFORMAT ".asset"

#define MAX_DIR_LENGTH 150       //Maxaima longitud de un directorio

#define MAX_LEVEL 999       //Maximo nivel
typedef char fileName_t[MAX_FILE_NAME];

typedef fileName_t directory_t[MAX_FILES_IN_FOLDER];

typedef struct{
    int level;
    char levelName[MAX_FILE_NAME];
    char lastLevelTrue;
}level_t;

void clearFileBuffer(void);
int readFile(char * file);
void printFile(void);
int loadAsset(char * file);
int loadDirectory(char * carpeta, directory_t * directoryStore);
<<<<<<< HEAD
int loadAllAssets(char * platform, directory_t * directoryStore);
=======
int loadAllAssets(char * platform,  directory_t * directoryStore);
>>>>>>> 94b59aa01b00cdddc23faa0b489587379a0b303a
int indexAllLevels(char * platform, char * levelPrefix, directory_t * directoryStore, level_t levelArray[]);
int readObj(int paramNo, object_t * objOut);
int readLevelSettings(int checkAllFields, char * file, level_setting_t * levelSettings);
int loadLevel(int levelNo, level_setting_t * levelSettings, char * platform, object_t ** listaAliens, object_t ** listaUsr, object_t ** listaBarreras);
int stringEndCmp(char * string, char * end);
int getLevelNoOfFile(int prefixLenghtToIgnore, char * fileName, int maxFileLenght, char * nameOut);
void imprimirNIVELES(level_t levelArray[]);
#endif