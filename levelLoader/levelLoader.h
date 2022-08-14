#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#define MAX_PARAM_LETTERS 30
#define MAX_VALUE_LETTERS 50
#define MAX_FILE_ROWS 100

#define MAX_FILE_ROW_LENGHT 100

#define MAX_FILE_NAME 30

#define MAX_FILES_IN_FOLDER 30

#define ASSETSDIR "../game/assets"


typedef char fileName_t[MAX_FILE_NAME];

typedef fileName_t directory_t[MAX_FILES_IN_FOLDER];



void clearFileBuffer(void);
int readFile(char * file);
void printFile(void);
int loadAsset(char * file);
int loadDirectory(char * carpeta, directory_t * directoryStore);
int loadAllAssets(char * platform, directory_t * directoryStore);

#endif