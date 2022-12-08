 #ifndef DISENOALLEGRO_H
 #define DISENOALLEGRO_H

#include "allegro.h"
#include "allegroUtilities.h"

/************************************************************************************************
 * 
 *                              CONSTANTES
 * 
*************************************************************************************************
*  Estas constantes se utilizan para el diseño de los menues
*
**************************************************************/

#define MENUX 80   //Posicion x del menu
#define MENUY 300   //Posicion y del menu
#define ESPACIADOMENU 120   //Espaciado entre opciones del menu
#define SELECTOR 50     //Espacio entre el selector y la opcion seleccionada

#define CANTOP 5 //Cantidad de letras por columnas en el menu save score
#define SPACELETX 70    //Espaciado en X entre las columnas
#define SPACELETY 70    //Espaciado en Y entre las columnas
#define SCOREX X_MAX/2  //Posicion X del score en pantalla
#define SCOREY 70       //Posicion Y del score en pantalla
#define LETRAX X_MAX/2 - TAMLETRAX*(0.5) - SPACELETX //Posicion en X de la primer columna
#define LETRAY 200  //Posicion en Y de la primer columna

#define FIRSTX X_MAX/2  //Posicion X de la posicion 1 del leaderboard
#define FIRSTY 70       //Posicion Y de la posicion 1 del leaderboard
#define LEFTCOLUMNX 50 //Posicion X de la columna izquierda del leaderboard
#define COLUMNY 250     //Posicion Y de las columnas del leaderboard
#define RIGHTCOLUMNX X_MAX-300 //Posicion X de la columna derecha del leaderboard

 #endif