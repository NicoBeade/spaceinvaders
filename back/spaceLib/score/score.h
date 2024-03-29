#ifndef SCORE_H
#define SCORE_H

//CONSTANTES

enum ADDSCORE {POS_LEADERBOARD_FOUND,POS_LEADERBOARD_NOT_FOUND=-1,ERROR_LEADERBOARD=-2};

#define LEADERBOARD 10
#define ROWSIZE 15
#define FILENAME_SCORE "game/score.txt"

//TIPOS DE DATOS

typedef char leaderboard_t [LEADERBOARD][ROWSIZE];

//PROTOTIPOS

void parseScore(leaderboard_t leaderboard); //parsea el archivo de score ya existente 
void saveScore(leaderboard_t leaderboard); //sobreescribe el archivo de score con el nuevo puntaje
int addScore(leaderboard_t leaderboard, int score, char* name); //anyade un nuevo puntaje



#endif