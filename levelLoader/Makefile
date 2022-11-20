levelLoader: levelLoader.c spaceLib.o
	gcc -Wall -o levelLoader levelLoader.c ../spaceLib/spaceLib.o  -D LEVELTESTER
spaceLib.o:	../spaceLib/spaceLib.c ../spaceLib/spaceLib.h
	gcc -Wall -c ../spaceLib/spaceLib.c -o ../spaceLib/spaceLib.o -D FIX_UNDEF_EXTERNS
