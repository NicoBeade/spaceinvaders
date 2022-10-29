#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "libaudio.h"


char AudioFile[]="./Musica/LiveItUp.wav";
char Booo[]="./Musica/Booo.wav";

pthread_t soundT, soundT1;


void* soundThread(void audio){

    if(player_status()==READY){

        printf(GREEN_TEXT "Playing: %s \n",(char*)audio); 	printf(WHITE_TEXT);
	
		set_file_to_play((char*)audio);      // Load file 			
	
		play_sound(); 
    }

    while(1){};
}

int main(void){

    init_sound();

    pthread_create(&soundT, NULL, soundThread, AudioFile);

    pthread_create(&soundT1, NULL, soundThread, Booo);

    pthread_join(soundT1, NULL);

    return 0;
}