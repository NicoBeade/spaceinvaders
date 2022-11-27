#*******************************************************************************************************************************************
#													MAKEFILE RASPBERRY
#*******************************************************************************************************************************************
mainRaspi: inputRaspi.o displayRaspi.o spaceLib.o levelLoader.o score.o menuUtilitiesRaspi.o sprites.o audioHandlerRaspi.o utilidades.h spaceInvaders.c
	gcc joydrv.o disdrv.o displayRaspi.o libAudioSDL2.o inputRaspi.o levelLoader.o spaceLib.o score.o menuUtilities.o sprites.o audioHandlerRaspi.o spaceInvaders.c -lpthread -I/usr/local/include -L/usr/local/lib -lSDL2 -Wall -o spaceInvaders -g -D RASPI


inputRaspi.o: raspi/inputRaspi.c raspi/inputRaspi.h spaceLib/spaceLib.h
	gcc -c -g raspi/inputRaspi.c -Wall -o inputRaspi.o 

displayRaspi.o: raspi/displayRaspi.c raspi/displayRaspi.h spaceLib/spaceLib.h spaceLib/score/score.h
	gcc -c -g raspi/displayRaspi.c -Wall -o displayRaspi.o 

sprites.o: raspi/sprites.h raspi/sprites.c
	gcc -c -g raspi/sprites.c -Wall -o sprites.o

menuUtilitiesRaspi.o: menuUtilities.h spaceLib/spaceLib.h spaceLib/score/score.h raspi/displayRaspi.h raspi/sprites.h menuUtilities.c
	gcc -c -g menuUtilities.c -Wall -o menuUtilities.o -D RASPI

audioHandlerRaspi.o: raspi/audiosRaspi/audioHandlerRaspi.h raspi/audiosRaspi/audioHandlerRaspi.c raspi/audiosRaspi/audiosRaspi.h
	gcc -c -g raspi/audiosRaspi/audioHandlerRaspi.c -Wall -o audioHandlerRaspi.o

cleanRaspi:
	rm spaceLib.o levelLoader.o displayRaspi.o inputRaspi.o spaceInvaders.o

#*******************************************************************************************************************************************
#*******************************************************************************************************************************************



#*******************************************************************************************************************************************
#													MAKEFILE ALLEGRO
#*******************************************************************************************************************************************

COMPILACION_ALLEGRO = $$(pkg-config allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)

mainAllegro: spaceLib.o levelLoader.o allegro.o outputAllegro.o inputAllegro.o score.o menuUtilitiesAllegro.o spaceInvaders.c
	gcc spaceInvaders.c spaceLib.o levelLoader.o allegro.o outputAllegro.o inputAllegro.o score.o menuUtilities.o -o spaceInvadersAllegro $(COMPILACION_ALLEGRO) -pthread -g -Wall -D ALLEGRO

allegro.o: allegro/allegro.c allegro/allegro.h spaceLib/spaceLib.h
	gcc -c -g allegro/allegro.c -Wall $(COMPILACION_ALLEGRO)

outputAllegro.o: allegro/outputAllegro.c allegro/outputAllegro.h spaceLib/spaceLib.h
	gcc -c -g allegro/outputAllegro.c -Wall $(COMPILACION_ALLEGRO)

inputAllegro.o: allegro/inputAllegro.c allegro/inputAllegro.h
	gcc -c -g allegro/inputAllegro.c -Wall $(COMPILACION_ALLEGRO)

menuUtilitiesAllegro.o: menuUtilities.h menuUtilities.c
	gcc -c -g menuUtilities.c -Wall -o menuUtilities.o -D ALLEGRO



spaceLib.o:	spaceLib/spaceLib.c spaceLib/spaceLib.h
	gcc -Wall -c -g spaceLib/spaceLib.c -o spaceLib.o

levelLoader.o: levelLoader/levelLoader.c spaceLib.o levelLoader/levelLoader.h
	gcc -c -g levelLoader/levelLoader.c -Wall -o levelLoader.o

score.o: spaceLib/score/score.c spaceLib/score/score.h
	gcc -c -g spaceLib/score/score.c -Wall -o score.o