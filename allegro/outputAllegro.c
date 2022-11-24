/**********************************************************************************************************************************************************
 *
                          _   _                 _                                _   _                               
                         | | (_)               | |                       /\     | | | |                              
                       __| |  _   ___   _ __   | |   __ _   _   _       /  \    | | | |   ___    __ _   _ __    ___  
                      / _` | | | / __| | '_ \  | |  / _` | | | | |     / /\ \   | | | |  / _ \  / _` | | '__|  / _ \ 
                     | (_| | | | \__ \ | |_) | | | | (_| | | |_| |    / ____ \  | | | | |  __/ | (_| | | |    | (_) |
                      \__,_| |_| |___/ | .__/  |_|  \__,_|  \__, |   /_/    \_\ |_| |_|  \___|  \__, | |_|     \___/ 
                                       | |                   __/ |                               __/ |               
                                       |_|                  |___/                               |___/                                                       
 * 
 ***********************************************************************************************************************************************************
*   Este archivo contiene las funciones y threads encargadas de mostrar en pantalla el juego a traves de la libreria Allegro
*
 **********************************************************************************************************************************************************/

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h> //Manejo de ttfs
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h> 
#include <pthread.h>
#include <stdio.h>
#include "outputAllegro.h"
#include "allegro.h"
#include "../spaceLib/spaceLib.h"
#include <semaphore.h>

extern gameStatus_t GAME_STATUS;
extern sem_t SEM_GAME;
extern sem_t SEM_MENU;

static ALLEGRO_FONT * fuentes[FONTMAX] = {NULL};
/***********************************************************************************************************************************************************
 * 
 *                                                                      PROTOTIPOS DE FUNCIONES LOCALES
 * 
 * ********************************************************************************************************************************************************/


    /******************************************************************************
     *  SHOW ENTITY
     *      Esta funcion recibe como unico parametro un puntero a un object_t
     *      y se encarga de mostrarlo en pantalla
     * 
     * ***************************************************************************/

int showEntity(object_t * entity);

    /******************************************************************************
     *  SHOW LISTA
     *      Esta funcion recibe como unico parametro un puntero al primer elemento
     *      de una lista de objetos (object_t) y los imprime en pantalla 
     * 
     * ***************************************************************************/

int showObjects(object_t * inicial);
void showText(texto_t * data);
int showTexts(texto_t * inicial);
int showSprite(sprite_t * sprite);
int showSprites(sprite_t * inicial);


/***********************************************************************************************************************************************************
 * 
 *                                                                      THREAD PRINCIPAL
 * 
 * ********************************************************************************************************************************************************/

static int idQeue[20];

