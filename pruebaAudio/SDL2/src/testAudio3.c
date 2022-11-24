#include <SDL2/SDL.h>
#include "audio.h"
#include <stdio.h>


#define KENOBI "../sounds/obi-wan-hello-there.wav"
#define GRIEVOUS "../sounds/generalkenobi.wav"
#define THEME "../music/GTheme.wav"
#define MAINTHEME "../music/MAINTHEME.wav"


int main(void)
{
    /* Init Simple-SDL2-Audio */
    if ( initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
	endAudio();
	return -1;
    }

    /* Play music  */
    playMusic(MAINTHEME, SDL_MIX_MAXVOLUME);    //temón

    SDL_Delay(10000);

    printf("Changing Music\n"); 

    //Si quiero cambiar abruptamente de musica una forma facil es "terminar" el audio e iniciarlo de nuevo
    endAudio();

    if ( initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
	endAudio();
	return -1;
    }

    //Reproduzco la nueva musica
    playMusic(THEME, SDL_MIX_MAXVOLUME);
    
    if ( playerStatus() == READY && musicStatus() == PLAYING)
    {
        printf("Music playing ok.\n");
    }
    //Delay para que se escuche la otra musica
    SDL_Delay(10000);
    
    /* End Simple-SDL2-Audio */
    endAudio();
    printf("Bye bye...\n");

    return 0;
}