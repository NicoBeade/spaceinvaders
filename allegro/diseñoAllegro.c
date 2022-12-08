#include "allegro.h"
#include "allegroUtilities.h"

/************************************************************************************************
 * 
 *                              CONSTANTES
 * 
*************************************************************************************************/

#define MENUX 80   //Posicion x del menu
#define MENUY 300   //Posicion y del menu
#define ESPACIADOMENU 120   //Espaciado entre opciones del menu
#define SELECTOR 50     //Espacio entre el selector y la opcion seleccionada

#define CANTOP 5
#define SPACELETX 70
#define SPACELETY 70 
#define SCOREX X_MAX/2
#define SCOREY 70
#define LETRAX X_MAX/2 - TAMLETRAX*(0.5) - SPACELETX
#define LETRAY 200

#define FIRSTX X_MAX/2
#define FIRSTY 70
#define LEFTCOLUMNX 100
#define COLUMNY 220
#define RIGHTCOLUMNX X_MAX-300


/***********************************************************************************************************************************************************
 * 
 *                                                                      MENUES
 * 
 * ********************************************************************************************************************************************************/

TextObj_t * allegroMenu(menu_t * data, TextObj_t * lists){
//Esta funcion se utiliza para mostrar los menues en pantalla
    
    int i;
    int titleLen = strlen(data->titulo);
    TextObj_t salida = {NULL,NULL};

    salida.textoList=addText(salida.textoList, data->titulo, largeF, (X_MAX/2) - (titleLen/2)*36 + 50, 50);

    for( i = 0; i<data->cantOpciones; i++){
        //Se agregan los textos de las opciones a la lista de textos
        if(i==0){
            salida.textoList=addText(salida.textoList, data->textOpciones[i], largeF, MENUX + SELECTOR, MENUY + i * ESPACIADOMENU);
        }
        else{
            salida.textoList=addText(salida.textoList, data->textOpciones[i], mediumF, MENUX , MENUY + i * ESPACIADOMENU);
        }
    }
    //se agrega el selector
    salida.textoList = addText(salida.textoList, ">", largeF, MENUX, MENUY);

    salida.spriteList = addSprite(salida.spriteList, ((data->spritesDir)[0]), X_MAX - 400, Y_MAX/2);
    if(data->titulo == NULL){
        salida.spriteList = addSprite(salida.spriteList, "game/spritesAllegro/titleMM.png", X_MAX/2, 0);
    };
    lists->spriteList= salida.spriteList;
    lists->textoList= salida.textoList;

    return lists;
}

void changeOption(void * dataIn){
//Esta funcion realiza la animacion de cambio de oipcion

    if(!dataIn){
        printf("puntero a datos NULL\n");
    }

    if(GAME_STATUS.menuActual != MENU_VOLUME){
        changeOptionData_t * data = (changeOptionData_t *) dataIn;
        texto_t * puntero = (*data->toText)->next;
        sprite_t * sprite = *data->toSprite;
        
        int i = 0;
        int esc = 0; //Esta variable se encarga de saber si hay que subir o bajar las opciones

        if(GAME_STATUS.menuActual != MENU_LEADERBOARD){
            if(data->actualOp == 0 && data->nextOp == ((data->menu)->cantOpciones)-1){

                esc = -(data->menu)->cantOpciones + 1; //Si estoy en la primer opcion tengo que subir todo

            }else if(data->actualOp == ((data->menu)->cantOpciones)-1 && data->nextOp == 0){

                esc = (data->menu)->cantOpciones - 1; //Si estoy en la ultima opcion tengo que bajar todo

            }else if(data->nextOp > data->actualOp){ //En las otras situaciones muevo todo un lugar para arriba o abajo

                esc = -1; 

            }else{

                esc = 1;

            }
            
            for(i = 0; i < (data->menu)->cantOpciones; i++ ){
                //Recorro la lista
                puntero->posy += esc * ESPACIADOMENU; //Muevo todo segun la variable esc

                if(i == data->actualOp){
                    puntero->posx-= SELECTOR; //La opcion actual la muevo hacia la izquierda
                    puntero->fuente = mediumF; //Reduzco el tamaño de letras
                }
                if(i == data->nextOp){
                    puntero->posx += SELECTOR; //La nueva opcion la muevo hacia la derecha
                    puntero->fuente = largeF; //Incremento el tamaño de letras
                }
                puntero = puntero->next;
            }

            if(((data->menu)->spritesDir)[0] != NULL){
                sprite->direccion = ((data->menu)->spritesDir)[data->nextOp];
            }
        }
    }
}

