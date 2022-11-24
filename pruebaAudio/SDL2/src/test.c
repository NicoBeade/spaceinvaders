#include <SDL2/SDL.h>
#include "audio.h"

#define S2WAIT 180

#define DOOR1 "../sounds/door1.wav"
#define DOOR2 "../sounds/door2.wav"
#define HIGHLANDS "../music/highlands.wav"
#define ROAD "../music/road.wav"

void dummyFun(void);
void shortDummyFun(void);

int main(void)
{
    /* Init Simple-SDL2-Audio */
    if ( initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
	endAudio();
	return -1;
    }

    /* Play music and a sound */
    playMusic(HIGHLANDS, SDL_MIX_MAXVOLUME);
    playSound(DOOR1, SDL_MIX_MAXVOLUME / 2);

    /* While using delay for showcase, don't actually do this in your project */
    printf("Delay showcase\n");
    if ( playerStatus() == READY && musicStatus() == PLAYING)
    {
        printf("Music playing ok.\n");
    }
    
    //SDL_Delay(5000);
    shortDummyFun();
    printf("End delay\n");

    /* Override music, play another sound */
    //playMusic(ROAD, SDL_MIX_MAXVOLUME);
    //SDL_Delay(1000);

    /* Pause audio test */
    printf("Pausing and waiting 1 second\n");
    pauseAudio();
    if ( musicStatus() == PAUSED )
    {
        printf("Music paused.\n");
    }

    SDL_Delay(1000);
    printf("Continuing...\n");
    unpauseAudio();
    if ( musicStatus() == PLAYING )
    {
        printf("Music unpaused.\n");
    }

    playSound(DOOR2, SDL_MIX_MAXVOLUME / 2);
    SDL_Delay(2000);

    /* Caching sound example, create, play from Memory, clear */

    Audio * sound = createAudio(DOOR1, 0, SDL_MIX_MAXVOLUME / 2);
    playSoundFromMemory(sound, SDL_MIX_MAXVOLUME);
    SDL_Delay(2000);

    Audio * music = createAudio(ROAD, 1, SDL_MIX_MAXVOLUME);
    playMusicFromMemory(music, SDL_MIX_MAXVOLUME);
    dummyFun();

    if ( musicStatus() == PLAYING )
    {
        printf("Music unpaused.\n");
    }

    /* End Simple-SDL2-Audio */
    endAudio();

    /* Important to free audio after ending Simple-SDL2-Audio because they might be referenced still */
    freeAudio(sound);
    freeAudio(music);

    return 0;
}

void dummyFun(void)
{
    int i = 1;

    for (i = 1; i < S2WAIT; i++)
    {
        printf("%d ",i);
        fflush(stdout);
        if (i % 5 == 0)
        {
            printf("Door\n");
            playSound(DOOR2, SDL_MIX_MAXVOLUME / 2);
        }

        SDL_Delay(1000);

    }
    putchar('\n');
}

void shortDummyFun(void)
{
    int i;
    for (i = 1; i < S2WAIT/12; i++)
    {
        printf("%d ",i);
        fflush(stdout);
        if (i % 5 == 0)
        {
            printf("Door\n");
            playSound(DOOR1, SDL_MIX_MAXVOLUME / 2);
        }

        SDL_Delay(1000);

    }
    putchar('\n');
}
