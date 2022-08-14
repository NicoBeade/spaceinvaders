levelLoader: levelLoader.c spaceLib.o
	gcc -Wall -o levelLoader levelLoader.c ../spaceLib/spaceLib.o
spaceLib.o:	../spaceLib/spaceLib.c ../spaceLib/spaceLib.h
	gcc -Wall -c ../spaceLib/spaceLib.c -o ../spaceLib/spaceLib.o