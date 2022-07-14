COMPILACION_ALLEGRO = $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

main: CopiaAliensYBalas.o displayRaspi.o testDisplayRaspi.o inputRaspi.o
	gcc joydrv.o disdrv.o CopiaAliensYBalas.o displayRaspi.o testDisplayRaspi.o inputRaspi.o -pthread -Wall -o prueba


mainAllegro: aliensYBalas.o testDisplayAllegro.o allegro.o displayAllegro.o inputAllegro.o
	gcc testDisplayAllegro.o aliensYBalas.o allegro.o displayAllegro.o inputAllegro.o -o testDisplayAllegro $(COMPILACION_ALLEGRO) -pthread




CopiaAliensYBalas.o: aliensYBalas.h CopiaAliensYBalas.c utilidades.h
	gcc -c CopiaAliensYBalas.c -Wall 

testDisplayRaspi.o: testDisplayRaspi.c displayRaspi.h aliensYBalas.h utilidades.h
	gcc -c testDisplayRaspi.c -Wall 

displayRaspi.o: displayRaspi.h displayRaspi.c utilidades.h 
	gcc -c displayRaspi.c -Wall 

inputRaspi.o: inputRaspi.c inputRaspi.h utilidades.h
	gcc -c inputRaspi.c -Wall


testAnim: testAnim.o
	gcc testAnim.o disdrv.o -o pruebaAnim -Wall

testAnim.o: testAnim.c sprites.h disdrv.h
	gcc -c testAnim.c -Wall



testDisplayAllegro.o: testDisplayAllegro.c utilidades.h
	gcc -c testDisplayAllegro.c -Wall

allegro.o: allegro.c allegro.h
	gcc -c allegro.c -Wall $(COMPILACION_ALLEGRO)

displayAllegro.o: displayAllegro.c displayAllegro.h utilidades.h
	gcc -c displayAllegro.c -Wall $(COMPILACION_ALLEGRO)

inputAllegro.o: inputAllegro.c inputAllegro.h utilidades.h aliensYBalas.h
	gcc -c inputAllegro.c -Wall $(COMPILACION_ALLEGRO)

