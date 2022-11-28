
#include <stdio.h>
#include "audioHandlerRaspi.h"
#include "../../game/audioFiles/audioFiles.h"


#define VOLUME_JUMP 16
#define MAX_AUDIOS 25

//tipo de dato para la lookup table de los audios para reproducir desde un enum
typedef struct {
    int id;
    char* audio;
}audio_t;

//Structs de musicas
static const audio_t musicaMenu = {MUSICA_MENU,MUS_MENU};
static const audio_t musicaJuego = {MUSICA_JUEGO,MUS_JUEGO};

//Structs de sonidos
static const audio_t sonidoColisionAlienTocado = {COLISION_ALIEN_TOCADO,SON_COLISION_ALIEN_TOCADO};
static const audio_t sonidoColisionAlienMuerto = {COLISION_ALIEN_MUERTO,SON_COLISION_ALIEN_MUERTO};
static const audio_t sonidoColisionUserTocado = {COLISION_USER_TOCADO,SON_COLISION_USER_TOCADO};
static const audio_t sonidoColisionUserMuerto = {COLISION_USER_MUERTO,SON_COLISION_USER_MUERTO};
static const audio_t sonidoColisionMothershipMuerta = {COLISION_MOTHERSHIP_MUERTA,SON_COLISION_MOTHERSHIP_MUERTA};
static const audio_t sonidoColisionBarreraTocada = {COLISION_BARRERA_TOCADA,SON_COLISION_BARRERA_TOCADA};
static const audio_t sonidoColisionBarreraMuerta = {COLISION_BARRERA_MUERTA,SON_COLISION_BARRERA_MUERTA};
static const audio_t sonidoColisionChoqueBalas = {COLISION_CHOQUE_BALAS, SON_COLISION_CHOQUE_BALAS};
static const audio_t sonidoMothershipAparece = {MOTHERSHIP_APARECE, SON_MOTHERSHIP_APARECE};
static const audio_t sonidoMovimientoAliens = {MOVIMIENTO_ALIENS, SON_MOVIMIENTO_ALIENS};
static const audio_t sonidoBalaUser = {BALA_USER, SON_BALA_USER};
static const audio_t sonidoBalaAlien = {BALA_ALIEN, SON_BALA_ALIEN};
static const audio_t sonidoSelectMenu = {SELECT_MENU,SON_SELECT_MENU};
static const audio_t sonidoSwapMenu = {SWAP_MENU, SON_SWAP_MENU};
static const audio_t sonidoErrorMenu = {ERROR_MENU, SON_ERROR_MENU};
static const audio_t sonidoSweepLetra = {SWEEP_LETRA, SON_SWEEP_LETRA};
static const audio_t sonidoSavedScore = {SAVED_SCORE, SON_SAVED_SCORE};
static const audio_t sonidoPartidaGanada = {PARTIDA_GANADA,SON_PARTIDA_GANADA};
static const audio_t sonidoPartidaPerdida = {PARTIDA_PERDIDA,SON_PARTIDA_PERDIDA};


//Variable de volumen modificable con wrappper
static int volumenAudio=SDL_MIX_MAXVOLUME; //Arranca el volumen al mango

//array que contiene los structs de audios (id y path) para los sonidos y audios del juego 
static const audio_t* audioArray [MAX_AUDIOS] = {
    &musicaMenu, &musicaJuego,&sonidoColisionAlienTocado,&sonidoColisionAlienMuerto,&sonidoColisionUserTocado,&sonidoColisionUserMuerto,&sonidoColisionMothershipMuerta,&sonidoColisionBarreraTocada,&sonidoColisionBarreraMuerta,&sonidoColisionChoqueBalas,
    &sonidoMothershipAparece,
    &sonidoMovimientoAliens,
    &sonidoBalaUser,&sonidoBalaAlien,
    &sonidoSelectMenu,&sonidoSwapMenu,&sonidoErrorMenu,&sonidoSweepLetra,&sonidoSavedScore,
    &sonidoPartidaGanada,&sonidoPartidaPerdida
};

//handler del sonido a reproducirse
void audioHandlerRaspi(int audioId){
    printf("Reproduciendo audio con id %d \n",audioId);

    //Primero se ubica el audio en la look up table sabiendo el audioId
    audio_t* pointerAudio = NULL;
    int i;
    for (i=0; i<MAX_AUDIOS && pointerAudio == NULL; i++){
        if (audioArray[i]!=NULL && audioArray[i]->id == audioId){
            pointerAudio = (audio_t*) audioArray[i];
        }
    }
    printf("El valor del contador para el audio: %d\n",i);
    if (pointerAudio==NULL){
        fprintf(stderr,"Sound file not found, audioId received: %d \n",audioId);
        return;
    }

    //Ahora se reproduce el sonido o musica hallado
    printf("Reproduciendo audio con path %s \n", pointerAudio->audio);
    //Si es una musica
    if (audioId == MUSICA_MENU || audioId == MUSICA_JUEGO){
        endAudio(); //termina la musica anterior abruptamente
        if ( initAudio() == NO_INIT) { //Se inicia el sistema de audio nuevamente, con sus respectivas guardas
            fprintf(stderr, "Audio not initilized.\n");
	        endAudio();
            return;
            }
        playMusic(pointerAudio->audio,volumenAudio);
    }
    //Si es un sonido
    else{
        playSound(pointerAudio->audio,volumenAudio);
    }
    return;
}  

//regulador de volumen. Recibe el sentido en el que se modifica el volumen. 1 para subir, -1 para bajar
int regVolumeRaspi(int sentido){
    if (sentido==SUBIR_AUDIO && volumenAudio!=SDL_MIX_MAXVOLUME){
        volumenAudio+=VOLUME_JUMP;
        playSound(SON_SELECT_MENU, volumenAudio);
    }
    else if (sentido==BAJAR_AUDIO && volumenAudio!=0){
        volumenAudio-=VOLUME_JUMP;
        playSound(SON_SELECT_MENU, volumenAudio);
    }
    else if (sentido==CHECK_AUDIO){
    }
    else {
        playSound(SON_ERROR_MENU, SDL_MIX_MAXVOLUME);
    }
    printf("Volumen Audio quedo cargado en: %d\n",volumenAudio);
    return volumenAudio/VOLUME_JUMP;
}