#*******************************************************************************************************************************************
#													MAKEFILE RASPBERRY
#*******************************************************************************************************************************************
mainRaspi: spaceInvaders.o inputRaspi.o displayRaspi.o spaceLib.o levelLoader.o utilidades.h
	gcc joydrv.o disdrv.o displayRaspi.o inputRaspi.o levelLoader.o spaceLib.o spaceInvaders.o -pthread -Wall -o spaceInvaders -g


inputRaspi.o: raspi/inputRaspi.c raspi/inputRaspi.h
	gcc -c -g raspi/inputRaspi.c -Wall -o inputRaspi.o 

displayRaspi.o: raspi/displayRaspi.c raspi/displayRaspi.h
	gcc -c -g raspi/displayRaspi.c -Wall -o displayRaspi.o 

cleanRaspi:
	rm spaceLib.o levelLoader.o displayRaspi.o inputRaspi.o spaceInvaders.o

#*******************************************************************************************************************************************
#*******************************************************************************************************************************************



#*******************************************************************************************************************************************
#													MAKEFILE ALLEGRO
#*******************************************************************************************************************************************

COMPILACION_ALLEGRO = $$(pkg-config allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 --libs --cflags)

mainAllegro: spaceLib.o levelLoader.o spaceInvaders.o allegro.o displayAllegro.o inputAllegro.o
	gcc spaceInvaders.o spaceLib.o levelLoader.o allegro.o displayAllegro.o inputAllegro.o -o spaceInvadersAllegro $(COMPILACION_ALLEGRO) -pthread -g -Wall

allegro.o: allegro/allegro.c allegro/allegro.h spaceLib/spaceLib.h
	gcc -c -g allegro/allegro.c -Wall $(COMPILACION_ALLEGRO)

displayAllegro.o: allegro/displayAllegro.c allegro/displayAllegro.h utilidades.h spaceLib/spaceLib.h
	gcc -c -g allegro/displayAllegro.c -Wall $(COMPILACION_ALLEGRO)

inputAllegro.o: allegro/inputAllegro.c allegro/inputAllegro.h utilidades.h
	gcc -c -g allegro/inputAllegro.c -Wall $(COMPILACION_ALLEGRO)








spaceInvaders.o: spaceInvaders.c utilidades.h
	gcc -c -g spaceInvaders.c -Wall 

spaceLib.o:	spaceLib/spaceLib.c spaceLib/spaceLib.h
	gcc -Wall -c -g spaceLib/spaceLib.c -o spaceLib.o

levelLoader.o: levelLoader/levelLoader.c spaceLib.o levelLoader/levelLoader.h
	gcc -c -g levelLoader/levelLoader.c -Wall -o levelLoader.o
