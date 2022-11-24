
#include <stdio.h>
#include "audioHandlerRaspi.h"


#define VOLUME_JUMP 16



static int volumenAudio=80;

//handler del sonido a reproducirse
void audioHandlerRaspi(int audioId){
    switch(audioId){
        //musica del juego
        case MUSICA_MENU:
            endAudio(); //Se interrumpe el audio anterior abruptamente
            if ( initAudio() == NO_INIT) { //Se inicia el sistema de audio nuevamente, con sus respectivas guardas
                fprintf(stderr, "Audio not initilized.\n");
	            endAudio();
                return;
            }
            playMusic(MUS_MENU, volumenAudio);
            break;
        case MUSICA_JUEGO:
            endAudio(); //Se interrumpe el audio anterior abruptamente
            if ( initAudio() == NO_INIT) { //Se inicia el sistema de audio nuevamente, con sus respectivas guardas
                fprintf(stderr, "Audio not initilized.\n");
	            endAudio();
                return;
            }
            playMusic(MUS_JUEGO, volumenAudio);
            break;

        //sonidos de colisiones
        case COLISION_ALIEN_TOCADO:
            playSound(SON_COLISION_ALIEN_TOCADO, volumenAudio);
            break;
        case COLISION_ALIEN_MUERTO:
            playSound(SON_COLISION_ALIEN_MUERTO, volumenAudio);
            break;
        case COLISION_USER_TOCADO:
            playSound(SON_COLISION_USER_TOCADO, volumenAudio);
            break;
        case COLISION_USER_MUERTO:
            playSound(SON_COLISION_USER_MUERTO, volumenAudio);
            break;
        case COLISION_MOTHERSHIP_MUERTA:
            playSound(SON_COLISION_MOTHERSHIP_MUERTA, volumenAudio);
            break;
        case COLISION_BARRERA_TOCADA:
            playSound(SON_COLISION_BARRERA_TOCADA, volumenAudio);
            break;
        case COLISION_BARRERA_MUERTA:
            playSound(SON_COLISION_BARRERA_MUERTA, volumenAudio);
            break;

        //sonido de movimiento de aliens
        case MOVIMIENTO_ALIENS:
            playSound(SON_MOVIMIENTO_ALIENS, volumenAudio);
            break;

        //sonido de aparicion de mothership
        case MOTHERSHIP_APARECE:
            playSound(SON_MOTHERSHIP_APARECE, volumenAudio);
            break;

        //sonidos de balas
        case BALA_ALIEN:
            playSound(SON_BALA_ALIEN, volumenAudio);
            break;
        case BALA_USER:
            playSound(SON_BALA_USER, volumenAudio);
            break;

        //sonidos de menu
        case SELECT_MENU:
            playSound(SON_SELECT_MENU, volumenAudio);
            break;
        case SWAP_MENU:
            playSound(SON_SWAP_MENU, volumenAudio);
            break;
        case ERROR_MENU:
            playSound(SON_ERROR_MENU, volumenAudio);
            break;
        case SAVED_SCORE:
            playSound(SON_SAVED_SCORE, volumenAudio);
            break;
        case SWEEP_LETRA:
            playSound(SON_SWEEP_LETRA, volumenAudio);
            break;
    }

    return;
}


//regulador de volumen. Recibe el sentido en el que se modifica el volumen. 1 para subir, -1 para bajar
void regVolumeRaspi(int sentido){
    if (sentido==1 && volumenAudio!=SDL_MIX_MAXVOLUME){
        volumenAudio+=VOLUME_JUMP;
        playSound(SON_SELECT_MENU, volumenAudio);
    }
    else if (sentido==-1 && volumenAudio!=0){
        volumenAudio-=VOLUME_JUMP;
        playSound(SON_SELECT_MENU, volumenAudio);
    }
    else {
        playSound(SON_ERROR_MENU, SDL_MIX_MAXVOLUME/2);
    }
    return;
}