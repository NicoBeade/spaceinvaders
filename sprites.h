/**********************************************************************************************************************************************************
 * 
                                                 ___  _ __   _ _  (_) | |_   ___   ___     | |_  
                                                (_0< | '_ \ | '_| | | |  _| / 0_) (_0<  _  | ' \ 
                                                /__/ | .__/ |_|   |_|  \__| \___| /__/ (_) |_||_|
                                                     |_|                                                                              
 * 
 ***********************************************************************************************************************************************************
*   Este .h contiene tipos de datos, matrices constantes para el manejo de los sprites, caracteres, pantallas y menues del juego en raspi
*
 **********************************************************************************************************************************************************/

#ifndef SPRITES_RASPI_H
#define SPRITES_RASPI_H

#include <stdint.h>

//Hola

#define OFFSETLETRA 0
#define OFFSETALIEN 39
#define OFFSETCHARESP 36
#define OFFSETNUM 26


/*******************************************************************************************************************************************
 *                               _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_0<   / _` | / 0_)   | |) | / _` | |  _| / _ \ (_0<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/

typedef const uint8_t sprite_t [2][3]; //matriz de 2x3, tamanyo de los enemigos
typedef const uint8_t fullDisp_t [16][16]; //matriz de 16x16, para imprimir sobre todo el display
typedef const uint8_t caracteres_t [8][4]; //matriz de 8x4 para imprimir letras
typedef uint8_t halfDisp_t [8][16]; //matriz de 8x16 para imprimir sobre la mitad del display


/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 *                                                   ___                _   _              
                                                    / __|  _ __   _ _  (_) | |_   ___   ___
                                                    \__ \ | '_ \ | '_| | | |  _| / 0_) (_0<
                                                    |___/ | .__/ |_|   |_|  \__| \___| /__/
                                                          |_|                              
*
********************************************************************************************************************************************/
sprite_t daniel1 = {{1,0,1},{1,1,1}}; //2 sprites para cada tipo de enemigo
sprite_t daniel2 = {{1,1,1},{1,0,1}};

sprite_t pablo1 ={{1,1,0},{1,0,1}};
sprite_t pablo2 ={{0,1,1},{1,0,1}};

sprite_t nicolas1 ={{1,0,1},{0,1,0}};
sprite_t nicolas2 ={{0,1,0},{1,0,1}};

sprite_t nave = {{0,1,0},{1,1,1}}; //1 sprite unico para la nave del usuario