/***********************************************************************************************************************************************************
 * 
 *                                                                      SCORE
 * 
 * ********************************************************************************************************************************************************/

TextObj_t * allegroScore(TextObj_t * lists, char* scoreActual, char letras[15][2]){
    
    int i, j;
    float scoreLen = strlen(scoreActual);
    TextObj_t salida = {NULL,NULL};

    for(i = 0; i < 3; i++){
    //Se recorren las columnas
        for(j = 0; j < CANTOP; j++){
            //Se recorren las letras que se muestran en cada columna
            //Se añaden los textos que permiten mostrar las letras

            if(j == (CANTOP/2)){
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], largeF, LETRAX + SPACELETX*i - 5, LETRAY + SPACELETY*j);
            }else if(j == 0){
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], smallF, LETRAX + SPACELETX*i + 3, LETRAY + SPACELETY*j+40);
            }else if(j == CANTOP-1){
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], smallF, LETRAX + SPACELETX*i + 3, LETRAY + SPACELETY*j-15);
            }else if(j == 1){
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], mediumF, LETRAX + SPACELETX*i, LETRAY + SPACELETY*j+5);
            }else{
                salida.textoList = addText(salida.textoList, letras[i*CANTOP+j], mediumF, LETRAX + SPACELETX*i, LETRAY + SPACELETY*j+5);  
            }
        }
    }
    //salida.textoList = addText(salida.textoList, ">", largeF, LETRAX - 35 , LETRAY + SPACELETY* 2); //se añade el cursor
    salida.textoList = addText(salida.textoList, scoreActual, bigF, SCOREX - (scoreLen/2.0)*58, SCOREY); //se añade el score 
    
    salida.spriteList = addSprite(salida.spriteList, "game/spritesAllegro/Selector1.png", LETRAX -15, LETRAY + 123);
    lists->textoList= salida.textoList;
    lists->spriteList=salida.spriteList;
    
    return lists;
}

void changeLetra(char letras[15][2], int colActual, int dir){

    int i;
    char (* puntero)[2] = &(letras[0]); //Se crea un puntero temporal
    char str[2]={0,0}; //Se crea un string temporal

    puntero += colActual*CANTOP; //Se mueve el puntero hacia la primer letra a cambiar

    for(i = 0; i < CANTOP; i++){
        
        strcpy(str, *puntero); //Se guarda la letra a cambiar en el string temporal

        str[0]+=dir; //Se modifica el string temporal

        if(dir<0){
            switch (str[0]){//Chequea que no se pase de los caracteres posibles.
                case 'A' - 1:
                    str[0] = '9';
                    break;
                case '0' - 1:
                    str[0] = ' ';
                    break;
                case ' ' - 1: 
                    str[0] = 'Z';
                default:
                    break;
                }
        }else if(dir>0){
            switch (str[0]){//Chequea que no se pase de los caracteres posibles.
                case 'Z' + 1:
                    str[0] = ' ';
                    break;
                case ' ' + 1:
                    str[0] = '0';
                    break;
                case '9' + 1:
                    str[0] = 'A';
                default:
                    break;
                }
        }

        strcpy(*puntero,str); //Se modifica el string original
        puntero++;
    }

}

sprite_t * changeCol(sprite_t * toshow, int nextOp){

    if(!toshow){
        printf("puntero a lista de sprites NULL\n");
    }
    sprite_t * puntero = toshow; //Se crea un puntero temporal a la lista de textos

    puntero->posx = LETRAX - 15 + nextOp*SPACELETX; //Se mueve el selector
    return toshow;
}

/***********************************************************************************************************************************************************
 * 
 *                                                                      LIDERBOARD
 * 
 * ********************************************************************************************************************************************************/

