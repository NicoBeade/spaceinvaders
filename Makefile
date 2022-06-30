main: aliensYBalas.o displayRaspi.o testDisplayRaspi.o inputRaspi.o
	gcc joydrv.o disdrv.o aliensYBalas.o displayRaspi.o testDisplayRaspi.o inputRaspi.o -pthread -Wall -o prueba

aliensYBalas.o: aliensYBalas.h aliensYBalas.c utilidades.h
	gcc -c aliensYBalas.c -Wall 

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