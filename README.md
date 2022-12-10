# Space Invaders
Este es el trabajo final de Programacion I. Consiste en un Space Invaders programado en C multiplataforma que puede ser corrido desde una RaspberryPi o bien desde un ordenador con sistema operativo Linux, utilizando la libreria allegro.

## Como instalar
Para jugar se debe clonar el repositorio y seguir los siguientes pasos:

### Linux:
Si se dese jugar en linux se debe instalar la librería Allegro 5: https://liballeg.org/download.html        
Luego se debe compilar utilizando los siguientes comandos:
```
make mainAllegro
```
Seguido de
```
./spaceInvadersAllegro
```

### Raspberry Pi:
Para jugar desde Raspberry se deben utilizar los siguientes comandos de terminal:
```
make mainRaspi
```
Luego
```
./spaceInvaders
```