void * displayt (ALLEGRO_THREAD * thr, void * dataIn){

    output_data_t * data = (output_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    ALLEGRO_BITMAP * background = NULL;

    int bgpos = 0;
    int bgtimer = 0;
    int i = 0; //Contador

    //---------INICIALIZACION PARA EL USO DEL DISPLAY

    ALLEGRO_DISPLAY * display = NULL; 

    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    display = al_create_display(X_MAX,Y_MAX); //Se crea el display
    fuentes[smallF] = al_load_ttf_font("allegro/spaceInv.ttf", 20, 0); //fuente small

    fuentes[mediumF] = al_load_ttf_font("allegro/spaceInv.ttf", 30, 0); //fuente medium

    fuentes[largeF] = al_load_ttf_font("allegro/spaceInv.ttf", 50, 0); //fuente large

    fuentes[bigF] = al_load_ttf_font("allegro/spaceInv.ttf", 80, 0); //fuente big
    
    al_register_event_source(event_queue, al_get_display_event_source(display));

    //-------------------------------------------------
    //---------INICIALIZACION PARA EL USO DEL AUDIO

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(MUSICAMAX - 1);

    ALLEGRO_SAMPLE * audios[AUDIOMAX];
    ALLEGRO_SAMPLE * musica[MUSICAMAX - AUDIOMAX];
    ALLEGRO_SAMPLE_ID * musicaActual = NULL;

    //Inicializacion de musica
    musica[MUSICA_MENU - AUDIOMAX] = al_load_sample("game/audio/spaceinvadersMainTheme.wav");
    musica[MUSICA_JUEGO - AUDIOMAX] = al_load_sample("game/audio/spaceinvadersMainTheme.wav");

    //Inicializacion de Sonidos
    audios[COLISION_ALIEN_TOCADO] = al_load_sample("raspi/audiosRaspi/audioFilesRaspi/alien_tocado.wav");
    audios[COLISION_ALIEN_MUERTO] = al_load_sample("raspi/audiosRaspi/audioFilesRaspi/alien_muerto.wav");
    audios[COLISION_USER_TOCADO] = al_load_sample("raspi/audiosRaspi/audioFilesRaspi/user_tocado.wav");
    audios[COLISION_USER_MUERTO] = al_load_sample("raspi/audiosRaspi/audioFilesRaspi/user_muerto.wav");
    audios[BALA_USER] = al_load_sample("raspi/audiosRaspi/audioFilesRaspi/bala_user.wav");
    
    al_play_sample(musica[MUSICA_MENU - AUDIOMAX], 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, musicaActual);

    //-------------------------------------------------

    background = al_load_bitmap("game/spritesAllegro/fondo.png");
    if(!background){
         printf("fallo background\n");
    }

    while(!*data->close_display){

        usleep(10 * U_SEC2M_SEC);    

        bgtimer += 1;

        if(bgtimer % BGVEL == 0){

            bgpos += 1;
            bgtimer = 0;
        }
        if(bgpos == BGHEIGHT){
            bgpos = 0;
        }

        /**************************************************************
         * 
         *              DISPLAY
         * 
         * ***********************************************************/
        
        if(*data->displayFlag){
        
            //Se limpia la pantalla
            al_clear_to_color(al_map_rgb(BGCOLOR));

            al_draw_bitmap(background, 0, bgpos, 0);
            al_draw_bitmap(background, 0, bgpos - BGHEIGHT, 0);

            if(GAME_STATUS.inGame == 1 && GAME_STATUS.pantallaActual != MENU){
                //sem_wait(&SEM_GAME);
 
                //Se dibujan los elementos y textos en el buffer
                showObjects( *((*data).punteros.balasUsr) );
                showObjects( *((*data).punteros.balasAlien) );
                showObjects( *((*data).punteros.alienList) );
                showObjects( *((*data).punteros.UsrList) );
                showObjects( *((*data).punteros.barrerasList) );
                showObjects( *((*data).punteros.mothershipList) );

                showTexts(*data->text);
                //Objectos varios
                

                //sem_post(&SEM_GAME);

            }else if(GAME_STATUS.inGame == 0 || GAME_STATUS.pantallaActual == MENU){
                sem_wait(&SEM_MENU);

                //Se escriben los textos en el buffer
                showTexts(*data->text);
                showSprites( *((*data).punteros.screenObjects) );
                sem_post(&SEM_MENU);

            }

            //Se muestra en pantalla
            al_flip_display();

            *data->displayFlag= false;

        }

         /*************************************************************/
         /**************************************************************
         * 
         *              AUDIO
         * 
         * ***********************************************************/

        for(i=0; i<20; i++){
            if(idQeue[i] != 0){
                if(idQeue[i] < AUDIOMAX){
                    al_play_sample(audios[idQeue[i]], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                else if(idQeue[i] < MUSICAMAX){
                    al_stop_sample(musicaActual);
                    
                    al_play_sample(musica[idQeue[i] - AUDIOMAX], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, musicaActual);
                }
            }
            idQeue[i]=0;
        }
              

         /*************************************************************/
    }

    al_destroy_display(display);
    al_uninstall_audio();
    pthread_exit(0);
}

/**********************************************************************************************************************************************************/

/***********************************************************************************************************************************************************
 * 
 *                                                                      FUNCIONES SECUNDARIAS
 * 
 * ********************************************************************************************************************************************************/

        /**************************************************************
         * 
         *              DISPLAY
         * 
         * ***********************************************************/

/*****************ENTIDADES**********************/
int showEntity(object_t * entity){
  
    ALLEGRO_BITMAP * image = NULL;      //Se crea un bitmap donde guardar la imagen

    //Se busca la direccion del sprite
    objectType_t * asset = getObjType(entity->type);
    
    char * sprite = asset->sprite1;

    if (sprite == NULL){
        return -1;
    }
    else{
        image = al_load_bitmap(sprite);    //Se carga en el bitmap
    }
    

    //Si no se pudo cargar salta error
    if (!image) {
        return -1;
    }

    al_draw_bitmap(image, entity->pos.x, entity->pos.y, 0);     //Se dibuja en el display

    al_destroy_bitmap(image);       //Se eleimina la imagen

    return 0;
}

int showObjects(object_t * inicial){

    //punter al primer objeto de la lista
    object_t * puntero = inicial;

    //Si se paso un NULL salta error
    if(puntero == NULL){
        return -1;
    }

    else {
        //Se muestra el primer objeto
        showEntity(puntero);

        //Se muestran los siguientes
        while(puntero->next != NULL){
            puntero = puntero->next;
            showEntity(puntero);
        }
    }
    
    return 0;
}

/*******************TEXTOS***********************/
void showText(texto_t * data){

    //Comando para escribir un texto en el buffer

    if(data){
        al_draw_text(fuentes[data->fuente], al_map_rgb(255,255,255) , data->posx, data->posy, ALLEGRO_ALIGN_LEFT, data->texto);
    }
}

int showTexts(texto_t * inicial){

    //punter al primer objeto de la lista
    texto_t * puntero = inicial;

    //Si se paso un NULL salta error
    if(puntero == NULL){
        return -1;
    }
    
    else {
        //Se muestra el primer objeto
        showText(puntero);

        //Se muestran los siguientes
        while(puntero->next != NULL){
            puntero = puntero->next;
            showText(puntero);
        }
    }
    
    return 0;

}

/******************SPRITES***********************/
int showSprite(sprite_t * sprite){
  
    ALLEGRO_BITMAP * image = NULL;      //Se crea un bitmap donde guardar la imagen

    if (sprite == NULL){
        return -1;
    }

    char * dir = sprite->direccion;

    if (dir == NULL){
        return -1;
    }
    else{
        image = al_load_bitmap(dir);    //Se carga en el bitmap
    }

    //Si no se pudo cargar salta error
    if (!image) {
        return -1;
    }

    al_draw_bitmap(image, sprite->posx, sprite->posy, 0);     //Se dibuja en el display

    al_destroy_bitmap(image);       //Se eleimina la imagen

    return 0;
}

int showSprites(sprite_t * inicial){

    //punter al primer objeto de la lista
    sprite_t * puntero = inicial;

    //Si se paso un NULL salta error
    if(puntero == NULL){
        return -1;
    }

    else {
        //Se muestra el primer objeto
        showSprite(puntero);

        //Se muestran los siguientes
        while(puntero->next != NULL){
            puntero = puntero->next;
            showSprite(puntero);
        }
    }
    
    return 0;
}

        /**************************************************************
         * 
         *              AUDIO
         * 
         * ***********************************************************/

void addAudio(int id){
    
    int i = 0;

    while (idQeue[i] != 0)
    {
        i++;
    }

    idQeue[i] = id;

}

/**********************************************************************************************************************************************************/