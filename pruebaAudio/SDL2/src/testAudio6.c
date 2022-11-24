/*
    Test de la libreria para el manejo de SDL2. Este es un ejemplo de manejo con threads donde 
    cada thread llama a la misma funcion pero con distintos parametros. Esto puede servir por ejemplo 
    para threads que controlen distintos aspectos de un programa y al ocurrir cierto evento deben reproducir un
    sonido. Usa objetos de Audio creados por los threads, es un poco rebuscado pero es para dar un ejemplo de manejo distinto.

*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <SDL2/SDL.h>
#include "audio.h"

#define KENOBI "../sounds/obi-wan-hello-there.wav"
#define GRIEVOUS "../sounds/generalkenobi.wav"
#define THEME "../music/GTheme.wav"

void* Thread();
void* Thread2();
void* Thread3();

/**
 * @brief Inicializa el display. Debe llamarse UNA SOLA VEZ al empezar a usar el display. 
 * @param n numero del thread que llama a la funcion. Si es el 3 pongo musica y si es 1 o 2 pongo sonido
 * @param audio Puntero al objeto de audio que se quiere reproducir
*/
void playTrackFromThread(int n, Audio* audio);

pthread_mutex_t lock;

int main() {
    //inicializo el mutex lock
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    //inicializo el audio
     if ( initAudio() == NO_INIT)
    {
        fprintf(stderr, "Audio not initilized.\n");
	endAudio();
	return -1;
    }
    //creo los punteros a Audio. Van a ser inicializados por los distintos threads
    Audio*music=NULL; 
    Audio*sound=NULL;
    Audio*sound2=NULL;

    /*Creo los threads. Inicio al de la musica antes y espero para escucharla un poco. Luego inicio los otros.
    * Nada me asegura que se ejecute primero el audio de "Hello There". Delay para que la musica suene un poco.
    */
    pthread_t th1, th2, th3;
    pthread_create(&th3, NULL, Thread3, (void*)&music); 
    pthread_join(th3, NULL);
    sleep(5);
    pthread_create(&th1,NULL,Thread,(void*)&sound); 
    pthread_create(&th2, NULL, Thread2, (void*)&sound2); 
    
    pthread_join(th1,NULL);
	pthread_join(th2, NULL);
    
    
    sleep(5);   //delay para escuchar la musica
	endAudio(); 

    //Para ver si todo esta bien
    if(sound==NULL || music==NULL || sound2==NULL){
        printf("Null Audio* to free!\n");
    }
    //Destruyo los objetos de audio creados. Importante hacerlo despues de endAudio()
    freeAudio(sound);
    freeAudio(sound2);
    freeAudio(music);

    return 0;
}

void playTrackFromThread(int n, Audio * audio){
    //para el thread 3 reproduzco musica y para los otros un sonido
    static int i=1;
    printf("Print number %d entering from thread number %d\n", i++, n);
	if(n==3)
    {
        playMusicFromMemory(audio, SDL_MIX_MAXVOLUME);
    }
    else
    {
        pthread_mutex_lock(&lock);
        playSoundFromMemory(audio, SDL_MIX_MAXVOLUME);
        sleep(1);
        pthread_mutex_unlock(&lock);
    }
    
}

void* Thread(void* p){
    printf("Thread 1 executing\n");
    //creo un objeto de audio

    Audio * sound = createAudio(GRIEVOUS, 0, SDL_MIX_MAXVOLUME);

    /*Se lo paso al main para que luego de que se termine de usar se destruya
      Esto es importante porque los llamados a freeAudio() deben hacerse despues de endAudio 
      Hago lo mismo en los otros threads.
    */
    (*((Audio**)p))= sound;
    playTrackFromThread(1, sound);
	return NULL;
}

void* Thread2(void* p){
    printf("Thread 2 executing\n");
    Audio * sound2 = createAudio(KENOBI, 0, SDL_MIX_MAXVOLUME);
    (*((Audio**)p))= sound2;
    playTrackFromThread(2, sound2);
	return NULL;
}
void* Thread3(void* p){
    printf("Thread 3 executing\n");
    Audio * music = createAudio(THEME, 0, SDL_MIX_MAXVOLUME);
    (*((Audio**)p))= music;
    playTrackFromThread(3, music);
	return NULL;
}