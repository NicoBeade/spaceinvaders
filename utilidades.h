/**********************************************************************************************************************************************************
 * 
                                         _     _   _   _       _               _                    _     
                                        | |   (_) | | (_)     | |             | |                  | |    
                                 _   _  | |_   _  | |  _    __| |   __ _    __| |   ___   ___      | |__  
                                | | | | | __| | | | | | |  / _` |  / _` |  / _` |  / _ \ / __|     | '_ \ 
                                | |_| | | |_  | | | | | | | (_| | | (_| | | (_| | |  __/ \__ \  _  | | | |
                                 \__,_|  \__| |_| |_| |_|  \__,_|  \__,_|  \__,_|  \___| |___/ (_) |_| |_|
                                                                                            
 * 
 ***********************************************************************************************************************************************************
*   Este .h contiene tipos de datos y constantes que son utilizados en varios archivos del programa.
*
 **********************************************************************************************************************************************************/

 #ifndef UTILIDADES_H
 #define UTILIDADES_H

 #include "aliensYBalas.h"

/*******************************************************************************************************************************************
 * 
                                 _____   _                          _           ___           _              
                                |_   _| (_)  _ __   ___   ___    __| |  ___    |   \   __ _  | |_   ___   ___
                                  | |   | | | '_ \ / _ \ (_-<   / _` | / -_)   | |) | / _` | |  _| / _ \ (_-<
                                  |_|   |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__,_|  \__| \___/ /__/
                                            |_|                                                          
 * 
 ******************************************************************************************************************************************/
typedef enum objectTypes {DANIEL, PABLO, NICOLAS, NAVE, BARRERA, BALA_DANIEL, BALA_PABLO, BALA_NICOLAS, BALA_USUARIO, BARRERA_ESQUINA_SUP_IZQ, BARRERA_ESQUINA_SUP_DER, BARRERA_INTERNO, BARRERA_ESQUINA_INF_IZQ, BARRERA_ESQUINA_INF_DER, NONE} types_t;


typedef struct{//Datos utilizados al estar en algun menu.
    int x;//Movimiento en X e Y.
    int y;
    char press;// Presion del boton.
    int cantOpciones;//Cantidad total de opciones en el menu.
    void (*changeOption)(int opcionActual);//Callback a la funcion que cambia la opcion seleccionada.
    int (*selectOption)(int opcionActual);//Callback a la funcion que selecciona la opcion seleccionada.
}punteroMenu_t;

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


//*************DIRECCIONES
#define IZQUIERDA -1        //Constantes utilizadas para indicar la direccion en la que se deben mover los aliens.
#define DERECHA 1
#define ABAJO 2

//*************TIMER
#define U_SEC2M_SEC 1000    //Conversion de micro segundos a milisegundos.

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******************************************************************************************************************************************
 * 
                                             __  __                              
                                            |  \/  |  __ _   __   _ _   ___   ___
                                            | |\/| | / _` | / _| | '_| / _ \ (_-<
                                            |_|  |_| \__,_| \__| |_|   \___/ /__/                                                    
 * 
 ******************************************************************************************************************************************/

//--------------------------------------------------------------------Macros para acceder a los campos de levelSettings:
//Estas macros se utilizan para desreferenciar de forma mas sencilla al utilizar levelSettings en un thread.
#define X_MIN_L(levelSettings)                  (((level_setting_t *)levelSettings) -> xMin)
#define X_MAX_L(levelSettings)                  (((level_setting_t *)levelSettings) -> xMax)
#define Y_MIN_L(levelSettings)                  (((level_setting_t *)levelSettings) -> yMin)
#define Y_MAX_L(levelSettings)                  (((level_setting_t *)levelSettings) -> yMax)
#define SALTO_X_L(levelSettings)                (((level_setting_t *)levelSettings) -> saltoX)
#define SALTO_Y_L(levelSettings)                (((level_setting_t *)levelSettings) -> saltoY)
#define DISTINICIAL_X_L(levelSettings)          (((level_setting_t *)levelSettings) -> distInicialX)
#define DISTINICIAL_Y_L(levelSettings)          (((level_setting_t *)levelSettings) -> distInicialY)
#define BARRERAINICIAL_X_L(levelSettings)       (((level_setting_t *)levelSettings) -> barreraInicialX)
#define BARRERAINICIAL_Y_L(levelSettings)       (((level_setting_t *)levelSettings) -> barreraInicialY)
#define ANCHO_USR_L(levelSettings)              (((level_setting_t *)levelSettings) -> anchoUsr)
#define ALTO_USR_L(levelSettings)               (((level_setting_t *)levelSettings) -> altoUsr)
#define ANCHO_ALIEN_L(levelSettings)            (((level_setting_t *)levelSettings) -> anchoAlien)
#define ALTO_ALIEN_L(levelSettings)             (((level_setting_t *)levelSettings) -> altoAlien)
#define ANCHO_MINI_BARRERA_L(levelSettings)     (((level_setting_t *)levelSettings) -> anchoMiniBarrera)
#define ALTO_MINI_BARRERA_L(levelSettings)      (((level_setting_t *)levelSettings) -> altoMiniBarrera)
#define MARGEN_X_L(levelSettings)               (((level_setting_t *)levelSettings) -> margenX)
#define MARGEN_Y_L(levelSettings)               (((level_setting_t *)levelSettings) -> margenY)

#define MAX_USR_BULLETS_L(levelSettings)        (((level_setting_t *)levelSettings) -> maxUsrBullets)         
#define MAX_ENEMY_BULLETS_L(levelSettings)      (((level_setting_t *)levelSettings) -> maxEnemyBullets)  
#define SHOOT_PROB_DANI_L(levelSettings)        (((level_setting_t *)levelSettings) -> shootProbDani)        
#define SHOOT_PROB_PABLO_L(levelSettings)       (((level_setting_t *)levelSettings) -> shootProbPablo)   
#define SHOOT_PROB_NICO_L(levelSettings)        (((level_setting_t *)levelSettings) -> shootProbNico)         
#define INIT_USR_LIVES_L(levelSettings)         (((level_setting_t *)levelSettings) -> initUsrLives)        
#define INIT_DANIEL_LIVES_L(levelSettings)      (((level_setting_t *)levelSettings) -> initDanielLives)    
#define INIT_PABLO_LIVES_L(levelSettings)       (((level_setting_t *)levelSettings) -> initPabloLives)         
#define INIT_NICOLAS_LIVES_L(levelSettings)     (((level_setting_t *)levelSettings) -> initNicolasLives)       
#define MINI_BARRERA_LIVES(levelSettings)       (((level_setting_t *)levelSettings) -> miniBarreraLives)
#define DESPLAZAMIENTO_X_L(levelSettings)       (((level_setting_t *)levelSettings) -> desplazamientoX)
#define DESPLAZAMIENTO_Y_L(levelSettings)       (((level_setting_t *)levelSettings) -> desplazamientoY)
#define DESPLAZAMIENTO_USR_L(levelSettings)     (((level_setting_t *)levelSettings) -> desplazamientoUsr)

/******************************************************************************************************************************************
*******************************************************************************************************************************************/


 #endif//utilidades.h