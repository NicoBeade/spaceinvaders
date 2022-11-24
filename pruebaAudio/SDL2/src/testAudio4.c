/*
    Test de la libreria para el manejo de SDL2. Este es un ejemplo de manejo con objetos de tipo Audio.
    Se llaman seguidos los sonidos
*/
#include <SDL2/SDL.h>
#include "audio.h"
#include <stdio.h>
#include<unistd.h>

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

    Audio * music = createAudio(THEME, 0, SDL_MIX_MAXVOLUME);
    Audio * sound = createAudio(GRIEVOUS, 0, SDL_MIX_MAXVOLUME);
    Audio * sound2 = createAudio(KENOBI, 0, SDL_MIX_MAXVOLUME);
    //playMusicFromMemory(music, SDL_MIX_MAXVOLUME);
    
    //pongo la musica de fondo
    playSoundFromMemory(music, SDL_MIX_MAXVOLUME);
    SDL_Delay(3000);
    //varios sonidos a la vez con un poco de delay para distinguirlos. Se superponen pero se escuchan
    playSoundFromMemory(sound2, SDL_MIX_MAXVOLUME);
    SDL_Delay(300);
    playSoundFromMemory(sound2, SDL_MIX_MAXVOLUME);
    SDL_Delay(300);
    playSoundFromMemory(sound, SDL_MIX_MAXVOLUME);
    SDL_Delay(400);
    playSoundFromMemory(sound2, SDL_MIX_MAXVOLUME);
    SDL_Delay(4000);

    /* End Simple-SDL2-Audio */
    printf("Exiting...\n");
    endAudio();
    printf("Bye bye...\n");

    //Destroy Objects
    freeAudio(sound);
    freeAudio(sound2);
    freeAudio(music);
    return 0;
}