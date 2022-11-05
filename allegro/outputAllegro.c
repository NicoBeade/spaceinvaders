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
void showText(texto_t * data, ALLEGRO_FONT * fuente);
int showTexts(texto_t * inicial, ALLEGRO_FONT * fuente);

audio_t * play(audio_t * firstAudio, ALLEGRO_SAMPLE * sample);

/***********************************************************************************************************************************************************
 * 
 *                                                                      THREAD PRINCIPAL
 * 
 * ********************************************************************************************************************************************************/

void * displayt (ALLEGRO_THREAD * thr, void * dataIn){

    output_data_t * data = (output_data_t *) dataIn;

    ALLEGRO_EVENT_QUEUE * event_queue = * data->event_queue;

    ALLEGRO_BITMAP * background = NULL;

    int bgpos = 0;
    int bgtimer = 0;

    //---------INICIALIZACION PARA EL USO DEL DISPLAY

    ALLEGRO_DISPLAY * display = NULL; 
    ALLEGRO_FONT * fuente = NULL;

    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    display = al_create_display(X_MAX,Y_MAX); //Se crea el display
    fuente = al_load_ttf_font("allegro/spaceInv.ttf", 36, 0);
    
    al_register_event_source(event_queue, al_get_display_event_source(display));

    //-------------------------------------------------
    //---------INICIALIZACION PARA EL USO DEL AUDIO

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(audioMax);

    ALLEGRO_SAMPLE * samples[audioMax];

    audio_t * audioActual = NULL;

    samples[aUsrDeath] = al_load_sample("../game/audio/explosion.wav");
    samples[aShoot] = al_load_sample("../game/audio/shoot.wav");
    

    //-------------------------------------------------

    background = al_load_bitmap("allegro/fondo2.png");
    if(!background){
         printf("fallo\n");
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
                sem_wait(&SEM_GAME);
 
                //Se dibujan los elementos y textos en el buffer
                showObjects( *((*data).punteros.balasUsr) );
                showObjects( *((*data).punteros.balasAlien) );
                showObjects( *((*data).punteros.alienList) );
                showObjects( *((*data).punteros.UsrList) );
                showObjects( *((*data).punteros.barrerasList) );
                showObjects( *((*data).punteros.mothershipList) );

                sem_post(&SEM_GAME);

            }else if(GAME_STATUS.inGame == 0 || GAME_STATUS.pantallaActual == MENU){
                sem_wait(&SEM_MENU);

                //Se escriben los textos en el buffer
                showTexts(*data->text, fuente);

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

        //Si hay audios para reproducir
        if(*data->audio != NULL){

            //apunto al primer audio
            audioActual = *data->audio;

            //si hay mas de uno
            while(audioActual != NULL){

                audioActual = play(audioActual, samples[audioActual->audioId]);
            }

            *data->audio = NULL;
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

/*********DISPLAY*************/

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
        //fprintf(stderr, "failed to load image ! %s\n", sprite);
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

void showText(texto_t * data, ALLEGRO_FONT * fuente){

    //Comando para escribir un texto en el buffer
    //int lenght = strlen(data->texto);

    if(data){
        /*al_draw_rectangle(data->posx - 10, data->posy - 5, 
        data->posx + lenght*TAMLETRAX +10 , data->posy + TAMLETRAY + 5, al_map_rgb(0,0,255), 3);

        al_draw_rectangle(data->posx - 5, data->posy -10, 
        data->posx + lenght*TAMLETRAX + 5, data->posy + TAMLETRAY +10, al_map_rgb(0,0,255), 3);

        al_draw_filled_rectangle(data->posx - 7, data->posy - 2, 
        data->posx + lenght*TAMLETRAX + 10, data->posy + TAMLETRAY +2, al_map_rgb(0,0,0));

        al_draw_filled_rectangle(data->posx - 2, data->posy -7, 
        data->posx + lenght*TAMLETRAX + 2, data->posy + TAMLETRAY + 7, al_map_rgb(0,0,0));*/

        al_draw_text(fuente, al_map_rgb(255,255,255) , data->posx, data->posy, ALLEGRO_ALIGN_LEFT, data->texto);
    }
}

int showTexts(texto_t * inicial, ALLEGRO_FONT * fuente){

    //punter al primer objeto de la lista
    texto_t * puntero = inicial;

    //Si se paso un NULL salta error
    if(puntero == NULL){
        return -1;
    }
    
    else {
        //Se muestra el primer objeto
        showText(puntero, fuente);

        //Se muestran los siguientes
        while(puntero->next != NULL){
            puntero = puntero->next;
            showText(puntero, fuente);
        }
    }
    
    return 0;

}

/*********AUDIO*************/

audio_t * play(audio_t * firstAudio, ALLEGRO_SAMPLE * sample){

    audio_t * sig = firstAudio->next;
    
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    
    free(firstAudio);
    
    return sig;
}
/**********************************************************************************************************************************************************/