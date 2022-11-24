/*
    Test de la libreria para el manejo de SDL2. Este es un ejemplo de manejo con threads donde 
    cada thread llama a la misma funcion pero con distinto parametro. Esto puede servir por ejemplo 
    para threads que controlen distintos aspectos de un programa y al ocurrir cierto evento deben reproducir un
    sonido.

*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#include <SDL2/SDL.h>
#include "audio.h"


#define GRIEVOUS "../sounds/generalkenobi.wav"
#define THEME "../music/GTheme.wav"


/*Threads*/
void* Thread();
void* Thread2();
void* Thread3();

/**
 * @brief Inicializa el display. Debe llamarse UNA SOLA VEZ al empezar a usar el display. 
 * @param n numero del thread que llama a la funcion. Si es el 3 pongo musica y si es 1 o 2 pongo sonido
*/
void playTrackFromThread(int n);

pthread_mutex_t lock;

int main() {
    //inicio mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    //inicio audio
     if ( initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
	    endAudio();
	    return -1;
    }

    //creo los threads. Me uno primero al thread 3 para poner la musica antes
    pthread_t th1, th2, th3;
    pthread_create(&th3, NULL, Thread3, NULL); 
    pthread_join(th3, NULL);
    sleep(5);   //delay para escuchar la musica

    //ejecuto los demas threads
    pthread_create(&th1,NULL,Thread,NULL); 
    pthread_create(&th2, NULL, Thread2, NULL); 
    
    pthread_join(th1,NULL);
	pthread_join(th2, NULL);
    
    //delay para seguir escuchando la musica
    sleep(5);
    return 0;
}

void playTrackFromThread(int n){
    //print para ver como entran los threads y quien se llama primero
    static int i=1;
    printf("Print numero %d entrando desde el thread %d\n", i++, n);
	if(n==3)
    {
        playMusic(THEME, SDL_MIX_MAXVOLUME);
    }

    else
    {
    //bloqueado para que no se puedan meter los 2 threads de sonido a la vez
        pthread_mutex_lock(&lock);
        playSound(GRIEVOUS, SDL_MIX_MAXVOLUME );    //Uso solo un sonido para simplificar el codigo. 
        sleep(1);   //delay para escuchar bien el sonido
        pthread_mutex_unlock(&lock);
    }
    
}

//threads que llaman a la funcion de audio
void* Thread(){
    printf("Thread 1 executing\n");
    playTrackFromThread(1);
	return NULL;
}
void* Thread2(){
    printf("Thread 2 executing\n");
    playTrackFromThread(2);
	return NULL;
}
void* Thread3(){
    printf("Thread 3 executing\n");
    playTrackFromThread(3);
	return NULL;
}