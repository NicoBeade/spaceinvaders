main: aliensYBalas.o displayRaspi.o testDisplayRaspi.o
	gcc disdrv.o aliensYBalas.o displayRaspi.o testDisplayRaspi.o -pthread -Wall -o prueba

aliensYBalas.o: aliensYBalas.h aliensYBalas.c utilidades.h
	gcc -c aliensYBalas.c -Wall 

testDisplayRaspi.o: testDisplayRaspi.c displayRaspi.h aliensYBalas.h utilidades.h
	gcc -c testDisplayRaspi.c -Wall 

displayRaspi.o: displayRaspi.h displayRaspi.c utilidades.h 
	gcc -c displayRaspi.c -Wall 