/**********************************************************************************************************************************************************
 * 
                                    _   _               _                 ___                      _          
                                 __| | (_)  ___  _ __  | |  __ _   _  _  | _ \  __ _   ___  _ __  (_)      __ 
                                / _` | | | (_-< | '_ \ | | / _` | | || | |   / / _` | (_-< | '_ \ | |  _  / _|
                                \__,_| |_| /__/ | .__/ |_| \__,_|  \_, | |_|_\ \__,_| /__/ | .__/ |_| (_) \__|
                                                |_|                |__/                    |_|                
* 
 ***********************************************************************************************************************************************************
*   
*   Este archivo contiene las funciones y threads encargadas de todas las acciones relacionadas al manejo del display RPI.
*   Tiene las siguientes funcionalidades:
*       -Funciones para agregar informacion al buffer e imprimir cosas en el display.
*       -Thread que se encarga de actualizar el display durante la ejecucion del juego en la raspberry.
*       -Thread que se encarga del barrido de texto en el display durante la ejecucion de un menu (menu de inicio, pausa, etc.).
*       -Funciones que indican que hacer cuando se selecciona una opcion en un menu.
*
 **********************************************************************************************************************************************************/


#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "displayRaspi.h"
#include <stdio.h>


/*int main (void){
    dcoord_t p = {5,5};
    disp_init();
    disp_clear();
    drawSprite(p,daniel1);
    disp_update()
    usleep(500*1000);
    cleanEnemy(p);
    drawSprite(p,daniel2);
    disp_update();
    return 0;
}
*/ //Ejemplo de main 


/*******************************************************************************************************************************************
 * 
             ___               _           _     _                          _           ___   _            _     _           
            | _ \  _ _   ___  | |_   ___  | |_  (_)  _ __   ___   ___    __| |  ___    / __| | |_   __ _  | |_  (_)  __   ___
            |  _/ | '_| / _ \ |  _| / _ \ |  _| | | | '_ \ / _ \ (_-<   / _` | / -_)   \__ \ |  _| / _` | |  _| | | / _| (_-<
            |_|   |_|   \___/  \__| \___/  \__| |_| | .__/ \___/ /__/   \__,_| \___|   |___/  \__| \__,_|  \__| |_| \__| /__/
                                                    |_|                                                                                                                                                                                                               
 * 
 ******************************************************************************************************************************************/
//*****************THREAD DISPLAY DURANTE MENUES
static int offsetAlfabeto(char caracter); //Se utiliza para obtener el offset necesario para acceder al string "alfabeto".
static void swipeCharacter(halfDisp_t* lowerDispMenu, caracteres_t caracter, int direccion); //Agrega un caracter completo al buffer.
static void copyMatrixLetter(char,uint8_t [8][4]); //Copia de matrices de letra
static void sweepMatrix(uint8_t [8][4], int ); //Barrido de letra x1 linea 
static void addRow(uint8_t [8][4], uint8_t [4], int ); //Agrega una linea en la matriz con las celdas correspondientes
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/******************************************************************************************************************************************
 * 
  ___                      _                                _         _        _   _               _                   ___   ___   ___ 
 | __|  _  _   _ _    __  (_)  ___   _ _    ___   ___    __| |  ___  | |    __| | (_)  ___  _ __  | |  __ _   _  _    | _ \ | _ \ |_ _|
 | _|  | || | | ' \  / _| | | / _ \ | ' \  / -_) (_-<   / _` | / -_) | |   / _` | | | (_-< | '_ \ | | / _` | | || |   |   / |  _/  | | 
 |_|    \_,_| |_||_| \__| |_| \___/ |_||_| \___| /__/   \__,_| \___| |_|   \__,_| |_| /__/ | .__/ |_| \__,_|  \_, |   |_|_\ |_|   |___|
                                                                                           | |
 * 
 ******************************************************************************************************************************************/

//IMPORTANTE: AL DIBUJAR O LIMPIAR EL DISPLAY HAY QUE LLAMAR A disp_update() Y DEBE HABER SIDO INICIALIZADO CON disp_init()