/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
 * 
                                         ___          _   _   ___    _               _                    
                                        | __|  _  _  | | | | |   \  (_)  ___  _ __  | |  __ _   _  _   ___
                                        | _|  | || | | | | | | |) | | | (_0< | '_ \ | | / _` | | || | (_0<
                                        |_|    \_,_| |_| |_| |___/  |_| /__/ | .__/ |_| \__,_|  \_, | /__/
                                                                             |_|                |__/   
*
********************************************************************************************************************************************/
fullDisp_t alienSpaceInvaders1 = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
    {0,0,0,1,1,0,1,1,1,1,0,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
    {0,0,1,0,1,1,1,1,1,1,1,1,0,1,0,0},
    {0,0,1,0,1,0,0,0,0,0,0,1,0,1,0,0},
    {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
/*
██████████████████████████████████████████████████
█████████████░░░▓████████████████▒░░▒█████████████
█████████████▒▒▒▒▓▓▓██████████▓▓▓▒▒▒▓█████████████
████████████████▒░░░██████████░░░▓████████████████
█████████████▒▒▒░░░░▒▒▒▒▒▒▒▒▒▒░░░░▒▒▓█████████████
█████████████░░░░░░░░░░░░░░░░░░░░░░░▒█████████████
█████████▓░░░░░░▒███░░░░░░░░░░███▒░░░░░░██████████
█████████▓░░░░░░▒██▓░░░░░░░░░░███▒░░░░░░▓█████████
██████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒██████
██████░░░░▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░▓▓▓░░░▒██████
██████░░░▒███░░░░░░░░░░░░░░░░░░░░░░░▒███░░░▒██████
██████░░░▒███░░░▒████████████████▒░░▒███░░░▒██████
██████▒░░▒███▒░░▒████████████████▒░░▒███▒░░▓██████
████████████████▒░░░░░░███▓░░░░░░▓████████████████
████████████████▓▓▓▓▓▓▓████▓▓▓▓▓▓█████████████████
*/
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
 *
                                                 _             _    __   ___    _             
                                                | |_    __ _  | |  / _| |   \  (_)  ___  _ __ 
                                                | ' \  / _` | | | |  _| | |) | | | (_0< | '_ \
                                                |_||_| \__,_| |_| |_|   |___/  |_| /__/ | .__/
                                                                                        |_|   
*
********************************************************************************************************************************************/

halfDisp_t halfDispTrophy = {
    {0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
    {0,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0},
    {0,0,0,1,0,1,1,0,0,1,1,1,0,1,0,0},
    {0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0},
    {0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
};

halfDisp_t halfDispAlienSpaceInvaders = {
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
    {0,0,0,1,1,0,1,1,1,1,0,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
    {0,0,1,0,1,1,1,1,1,1,1,1,0,1,0,0},
    {0,0,1,0,1,0,0,0,0,0,0,1,0,1,0,0},
    {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
};

halfDisp_t halfDispVolume = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0},
    {0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,1,1,0,1,1,0,0,0},
    {0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0},
};

halfDisp_t halfDispResume = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
};

halfDisp_t halfDispRestart = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
};


/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                             ___                            _                          
                                            / __|  __ _   _ _   __ _   __  | |_   ___   _ _   ___   ___
                                           | (__  / _` | | '_| / _` | / _| |  _| / 0_) | '_| / 0_) (_0<
                                            \___| \__,_| |_|   \__,_| \__|  \__| \___| |_|   \___| /__/
*
********************************************************************************************************************************************/

caracteres_t alienChar1 ={
    {0,0,1,0},
    {0,0,0,1},
    {0,0,1,1},
    {0,1,1,0},
    {1,1,1,1},
    {1,0,1,1},
    {1,0,1,0},
    {0,0,0,1},
};

caracteres_t alienChar2 ={
    {0,0,0,0},
    {0,0,0,1},
    {1,1,1,1},
    {1,1,1,0},
    {1,1,1,1},
    {1,1,1,1},
    {0,0,0,0},
    {1,0,1,1},
};

caracteres_t alienChar3 ={
    {1,0,0,0},
    {0,0,0,0},
    {1,0,0,0},
    {1,1,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
};

caracteres_t letraA ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraB ={ 
    {0,0,0,0},
    {1,1,0,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraC ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraD ={
    {0,0,0,0},
    {1,1,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraE ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraF ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraG ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraH ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraI ={
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraJ ={
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraK ={
    {0,0,0,0},
    {1,0,0,0},
    {1,0,1,0},
    {1,1,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraL ={
    {0,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraM ={
    {0,0,0,0},
    {1,1,1,0},
    {1,1,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraN ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraO ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraP ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraQ ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraR ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,1,0,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraS ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraT ={
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraU ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraV ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraW ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,1,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraX ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,1,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraY ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraZ ={
    {0,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,1,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra0 ={
    {0,0,0,0},
    {0,1,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,0,1,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra1 ={
    {0,0,0,0},
    {0,1,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra2 ={
    {0,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra3 ={
    {0,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra4 ={
    {0,0,0,0},
    {1,0,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra5 ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,0,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra6 ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra7 ={
    {0,0,0,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra8 ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letra9 ={
    {0,0,0,0},
    {1,1,1,0},
    {1,0,1,0},
    {1,1,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraGuion ={
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraPunto ={
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

caracteres_t letraEspacio ={
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};


caracteres_t* alfabeto [42] =  {&letraA,&letraB,&letraC,&letraD,&letraE,&letraF,&letraG,&letraH,
&letraI,&letraJ,&letraK,&letraL,&letraM,&letraN,&letraO,&letraP,&letraQ,&letraR,&letraS,&letraT,
&letraU,&letraV,&letraW,&letraX,&letraY,&letraZ,&letra0,&letra1,&letra2,&letra3,&letra4,&letra5,
&letra6,&letra7,&letra8,&letra9,&letraGuion,&letraPunto,&letraEspacio,&alienChar1,&alienChar2,&alienChar3};


/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

#endif