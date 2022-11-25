#ifndef AUDIOSRASPI
#define AUDIOSRASPI

/*Como compilar:

gcc -Wall testAudio#.c ../libAudioSDL2.o -I/usr/local/include -L/usr/local/lib -lSDL2 -lpthread -o test#.out 

Donde deben reemplazar el “#” por el número de ejemplo de audio que estén compilando. 
Si su ejemplo no utiliza threads, no es necesario linkear (-lpthread).*/

#include "audio.h"
#include <SDL2/SDL.h>



#define PATH "raspi/audiosRaspi/audioFilesRaspi/"

//Paths a las musicas del juego
#define MUS_MENU (PATH "musica_menu.wav")
#define MUS_JUEGO (PATH "musica_juego.wav")

//Paths a los sonidos de las colisiones
#define SON_COLISION_ALIEN_TOCADO (PATH "alien_tocado.wav")
#define SON_COLISION_ALIEN_MUERTO (PATH "alien_muerto.wav")
#define SON_COLISION_USER_TOCADO (PATH "user_tocado.wav")
#define SON_COLISION_USER_MUERTO (PATH "user_muerto.wav")
#define SON_COLISION_MOTHERSHIP_MUERTA (PATH "mothership_muerta.wav")
#define SON_COLISION_BARRERA_TOCADA (PATH "barrera_tocada.wav")
#define SON_COLISION_BARRERA_MUERTA (PATH "barrera_muerta.wav")

//Path al sonido de movimiento de los aliens
#define SON_MOVIMIENTO_ALIENS (PATH "movimiento_aliens.wav")

//Path al sonido de la aparicion de la mothership
#define SON_MOTHERSHIP_APARECE (PATH "mothership_aparece.wav")

//Paths a los sonidos de las balas disparadas
#define SON_BALA_ALIEN (PATH "bala_alien.wav")
#define SON_BALA_USER (PATH "bala_user.wav")

//Paths a los sonidos de acciones dentro del menu
#define SON_SELECT_MENU (PATH "select_menu.wav")
#define SON_SWAP_MENU (PATH "user_tocado.wav")
#define SON_ERROR_MENU (PATH "error_menu.wav")
#define SON_SAVED_SCORE (PATH "saved_score.wav")
#define SON_SWEEP_LETRA (PATH "sweep_letra.wav")

//Enums de los sonidos y musicas del juego 
enum AUDIO {  MUSICA_MENU,MUSICA_JUEGO,
              COLISION_ALIEN_TOCADO,COLISION_ALIEN_MUERTO,COLISION_USER_TOCADO,COLISION_USER_MUERTO,COLISION_MOTHERSHIP_MUERTA,COLISION_BARRERA_TOCADA,COLISION_BARRERA_MUERTA,
              MOTHERSHIP_APARECE,
              MOVIMIENTO_ALIENS,
              BALA_USER,BALA_ALIEN,
              SELECT_MENU,SWAP_MENU,ERROR_MENU,SWEEP_LETRA,
              SAVED_SCORE,
              };

#endif