TextObj_t * allegroLiderboard(menu_t * data, TextObj_t * lists){
    
    int i;
    TextObj_t salida = {NULL,NULL};

    int firstLen = strlen(data->textOpciones[0]);
    int secondLen = strlen(data->textOpciones[1]);

    salida.textoList=addText(salida.textoList, data->textOpciones[0], bigF, FIRSTX -(firstLen/2)*40 , FIRSTY);
    salida.textoList=addText(salida.textoList, data->textOpciones[1], largeF, FIRSTX -(secondLen/2)*25 , FIRSTY + 120);

    for(i=2; i < data->cantOpciones; i++){
        if(i%2 == 0){
            salida.textoList=addText(salida.textoList, data->textOpciones[i], mediumF, LEFTCOLUMNX , COLUMNY + 60*i/2);
        }else{
            salida.textoList=addText(salida.textoList, data->textOpciones[i], mediumF, RIGHTCOLUMNX , COLUMNY + 60* (i-1)/2);
        }
    }

    lists->spriteList= salida.spriteList;
    lists->textoList= salida.textoList;

    return lists;
}

/***********************************************************************************************************************************************************
 * 
 *                                                                      VOLUME
 * 
 * ********************************************************************************************************************************************************/

TextObj_t * allegroVolume(menu_t * data, TextObj_t * lists, int volumenActual){

    TextObj_t salida = {NULL,NULL};
    int titleLen = strlen(data->titulo);
    float volumeLen = strlen(data->textOpciones[volumenActual]);
    
    salida.textoList = addText(salida.textoList, data->textOpciones[volumenActual], bigF , (X_MAX*0.3) - (volumeLen + 1)*40, Y_MAX * 0.4);

    for(int i =0; i< volumenActual; i++){
        salida.spriteList = addSprite(salida.spriteList, "game/spritesAllegro/volumeBar.png", X_MAX * 0.3 + 50* i, Y_MAX * 0.4);
    }
    salida.textoList=addText(salida.textoList, data->titulo, largeF, (X_MAX/2) - (titleLen/2)*36, 50);

    lists->textoList= salida.textoList;
    lists->spriteList=salida.spriteList;

    return lists;
}

sprite_t * changeVolume(menu_t * data, texto_t * listText, sprite_t * listSprite, int volumenActual){

    if(!listText){
        printf("puntero a lista de textos NULL\n");
    }
    listText->texto = data->textOpciones[volumenActual];

    listSprite = emptySprite(listSprite);
    
    for(int i =0; i< volumenActual; i++){
        listSprite = addSprite(listSprite, "game/spritesAllegro/volumeBar.png", X_MAX * 0.3 + 50* i, Y_MAX * 0.4);
    }

    return listSprite;
}
/***********************************************************************************************************************************************************
 * 
 *                                                                      LEVELS
 * 
 * ********************************************************************************************************************************************************/

texto_t * levelAllegro(texto_t * toText, char * score, char * vidas ){

    texto_t * temp = toText;

    temp = addText(temp, vidas, mediumF, X_MAX - 50, Y_MAX - 50);
    temp = addText(temp, "Lives :", mediumF, X_MAX - 200, Y_MAX - 50);
    temp = addText(temp, score, mediumF, 20, Y_MAX - 50);

    return temp;
}

int expo(int num, int exp){

    int temp = num;
    if (exp == 0){
        return 1;
    }
    else{
        for (int i = 0; i < exp - 1; i++){
            num = num * temp;
        }
    }

    return num;
}

int techo(float num){

    int i;
    for(i=0; i<10; i++){
        if(num < i+1 && num >= i){
            return i;
        }
    }
    return 0;
}

void refreshDatos( char * toScore, char * toVidas, int score, int vidas){

    int  i; 
    float temp;
    toVidas[0] = '0' + vidas;

    for( i = 0; i < 6; i++){

        temp = (float)((score % (expo(10, i)*10)) - (score % expo(10, i)));

        toScore[5 - i] = '0' + techo(temp / expo(10, i));
        
    }

}
