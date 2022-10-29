

/* TEST OF Sound Library libaudio by dany*/
// gcc testaudio.c libaudio.c -I/usr/local/include -L/usr/local/lib -lSDL -lpthread -o testaudio
// Run with ./testaudio


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "../libaudio.h"

//char AudioFile[]="./Sound/red.wav";
char AudioFile[]="LiveItUp.wav";
void do_something(void);

int main(void) 
{
	
	printf(GREEN_TEXT "Audio player \n" WHITE_TEXT );
	
    init_sound();
 	
	if(player_status()==READY)
	{
			
		printf(GREEN_TEXT "Playing: %s \n",AudioFile); 	printf(WHITE_TEXT);
	
		set_file_to_play(AudioFile);      // Load file 			
	
		play_sound(); 					  // Play File (Non Blocking)
	
		//do_something();					  // Waste some time while playing

		//stop_sound();	   					  // Stop and close file

		//printf(GREEN_TEXT "Sound was stopped by App\n" WHITE_TEXT );

		//exit(0);
	}
	else
	{
			
		printf(RED_TEXT "System not ready did you run init_sound() ? \n" WHITE_TEXT );
				
		return (-1);		
	}

	while (player_status() == PLAYING) {};
	printf("Finished\n");
	return 0;
	
}

#define PLAY_TIME 5

void do_something(void)
{
	int i;
				
	for(i=0;i<PLAY_TIME;i++)  // Do someting else for a while (change i)
	{
		
		printf(YELLOW_TEXT "Playing ...\n" WHITE_TEXT ); // show this every 300 ms while playing the sound in background

		//SDL_Delay(300);
		sleep(5);
	}
	
	
}
