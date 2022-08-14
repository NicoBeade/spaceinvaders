#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#define MAX_PARAM_LETTERS 30
#define MAX_VALUE_LETTERS 50
#define MAX_FILE_ROWS 100

#define MAX_FILE_ROW_LENGHT 100


void clearFileBuffer(void);
int readFile(char * file);
void printFile(void);
int loadAsset(char * file);

#endif