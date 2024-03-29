#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include "../spaceLib/spaceLib.h"
#define MAX_PARAM_LETTERS 40        //Maximo tamaño de char que ocupa un parametro
#define MAX_VALUE_LETTERS 60        //Maximo tamaño de char que ocupa un valor
#define MAX_FILE_ROWS 350           //Maxima cantidad de filas de un archivo

#define MAX_FILE_ROW_LENGHT 100     //Maximo tamaño de fila para un archivo

#define MAX_FILE_NAME 150        //Maximo tamaño de archivo

#define MAX_FILES_IN_FOLDER 50  //Maxima cantidad de archivos en una carpeta

#define ASSETSDIR "game/assets"
#define LEVELSDIR "game/levels"
#define LEVELPREFIX "_level"
#define ASSETPREFIX "_"

#define LEVELFORMAT ".level"
#define ASSETFORMAT ".asset"

#define MAX_DIR_LENGTH 150       //Maxaima longitud de un directorio

#define MAX_LEVEL 100       //Maximo nivel
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
int loadAllAssets(char * platform, directory_t * directoryStore);
int loadAllAssets(char * platform,  directory_t * directoryStore);
int indexAllLevels(char * platform, char * levelsDir, char * levelPrefix, directory_t * directoryStore, level_t levelArray[]);
int levelArrayLen(level_t levelArray[]); //Calcula la longitud del array de niveles
int readObj(int paramNo, object_t * objOut);
int readLevelSettings(int checkAllFields, char * file, level_setting_t * levelSettings);
int loadLevel(int levelNo, level_t levelArray[], level_setting_t * levelSettings, char * platform, object_t ** listaAliens, object_t ** listaUsr, object_t ** listaBarreras);
void imprimirNIVELES(level_t levelArray[]);
int getLevelNoOfArray(level_t levelArray[], int levelNumber);
#endif