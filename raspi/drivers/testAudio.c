#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "libaudio.h"


char AudioFile[]="./Musica/LiveItUp.wav";
char Booo[]="./Musica/Booo.wav";

int main(void){

    init_sound();

    if(player_status()==READY){

        printf(GREEN_TEXT "Playing: %s \n",AudioFile); 	printf(WHITE_TEXT);
	
		set_file_to_play(AudioFile);      // Load file 			
	
		play_sound(); 

        set_file_to_play(Booo);

        play_sound();
    }

    while(1){};

    return 0;
}