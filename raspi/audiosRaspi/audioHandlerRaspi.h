#ifndef AUDIOHANDLERRASPI
#define AUDIOHANDLERRASPI


#include "audio.h"
#include <SDL2/SDL.h>

//Enums de las posibles acciones para el regulador de audio
enum REG_AUDIO {SUBIR_AUDIO,BAJAR_AUDIO,CHECK_AUDIO};

//Enums de los sonidos y musicas del juego 
enum AUDIO {  MUSICA_MENU,MUSICA_JUEGO,
              COLISION_ALIEN_TOCADO,COLISION_ALIEN_MUERTO,COLISION_USER_TOCADO,COLISION_USER_MUERTO,COLISION_MOTHERSHIP_MUERTA,COLISION_BARRERA_TOCADA,COLISION_BARRERA_MUERTA,COLISION_CHOQUE_BALAS,
              MOTHERSHIP_APARECE,
              MOVIMIENTO_ALIENS,
              BALA_USER,BALA_ALIEN,
              SELECT_MENU,SWAP_MENU,ERROR_MENU,SWEEP_LETRA,
              SAVED_SCORE,
              PARTIDA_GANADA,PARTIDA_PERDIDA
              };

void audioHandlerRaspi(int audioId); //handler de musica y sonidos para raspi
int regVolumeRaspi(int sentido); //handler de volumen para raspi

#endif