/**********************************************************************************************************************************************************
 * 
                                                              _   _                      _     
                                                             (_) | |                    | |    
                                         ___   _ __    _ __   _  | |_    ___   ___      | |__  
                                        / __| | '_ \  | '__| | | | __|  / _ \ / __|     | '_ \ 
                                        \__ \ | |_) | | |    | | | |_  |  __/ \__ \  _  | | | |
                                        |___/ | .__/  |_|    |_|  \__|  \___| |___/ (_) |_| |_|
                                            | |                                              
                                            |_|                                                                               
 * 
 ***********************************************************************************************************************************************************
*   Este .h contiene tipos de datos, matrices constantes para el manejo de los sprites, caracteres, pantallas y menues del juego en raspi
*
 **********************************************************************************************************************************************************/

#ifndef SPRITES_RASPI_H
#define SPRITES_RASPI_H

#include <stdint.h>

/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
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
 * 
                                     ___                     _                   _              
                                    / __|  ___   _ _    ___ | |_   __ _   _ _   | |_   ___   ___
                                   | (__  / _ \ | ' \  (_-< |  _| / _` | | ' \  |  _| / -_) (_-<
                                    \___| \___/ |_||_| /__/  \__| \__,_| |_||_|  \__| \___| /__/
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/
enum SPRITESALIENS {DANIEL, PABLO, NICOLAS};

#define FRAMERATE 4 //tasa de refresco del display
#define FIRSTROW(a) (((a)==1)? 0: 7) //Macro para el barrido vertical de las letras. Ubica la primer linea en funcion del sentido

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                             ___         _                           
                                            | __| __ __ | |_   ___   _ _   _ _    ___
                                            | _|  \ \ / |  _| / -_) | '_| | ' \  (_-<
                                            |___| /_\_\  \__| \___| |_|   |_||_| /__/                                                                                                                      
 * 
 ******************************************************************************************************************************************/

extern sprite_t daniel1;
extern sprite_t daniel2;
extern sprite_t pablo1;
extern sprite_t pablo2;
extern sprite_t nicolas1;
extern sprite_t nicolas2;
extern sprite_t nave;

extern fullDisp_t alienSpaceInvaders1;
extern halfDisp_t halfDispTrophy;
extern halfDisp_t halfDispAlienSpaceInvaders;
extern halfDisp_t halfDispVolume;
extern halfDisp_t halfDispResume;
extern halfDisp_t halfDispRestart;
extern halfDisp_t halfDispAAA;
extern halfDisp_t halfDispSelectLevels;
extern halfDisp_t halfDispSaveScore;
extern halfDisp_t halfDispQuitGame;
extern halfDisp_t halfDispHome;
extern halfDisp_t halfDispLostGame;
extern halfDisp_t halfDispNextLevel;

extern caracteres_t alienChar1;
extern caracteres_t alienChar2;
extern caracteres_t alienChar3;
extern caracteres_t letraA;
extern caracteres_t letraB;
extern caracteres_t letraC;
extern caracteres_t letraD;
extern caracteres_t letraE;
extern caracteres_t letraF;
extern caracteres_t letraG;
extern caracteres_t letraH;
extern caracteres_t letraI;
extern caracteres_t letraJ;
extern caracteres_t letraK;
extern caracteres_t letraL;
extern caracteres_t letraM;
extern caracteres_t letraN;
extern caracteres_t letraO;
extern caracteres_t letraP;
extern caracteres_t letraQ;
extern caracteres_t letraR;
extern caracteres_t letraS;
extern caracteres_t letraT;
extern caracteres_t letraU;
extern caracteres_t letraV;
extern caracteres_t letraW;
extern caracteres_t letraX;
extern caracteres_t letraY;
extern caracteres_t letraZ;
extern caracteres_t letra0;
extern caracteres_t letra1;
extern caracteres_t letra2;
extern caracteres_t letra3;
extern caracteres_t letra4;
extern caracteres_t letra5;
extern caracteres_t letra6;
extern caracteres_t letra7;
extern caracteres_t letra8;
extern caracteres_t letra9;
extern caracteres_t letraGuion;
extern caracteres_t letraPunto;
extern caracteres_t letraEspacio;
extern caracteres_t flechaScore;

extern caracteres_t* alfabeto [43];
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

#endif

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/