void drawSprite(dcoord_t p, sprite_t alien){ //Esta funcion imprime en display un enemigo en un sprite dados en la posicion p
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<=2 ; i++){
        for(j=0 ; j<=1 ; j++){
            if (alien [j][i] == 1){
                pAux.x = p.x+i;
                pAux.y = p.y+j;
                if(pAux.x>15||pAux.y>15)printf("Fuera de ranfo de impresion en algun pixel de la nave");
                disp_write(pAux,D_ON); //Actualiza el buffer          
            }
        }
    }
}

void cleanSprite(dcoord_t p){ //Esta funcion borra en display un enemigo (tienen todos el mismo tamanyo) en la posicion p
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<=2 ; i++){
        for(j=0 ; j<=1 ; j++){
            pAux.x = p.x+i; 
            pAux.y = p.y+j;
            if(pAux.x>15||pAux.y>15){
            printf("Fuera de rango de impresion en algun pixel de la nave");
            }
            disp_write(pAux ,D_OFF); //Actualiza el buffer
        }
    }
}

void clearBuffer(void){ //Esta funcion imprime en display un enemigo en un sprite dados en la posicion p
    uint8_t i,j;
    dcoord_t pAux;
    for (i=0 ; i<16 ; i++){
        for(j=0 ; j<16 ; j++){
            pAux.x = i;
            pAux.y = j;
            disp_write(pAux, D_OFF); //Actualiza el buffer          
        }
    }
}

