#*******************************************************************************************************************************************
#													MAKEFILE RASPBERRY
#*******************************************************************************************************************************************
mainRaspi: spaceInvaders.o inputRaspi.o displayRaspi.o levelLoader.o spaceLib.o utilidades.h
	gcc joydrv.o disdrv.o displayRaspi.o inputRaspi.o levelLoader.o spaceLib.o spaceInvaders.o -pthread -Wall -o spaceInvaders


spaceInvaders.o: spaceInvaders.c utilidades.h
	gcc -c spaceInvaders.c -Wall

inputRaspi.o: inputRaspi.c inputRaspi.h
	gcc -c inputRaspi.c -Wall

displayRaspi.o: displayRaspi.c displayRaspi.h
	gcc -c displayRaspi.c -Wall

levelLoader.o: levelLoader.c spaceLib.o
	gcc -c levelLoader/levelLoader.c spaceLib/spaceLib.o -Wall -o levelLoader.o

spaceLib.o:	spaceLib/spaceLib.c spaceLib/spaceLib.h
	gcc -Wall -c spaceLib/spaceLib.c -o spaceLib/spaceLib.o

#*******************************************************************************************************************************************
#*******************************************************************************************************************************************

#*******************************************************************************************************************************************
#													MAKEFILE ALLEGRO
#*******************************************************************************************************************************************

COMPILACION_ALLEGRO = $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

mainAllegro: aliensYBalas.o testDisplayAllegro.o allegro.o displayAllegro.o inputAllegro.o
	gcc testDisplayAllegro.o aliensYBalas.o allegro.o displayAllegro.o inputAllegro.o -o testDisplayAllegro $(COMPILACION_ALLEGRO) -pthread






testDisplayAllegro.o: testDisplayAllegro.c utilidades.h
	gcc -c testDisplayAllegro.c -Wall

allegro.o: allegro.c allegro.h
	gcc -c allegro.c -Wall $(COMPILACION_ALLEGRO)

displayAllegro.o: displayAllegro.c displayAllegro.h utilidades.h
	gcc -c displayAllegro.c -Wall $(COMPILACION_ALLEGRO)

inputAllegro.o: inputAllegro.c inputAllegro.h utilidades.h aliensYBalas.h
	gcc -c inputAllegro.c -Wall $(COMPILACION_ALLEGRO)

