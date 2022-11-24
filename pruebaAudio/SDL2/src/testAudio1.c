/*
    Test de la libreria para el manejo de SDL2. Este es un ejemplo de manejo basico.
    Se reproduce una musica de fondo y luego 2 sonidos separados por un delay para que se distingan bien.
*/

#include <SDL2/SDL.h>
#include "audio.h"
#include <stdio.h>


#define KENOBI "../sounds/obi-wan-hello-there.wav"
#define GRIEVOUS "../sounds/generalkenobi.wav"
#define THEME "../music/GTheme.wav"



int main(void)
{
    /* Init Simple-SDL2-Audio */
    if ( initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
	endAudio();
	return -1;
    }

    /* Reproduzco musica y dos sonidos */
    playMusic(THEME, SDL_MIX_MAXVOLUME);
    SDL_Delay(8000);

    playSound(KENOBI, SDL_MIX_MAXVOLUME );
    SDL_Delay(2100);

    playSound(GRIEVOUS, SDL_MIX_MAXVOLUME );
    SDL_Delay(5000);

    /* End Simple-SDL2-Audio */
    endAudio();
    printf("Bye bye...\n");

    return 0;
}