void printLetter(caracteres_t letter, dcoord_t coordenada){ //imprime una letra barriendo los 32 pixeles de una matriz de 8X4
    int i,j;
    dcoord_t punto;
    for (i=0; i<8; i++){
        for (j=0; j<4; j++){
            punto.x=j+coordenada.x;
            punto.y=i+coordenada.y;
            if (letter [punto.y][punto.x]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    sem_wait(&SEM_DRIVER);
    disp_update();
    sem_post(&SEM_DRIVER);
}

void printFullDisp(fullDisp_t displaySprite){ //imprime toda la pantalla barriendo los 256 pixeles de una matriz de 16x16
    int i,j;
    dcoord_t punto;
    for (i=0; i<16; i++){
        for (j=0; j<16; j++){
            punto.x=j;
            punto.y=i;
            if (displaySprite[i][j]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    sem_wait(&SEM_DRIVER);
    disp_update();
    sem_post(&SEM_DRIVER);
}

void printHalfDisp(halfDisp_t halfDispSprite, char mitad){ //imprime la mitad de la pantalla barriendo los 128 pixeles de una matriz
    int i,j,offset;                                        //se puede elegir cual mitad del display ingresando I (inf) o S (sup)
    if (mitad=='I'){
        offset=8;
    }
    else if(mitad=='S'){
        offset=0;
    }
    else {
        printf("Letra no valida para la impresion de medio display\n");
        return;
    }
    dcoord_t punto;
    for (i=0; i<8; i++){
        for (j=0; j<16; j++){
            punto.x=j;
            punto.y=i+offset;
            if (halfDispSprite [i][j]==1){
                disp_write(punto,D_ON);
            }
            else{
                disp_write(punto,D_OFF);
            }
        }
    }
    sem_wait(&SEM_DRIVER);
    disp_update();
    sem_post(&SEM_DRIVER);
}
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

/******************************************************************************************************************************************
 * 
  _____   _                            _        _         _        _   _               _                   ___   ___   ___ 
 |_   _| | |_    _ _   ___   __ _   __| |    __| |  ___  | |    __| | (_)  ___  _ __  | |  __ _   _  _    | _ \ | _ \ |_ _|
   | |   | ' \  | '_| / -_) / _` | / _` |   / _` | / -_) | |   / _` | | | (_-< | '_ \ | | / _` | | || |   |   / |  _/  | | 
   |_|   |_||_| |_|   \___| \__,_| \__,_|   \__,_| \___| |_|   \__,_| |_| /__/ | .__/ |_| \__,_|  \_, |   |_|_\ |_|   |___|
                                                                               |_|                |__/                     
 * 
 ******************************************************************************************************************************************/

void* displayRPIThread (void* argDisplayRPI){

    object_t* balasEnemigas;
    object_t* balasUsr;
    object_t* aliens;
    object_t* naveUser;
    object_t* barriers;
    object_t* mothership;

    dcoord_t punto; //punto del display a escribir

    while(GAME_STATUS.inGame){
        usleep(10 * U_SEC2M_SEC);//Espera 10mS para igualar el tiempo del timer.
        if( (timerTick % FRAMERATE) == 0 && GAME_STATUS.inGame ){
            sem_wait(&SEM_GAME);

            balasEnemigas = *(((argDisplayRPI_t*)argDisplayRPI)->balasEnemigas); //Puntero a la lista de balas enemigas
            balasUsr = *(((argDisplayRPI_t*)argDisplayRPI)->balasUsr); //Puntero a la lista de balas del usuario
            aliens = *(((argDisplayRPI_t*)argDisplayRPI)->aliens); //Puntero a la lista de aliens
            naveUser = *(((argDisplayRPI_t*)argDisplayRPI)->naveUser); //Puntero a la nave del usuario
            barriers = *(((argDisplayRPI_t*)argDisplayRPI)->barriers); //Puntero a la nave nodriza
            mothership = *(((argDisplayRPI_t*)argDisplayRPI)->mothership); //Puntero a la nave nodriza

            clearBuffer(); //limpio el buffer            
            //Actualizo el buffer con la nueva posicion de los aliens
            object_t* aux = aliens;
            objectType_t * objTypePointer;      //Variable auxiliar que almacena el objectType de cada elemento
            while (aliens!= NULL){ //mientras no se haya llegado al final de la lista

                punto.x=aliens->pos.x; //se definen posiciones en x y en y de los aliens, tomando como pivote la esquina superior izquierda
                punto.y=aliens->pos.y;

                if (punto.x>15||punto.y>15||punto.x<0||punto.y<0){
                    printf("Fuera de rango de impresion en la nave\n"); //chequeo de pixel a imprimir
                }
                objTypePointer = getObjType(aliens->type);
                if(objTypePointer == NULL){ //Si no encuentra el object type devuelve null
                    printf("Error in displayRaspi.c, displayRPIThread function : Object type %d not found\n", aliens->type);
                    pthread_exit(0);
                }
                int sprite1 = atoi(objTypePointer->sprite1);
                int sprite2 = atoi(objTypePointer->sprite2);
                int sprite;
                if(sprite1 < 1 || sprite1 > MAX_SPRITES){
                   printf("Error in displayRaspi.c,231 displayRPIThread function : sprite: %s (atoi %d, id %d) has an invalid format\n", objTypePointer->sprite1, sprite1, objTypePointer->id);
                   sem_post(&SEM_GAME);
                   pthread_exit(0); 
                }
                if(sprite1 < 1 || sprite2 > MAX_SPRITES){
                   printf("Error in displayRaspi.c,231 displayRPIThread function : sprite: %s (atoi %d, id %d) has an invalid format\n", objTypePointer->sprite2, sprite2, objTypePointer->id);
                   sem_post(&SEM_GAME);
                   pthread_exit(0);
                }
                if (aliens->animationStatus%2){ //chequeo de estado de animacion, se imprime un sprite u otro dependiendo de cuantas veces se haya desplazado un alien
                    sprite = sprite1;
                }
                else{
                    sprite = sprite2;
                }
                switch(sprite){ //dependiendo del estado de animacion y el alien a imprimir, se imprime un sprite distinto
                    case 1:
                        drawSprite(punto,daniel1);
                        break;
                    case 2:
                        drawSprite(punto,daniel2);
                        break;
                    case 3:
                        drawSprite(punto,nicolas1);
                        break;
                    case 4:
                        drawSprite(punto,nicolas2);
                        break;
                    case 5:
                        drawSprite(punto,pablo1);
                        break;
                    case 6:
                        drawSprite(punto,pablo2);
                        break;
                    
                    default: printf("Se esta queriendo imprimir como alien algo que no es un alien");
                        break;
                    
                }   
                aliens=aliens->next; //se pasa al siguiente alien en la lista
            }
            aliens=aux; //devuelve aliens al principio de la lista

            punto.x=naveUser->pos.x; //posicion en x y en y de la nave
            punto.y=naveUser->pos.y;
            drawSprite(punto,nave); //copia la nave en el buffer
            
            aux = balasEnemigas;

            while (balasEnemigas!= NULL){ //mientras no se haya llegado al final de la lista

                punto.x=balasEnemigas->pos.x; //se definen posiciones en x y en y de las balas, tomando como pivote la esquina superior izquierda
                punto.y=balasEnemigas->pos.y;
                if (punto.x>15||punto.y>15||punto.x<0||punto.y<0){
                    printf("Fuera de rango de impresion en la bala enemiga \n"); //chequea de pixel a imprimir
                }
                else{   
                    disp_write(punto,D_ON);
                }
                balasEnemigas = balasEnemigas -> next;
            }
            balasEnemigas = aux;

            aux = balasUsr;
            while (balasUsr!= NULL){ //mientras no se haya llegado al final de la lista

                punto.x=balasUsr->pos.x; //se definen posiciones en x y en y de las balas, tomando como pivote la esquina superior izquierda
                punto.y=balasUsr->pos.y;
                if (punto.x>15||punto.y>15||punto.x<0||punto.y<0){
                    printf("Fuera de rango de impresion en la bala aliada\n"); //chequea de pixel a imprimir
                }
                else{   
                    disp_write(punto,D_ON);
                }
                balasUsr = balasUsr -> next;
            }
            balasUsr = aux;

            aux = barriers;
            while (barriers!= NULL){ //mientras no se haya llegado al final de la lista

                punto.x=barriers->pos.x; //se definen posiciones en x y en y de las balas, tomando como pivote la esquina superior izquierda
                punto.y=barriers->pos.y;
                if (punto.x>15||punto.y>15||punto.x<0||punto.y<0){
                    printf("Fuera de rango de impresion de las barreras\n"); //chequea de pixel a imprimir
                }
                else{   
                    disp_write(punto,D_ON);
                }
                barriers = barriers -> next;
            }
            barriers = aux;
            punto.y=0;
            if(mothership->pos.x>0&&mothership->pos.x<15){
                punto.x=mothership->pos.x;
                disp_write(punto,D_ON);
            }
            if(mothership->pos.x+1>0&&mothership->pos.x+1<15){
                punto.x=mothership->pos.x+1;
                disp_write(punto,D_ON);
            }    
            if(mothership->pos.x+2>0&&mothership->pos.x+2<15){
                punto.x=mothership->pos.x+2;
                disp_write(punto,D_ON);
            }
                

            
            if(GAME_STATUS.inGame){
                sem_wait(&SEM_DRIVER);
                disp_update(); //se transfiere del buffer al display de la RPI
                sem_post(&SEM_DRIVER);
            }
            sem_post(&SEM_GAME);

        }
    }
    printf("Killed Display RPI\n");
    pthread_exit(0);
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/******************************************************************************************************************************************
 * 
             _____   _                            _        _   _               __  __                         ___   ___   ___ 
            |_   _| | |_    _ _   ___   __ _   __| |    __| | (_)  ___  _ __  |  \/  |  ___   _ _    _  _    | _ \ | _ \ |_ _|
              | |   | ' \  | '_| / -_) / _` | / _` |   / _` | | | (_-< | '_ \ | |\/| | / -_) | ' \  | || |   |   / |  _/  | | 
              |_|   |_||_| |_|   \___| \__,_| \__,_|   \__,_| |_| /__/ | .__/ |_|  |_| \___| |_||_|  \_,_|   |_|_\ |_|   |___|
                                                                       |_|                                                         
 * 
 ******************************************************************************************************************************************/

void* textAnimMenu(void* argTextAnimMenu){
//Este thread es el que se encarga de realizar el barrido de texto durante la ejecucion de un menu.
    
    int i, j, offset;
    int firstBarr = 4;
    pthread_t drawingSwipeT;

    int firstLetter = ( (((argTextAnimMenu_t*)argTextAnimMenu) -> direccion) == DERECHA ) ? 3 : 0;
    int lastLetter = ( (((argTextAnimMenu_t*)argTextAnimMenu) -> direccion) == DERECHA ) ? -1 : 4;
    
    //Primero imprimimos las primeras 4 letras y el dibujo.
    printf("Puntero al dibujo en textAnimMenu: %p", ((argTextAnimMenu_t*)argTextAnimMenu) -> drawing);
    argSwipeDrawing_t argSwipeDrawing = { ((argTextAnimMenu_t*)argTextAnimMenu) -> higherDispMenu, ((argTextAnimMenu_t*)argTextAnimMenu) -> direccion, ((argTextAnimMenu_t*)argTextAnimMenu) -> drawing };

    pthread_create(&drawingSwipeT, NULL, swipeDrawing, &argSwipeDrawing);//Agrega el dibujo.

    for(i = firstLetter ; i != lastLetter ; i -= (((argTextAnimMenu_t*)argTextAnimMenu) -> direccion)){

        offset = offsetAlfabeto((((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[i]);
        swipeCharacter(((argTextAnimMenu_t*)argTextAnimMenu) -> lowerDispMenu, *(alfabeto[offset]), ((argTextAnimMenu_t*)argTextAnimMenu) -> direccion);
    }
    
    pthread_join(drawingSwipeT, NULL);
    

    usleep(900 * U_SEC2M_SEC);//Espera medio segundo.

    if(velDispAnimation != 1){
        velDispAnimation = VEL_DISP_ANIMATION;
    }

    do{//Barre el texto hasta que se le indique lo contrario.
        for(j = firstBarr ; (((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[j] != '\0' ; j++){//Barre todas las letras del texto.

            offset = offsetAlfabeto((((argTextAnimMenu_t*)argTextAnimMenu) -> msg)[j]);
            
            swipeCharacter(((argTextAnimMenu_t*)argTextAnimMenu) -> lowerDispMenu, *(alfabeto[offset]), IZQUIERDA);       
        }
        firstBarr = 0;//Reinicia el proceso.
    }
    while(*(((argTextAnimMenu_t*)argTextAnimMenu) -> changeAnimation));

    velDispAnimation = VEL_DISP_ANIMATION;
    pthread_exit(0);
}


static int offsetAlfabeto(char caracter){
//Esta funcion se utiliza para obtener el offset necesario para acceder al string "alfabeto".
    int offset;
    
    if(caracter >= 'a' && caracter <= 'z'){//Si es una letra minuscula.
        offset = caracter - 'a' + OFFSETLETRA;
    }
    else if(caracter >= 'A' && caracter <= 'Z'){//Si es una letra mayuscula.
        offset = caracter - 'A' + OFFSETLETRA;
    }
    else if(caracter >= '0' && caracter <= '9'){//Si es un numero.
        offset = caracter - '0' + OFFSETNUM;
    }
    else if(caracter == '-'){//Si es un guion.
        offset = OFFSETCHARESP;
    }
    else if(caracter == '.'){//Si es un punto.
        offset = OFFSETCHARESP + 1;
    }
    else if(caracter == ' '){//Si es un espacio.
        offset = OFFSETCHARESP + 2;
    }
    else if(caracter == '>'){//Si es un espacio.
        offset = OFFSETCHARESP + 6;
    }
    return offset;
}

static void swipeCharacter(halfDisp_t* lowerDispMenu, caracteres_t caracter, int direccion){
//Esta funcion agrega un caracter completo al buffer.
    int i, fil, col, colInicialB, colFinalB, colInicialL, colFinalL;
    
    colInicialB = (direccion == DERECHA) ? 14 : 1;//Define a partir de que columna se realiza el barrido.
    colFinalB = (direccion == DERECHA) ? -1 : 16;//Define la ultima columna que se barrera.
    colInicialL = (direccion == DERECHA) ? 3 : 0;//Define a partir de que columna se muestra la nueva letra.
    colFinalL = (direccion == DERECHA) ? -1 : 4;//Define a la ultima columna de la letra que se mostrara.

    for(i = colInicialL ; i != colFinalL ; i -= direccion){//Recorre todas las columnas del caracter.

        for(col = colInicialB ; col != colFinalB ; col -= direccion){//Barre el display hacia un costado.
            usleep(velDispAnimation);//Indica a que velocidad se debe hacer el barrido.
            
            for(fil = 0 ; fil < 8 ; fil++){
                (*lowerDispMenu)[fil][col + direccion] = (*lowerDispMenu)[fil][col];//Realiza el barrido.
            }
        }

        for(fil = 0 ; fil < 8 ; fil++){//Ingresa la columna de la nueva letra al buffer.
            (*lowerDispMenu)[fil][colFinalB + direccion] = caracter[fil][i];
        }
        
        sem_wait(&SEM_MENU);
        printHalfDisp(*lowerDispMenu, 'I');//Muestra el contenido en el display.
        sem_post(&SEM_MENU);
    }
    
}

void* swipeDrawing(void* data){
/*Este thread se encarga de mostrar los dibujos de la parte superior del display durante un menu.
    Recibe un puntero a la parte superior del display, la direccion en la que se debe mover y un puntero al dibujo a mostrar.*/

    argSwipeDrawing_t* argSwipeDrawing = (argSwipeDrawing_t*)data;

    int i, fil, col, colInicialB, colFinalB, colInicialD, colFinalD;
    
    colInicialB = (argSwipeDrawing -> direccion == DERECHA) ? 14 : 1;//Define a partir de que columna se realiza el barrido.
    colFinalB = (argSwipeDrawing -> direccion == DERECHA) ? -1 : 16;//Define la ultima columna que se barrera.
    colInicialD = (argSwipeDrawing -> direccion == DERECHA) ? 15 : 0;//Define a partir de que columna se muestra la nueva letra.
    colFinalD = (argSwipeDrawing -> direccion == DERECHA) ? -1 : 16;//Define a la ultima columna de la letra que se mostrara.

    for(i = colInicialD ; i != colFinalD ; i -= argSwipeDrawing -> direccion){//Recorre todas las columnas del dibujo.

        for(col = colInicialB ; col != colFinalB ; col -= argSwipeDrawing -> direccion){//Barre el display hacia un costado.
            usleep(velDispAnimation);//Indica a que velocidad se debe hacer el barrido.
            
            for(fil = 0 ; fil < 8 ; fil++){
                (*( argSwipeDrawing -> higerDispMenu ))[fil][col + argSwipeDrawing -> direccion] = (*( argSwipeDrawing -> higerDispMenu ))[fil][col];//Realiza el barrido.
            }
        }

        for(fil = 0 ; fil < 8 ; fil++){//Ingresa la columna del dibujo.
            printf("puntero random: %p\n", argSwipeDrawing -> drawing);
            (*( argSwipeDrawing -> higerDispMenu ))[fil][colFinalB + argSwipeDrawing -> direccion] = (*( argSwipeDrawing -> drawing ))[fil][i];
            printf("Aca se buguea\n");
        }
        sem_wait(&SEM_MENU);
        printf("Antes de imprimir el display\n");
        printHalfDisp(*( argSwipeDrawing -> higerDispMenu ), 'S');//Muestra el contenido en el display.
        printf("Despues de imprimir el display\n");
        sem_post(&SEM_MENU);
    }
    pthread_exit(0);
}
/*******************************************************************************************************************************************
*******************************************************************************************************************************************/


/******************************************************************************************************************************************
 * 
                          _     _                _  _                    _   _                 ___                           
             ___   _ __  | |_  (_)  ___   _ _   | || |  __ _   _ _    __| | | |  ___   _ _    | _ \  __ _   _  _   ___  __ _ 
            / _ \ | '_ \ |  _| | | / _ \ | ' \  | __ | / _` | | ' \  / _` | | | / -_) | '_|   |  _/ / _` | | || | (_-< / _` |
            \___/ | .__/  \__| |_| \___/ |_||_| |_||_| \__,_| |_||_| \__,_| |_| \___| |_|     |_|   \__,_|  \_,_| /__/ \__,_|
                  |_|                                                                                                                                                             
 * 
 ******************************************************************************************************************************************/

void changeOption(void* argChangeOption){
//Esta funcion es la encargada de cambiar el texto que se muestra en pantalla en un menu.
    
    static argTextAnimMenu_t argTextAnimMenu;

    velDispAnimation = 1;

    *(((argChangeOption_t*)argChangeOption) -> animStatus) = 0;

    pthread_join(*(((argChangeOption_t*)argChangeOption) -> threadMenu), NULL);//Termina el thread anterior aumentando la velocidad del barrido.

    *(((argChangeOption_t*)argChangeOption) -> animStatus) = 1;

    argTextAnimMenu.msg = ((argChangeOption_t*)argChangeOption) -> nuevoTexto;
    argTextAnimMenu.lowerDispMenu = ((argChangeOption_t*)argChangeOption) -> lowerDispMenu;
    argTextAnimMenu.higherDispMenu = ((argChangeOption_t*)argChangeOption) -> higherDispMenu;
    argTextAnimMenu.drawing = ((argChangeOption_t*)argChangeOption) -> drawing;
    argTextAnimMenu.direccion = ((argChangeOption_t*)argChangeOption) -> direccion;
    argTextAnimMenu.changeAnimation = ((argChangeOption_t*)argChangeOption) -> animStatus;
                                        //Inicia el nuevo thread que mostrara el nuevo texto.

    pthread_create(((argChangeOption_t*)argChangeOption) -> threadMenu, NULL, textAnimMenu, &argTextAnimMenu);

}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/



/*******************************************************************************************************************************************
 * 
                                                     ___                        
                                                    / __|  __   ___   _ _   ___ 
                                                    \__ \ / _| / _ \ | '_| / -_)
                                                    |___/ \__| \___/ |_|   \___|
                                                                                                                                                            
 * 
 ******************************************************************************************************************************************/

halfDisp_t* getLeaderBoardName(halfDisp_t* nameDispMenu, int select){

    leaderboard_t leaderboard;
    parseScore(leaderboard);  //Se lee el leaderboard de score.txt

    char copiaLeaderboard [ROWSIZE];
    char name [5];
    int letra;
    int caracter = 1;
    
    strcpy(copiaLeaderboard, leaderboard[select]); //Accede al valor numerico de puntaje de cada posicion
    strtok(copiaLeaderboard," "); //Corta el texto del leaderboard hasta el espacio, se queda solo con el numero
    
    name [0] = '>'; //Agrega una flecha al principio del nombre
    name [4] = 0;  //Terminador
    for (letra=strlen(copiaLeaderboard)+1; leaderboard[select][letra]!=0; letra++){ //Copia el nombre del jugador en un string
        name[caracter]=leaderboard[select][letra];
        caracter++;
    }
    printf("%s\n",name);

    nameDispMenu = strToHalfDisplay(nameDispMenu, name); //Convierte el string a algo que se puede mostrar en el display.

    return nameDispMenu;
}


halfDisp_t* strToHalfDisplay(halfDisp_t * nombre, char* nombreStr){
//Esta funcion convierte un string de 4 letras en un halfDisplay_t.

    caracteres_t* caracter; //Variable que almacena el dibujo del caracter en pixeles.
    
    int offset; //Variable para desreferenciar en el arreglo de alfabeto.
    int i, j, k;
    int l = 0;

    for(i = 0; i < 4; i++){ //Recorre todas las letras del nombre
        offset = offsetAlfabeto(nombreStr[i]);
        caracter = alfabeto[offset];

        for(j = 0; j < 8; j++){//Recorre las filas del halfDisplay

            for(k = l; k < l+4; k++){//Recorre las columnas del halfDisplay
                (*nombre)[j][k] = (*caracter)[j][k-l]; //Copia el caracter en el halfDisplay
            }
        }

        l += 4;
    }

    return nombre;
}

void* letterFlashThread(void* data){
//Este thread se encarga de hacer titilar una letra en el display.

    letterFlash_t* letterFlash = (letterFlash_t*)data;

    caracteres_t* caracter; //Variable que almacena el dibujo del caracter en pixeles.
    
    int offset; //Variable para desreferenciar en el arreglo de alfabeto.
    int i, j;

    while(*(letterFlash->exitStatus)){
        offset = offsetAlfabeto(*(letterFlash->letra));
        caracter = alfabeto[offset];

        //Primero apaga todos los pixeles
        for(i = (letterFlash->pos)->y ; i < (letterFlash->pos)->y + 8 ; i++){//Recorre las filas

            for(j = (letterFlash->pos)->x ; j < (letterFlash->pos)->x + 4 ; j++){//Recorre las columnas
                (*(letterFlash->display))[i][j] = 0;
            }
        }
        sem_wait(&SEM_MENU);
        printHalfDisp(*(letterFlash->display), 'S');//Muestra el contenido en el display.
        sem_post(&SEM_MENU);

        usleep(VEL_TITILEO * U_SEC2M_SEC);

        //Luego imprime la letra
        for(i = (letterFlash->pos)->y ; i < (letterFlash->pos)->y + 8 ; i++){//Recorre las filas

            for(j = (letterFlash->pos)->x ; j < (letterFlash->pos)->x + 4 ; j++){//Recorre las columnas
                (*(letterFlash->display))[i][j] = (*caracter)[i][j - (letterFlash->pos)->x];
            }
        }
        sem_wait(&SEM_MENU);
        printHalfDisp(*(letterFlash->display), 'S');//Muestra el contenido en el display.
        sem_post(&SEM_MENU);

        usleep(2 * VEL_TITILEO * U_SEC2M_SEC);        
    }

    pthread_exit(0);
}


//BARRIDO LETRA: recibe la primer y segunda letra a barrer, el sentido del barrido y la coordenada a imprimir el barrido
//Esta funcion hace el barrido vertical de las letras en el display 
//Asumo el display ya correctamente incializado
void barridoLetra (char letraUno, char letraDos, int sentido, dcoord_t coordenada ){
    int i;
    uint8_t matriz [8][4];
    uint8_t matrizCopy [8][4];
    copyMatrixLetter(letraUno, matriz);
    copyMatrixLetter(letraDos, matrizCopy);
    for (i = 0; i<8 ; i++){
        sweepMatrix(matriz, sentido);
        addRow(matriz,matrizCopy[FIRSTROW(sentido)+i*sentido],sentido);
        printLetter(matriz, coordenada);
        usleep(20000);
    }
    return;
}


//Esta funcion agrega la linea correspondiente en la matriz al hacer el barrido vertical de las letras
static void addRow(uint8_t matriz [8][4], uint8_t rowToAdd [4], int sentido){
    int i;
    for (i=0; i<4; i++){
        matriz[FIRSTROW(-sentido)][i]=rowToAdd[i];
    }
    return;
}

//Esta funcion barre una fila la matriz indicada en la direccion correspondiente. La ultima fila va a ser sobreescrita por addRow
static void sweepMatrix(uint8_t matriz [8][4], int sentido){
    int i,j;
    for (j=0; j<7; j++){
        for(i=0; i<4; i++){
            matriz[FIRSTROW(sentido)+j*sentido][i]= matriz[FIRSTROW(sentido)+sentido+j*sentido][i];
        }
    }
}


//Esta matriz copia la matriz de letra indicada en la matriz correspondiente
static void copyMatrixLetter(char letra, uint8_t matriz [8][4]){
    int i,j;
    for (j=0; j<8; j++){
        for(i=0; i<4; i++){
            matriz[j][i]= (*(alfabeto[offsetAlfabeto(letra)]))[j][i];
        }
    }

    return;
}

/*******************************************************************************************************************************************
*******************************************************************************************************************************************/

