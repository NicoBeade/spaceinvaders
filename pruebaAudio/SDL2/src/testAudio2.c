/*
    Test de la libreria para el manejo de SDL2. Este es un ejemplo de manejo basico con threads.
    Se reproducen el sonido y la musica en threads separados.
    Se usa una variable en el main para pausar o despausar el audio con 2 threads distintos con acceso a dicha variable
*/

#include <SDL2/SDL.h>
#include "audio.h"
#include <stdio.h>
#include <pthread.h>


#define KENOBI "../sounds/obi-wan-hello-there.wav"
#define GRIEVOUS "../sounds/generalkenobi.wav"
#define THEME "../music/GTheme.wav"


void* musicThread();
void* soundThread();
void* pauseThread(void* pausePtr);
void* unpauseThread(void* pausePtr);

int main(void)
{
    
    int pause=0;    //flag para la pausa o despausa.
    /* Init Simple-SDL2-Audio */
    if ( initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
	endAudio();
	return -1;
    }

	pthread_t musicTh,soundTh,pauseTh,unpauseTh;
    pthread_create(&musicTh,NULL,musicThread,NULL); 
    SDL_Delay(1000);

    pthread_create(&soundTh, NULL, soundThread, NULL); 
    pthread_create(&unpauseTh, NULL, unpauseThread, (void*)&pause);
    pthread_create(&pauseTh, NULL, pauseThread, (void*)&pause); 

    pthread_join(musicTh,NULL);
	pthread_join(soundTh, NULL);

    if ( playerStatus() == READY && musicStatus() == PLAYING)
    {
        printf("Music playing ok.\n");
    }
    SDL_Delay(5000);

    //Pauso con un thread
    pause = 1;
     
    
    
    SDL_Delay(2000);

    //Despauso con otro
    pause = 2;
    printf("Continuing...\n");
    
    
    SDL_Delay(10000);

    //Me uno a los threads de pausa y despausa al final. ¿Por que? De tarea averiguarlo :)
    pthread_join(pauseTh,NULL);
    pthread_join(unpauseTh,NULL);

    /* End Simple-SDL2-Audio */
    endAudio();
    printf("Bye bye...\n");

    return 0;
}

//thread para la musica
void* musicThread(){
    playMusic(THEME, SDL_MIX_MAXVOLUME);
    return NULL;
}

//thread para el sonido
void* soundThread(){
    playSound(KENOBI, SDL_MIX_MAXVOLUME);
    return NULL;
}

void* pauseThread(void* pausePtr){
    //Espero a que la variable de pausa tome el valor 1
    while(*((int *)pausePtr) != 1);
    //Luego pauso la musica
    pauseAudio();
    if ( musicStatus() == PAUSED )
    {
        printf("Music paused.\n");
    }
    return NULL;
}

void* unpauseThread(void* pausePtr){
    //espero a que la variable de pausa tome el valor 2
    while(*((int *)pausePtr) != 2);
    //Luego despauso la musica
    unpauseAudio();
    if ( musicStatus() == PLAYING )
    {
        printf("Music unpaused.\n");
    }
    return NULL;
}