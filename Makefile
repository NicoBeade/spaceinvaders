#*******************************************************************************************************************************************
#													MAKEFILE RASPBERRY
#*******************************************************************************************************************************************
mainRaspi: inputRaspi.o displayRaspi.o spaceLib.o levelLoader.o score.o menuUtilitiesRaspi.o sprites.o audioHandlerRaspi.o spaceInvaders.c
	gcc front/raspi/drivers/joydrv.o front/raspi/drivers/disdrv.o displayRaspi.o front/raspi/drivers/libAudioSDL2.o inputRaspi.o levelLoader.o spaceLib.o score.o menuUtilities.o sprites.o audioHandlerRaspi.o spaceInvaders.c -lpthread -I/usr/local/include -L/usr/local/lib -lSDL2 -Wall -o spaceInvaders -g -D RASPI

inputRaspi.o: front/raspi/inputRaspi.c front/raspi/inputRaspi.h back/spaceLib/spaceLib.h
	gcc -c -g front/raspi/inputRaspi.c -Wall -o inputRaspi.o 

displayRaspi.o: front/raspi/displayRaspi.c front/raspi/displayRaspi.h back/spaceLib/spaceLib.h back/spaceLib/score/score.h
	gcc -c -g front/raspi/displayRaspi.c -Wall -o displayRaspi.o 

sprites.o: front/raspi/sprites.h front/raspi/sprites.c
	gcc -c -g front/raspi/sprites.c -Wall -o sprites.o

menuUtilitiesRaspi.o: front/menuUtilities.h back/spaceLib/spaceLib.h back/levelLoader/levelLoader.h back/spaceLib/score/score.h front/raspi/displayRaspi.h front/raspi/sprites.h front/menuUtilities.c
	gcc -c -g front/menuUtilities.c -Wall -o menuUtilities.o -D RASPI

audioHandlerRaspi.o: front/raspi/audioHandlerRaspi.h front/raspi/audioHandlerRaspi.c front/raspi/audioFilesRaspi.h
	gcc -c -g front/raspi/audioHandlerRaspi.c -Wall -o audioHandlerRaspi.o

#*******************************************************************************************************************************************
#*******************************************************************************************************************************************

#*******************************************************************************************************************************************
#													MAKEFILE LEVELTESTER
#*******************************************************************************************************************************************

levelTester: back/levelLoader/levelLoader.c spaceLib.o
	gcc -Wall back/levelLoader/levelLoader.c spaceLib.o -D LEVELTESTER -o levelTester

#*******************************************************************************************************************************************
#													MAKEFILE ALLEGRO
#*******************************************************************************************************************************************

COMPILACION_ALLEGRO = $$(pkg-config allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)

mainAllegro: spaceLib.o levelLoader.o allegro.o outputAllegro.o inputAllegro.o score.o menuUtilitiesAllegro.o diseñoAllegro.o spaceInvaders.c
	gcc spaceInvaders.c spaceLib.o levelLoader.o allegro.o outputAllegro.o inputAllegro.o score.o diseñoAllegro.o menuUtilities.o -o spaceInvadersAllegro $(COMPILACION_ALLEGRO) -pthread -g -Wall -D ALLEGRO
	rm *.o

allegro.o: front/allegro/allegro.c front/allegro/allegro.h back/spaceLib/spaceLib.h front/allegro/allegroUtilities.h
	gcc -c -g front/allegro/allegro.c -Wall $(COMPILACION_ALLEGRO)

outputAllegro.o: front/allegro/outputAllegro.c front/allegro/outputAllegro.h front/allegro/audio.h front/allegro/allegroUtilities.h
	gcc -c -g front/allegro/outputAllegro.c -Wall $(COMPILACION_ALLEGRO)

inputAllegro.o: front/allegro/inputAllegro.c front/allegro/inputAllegro.h front/allegro/allegroUtilities.h
	gcc -c -g front/allegro/inputAllegro.c -Wall $(COMPILACION_ALLEGRO)

menuUtilitiesAllegro.o: front/menuUtilities.h front/menuUtilities.c back/spaceLib/spaceLib.h front/allegro/allegro.h back/levelLoader/levelLoader.h back/spaceLib/score/score.h
	gcc -c -g front/menuUtilities.c -Wall -o menuUtilities.o -D ALLEGRO

diseñoAllegro.o: front/allegro/diseñoAllegro.c front/allegro/diseñoAllegro.h front/allegro/allegro.h front/allegro/allegroUtilities.h
	gcc -c -g front/allegro/diseñoAllegro.c -Wall $(COMPILACION_ALLEGRO)

#*******************************************************************************************************************************************
#													TARGETS GENERALES DE BACK END
#*******************************************************************************************************************************************
spaceLib.o:	back/spaceLib/spaceLib.c back/spaceLib/spaceLib.h
	gcc -Wall -c -g back/spaceLib/spaceLib.c -o spaceLib.o

levelLoader.o: back/levelLoader/levelLoader.c spaceLib.o back/levelLoader/levelLoader.h
	gcc -c -g back/levelLoader/levelLoader.c -Wall -o levelLoader.o

score.o: back/spaceLib/score/score.c back/spaceLib/score/score.h
	gcc -c -g back/spaceLib/score/score.c -Wall -o score.o


clean:
	rm *.o