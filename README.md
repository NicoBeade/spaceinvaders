# Space Invaders
Este es el trabajo final de Programacion I. Consiste en un Space Invaders programado en C multiplataforma que puede ser corrido desde una RaspberryPi o bien desde un ordenador con sistema operativo Linux, utilizando la libreria allegro.

## Instalación
Para jugar se debe clonar el repositorio y seguir los siguientes pasos:

### Linux:
Si se dese jugar en linux se debe instalar la librería Allegro 5: https://liballeg.org/download.html        
Luego se debe compilar utilizando los siguientes comandos:
```
make mainAllegro
```
Seguido de:
```
./spaceInvadersAllegro
```

### Raspberry Pi:
Para jugar desde Raspberry se deben utilizar los siguientes comandos de terminal:
```
make mainRaspi
```
Luego:
```
./spaceInvaders
```
## Level Tester
El proyecto desarrollado permite que el usuario desarrolle sin necesidad de recompilar sus propios niveles, con los assets que desee. Tras crear un nivel o modificar un asset desde la carpeta game, se recomienda testear el nivel para verificar que se tenga toda la información necesaria para la correcta ejecución del juego.
Para utilizar el levelTester, primero compilarlo si es que no se tiene el ejecutable:
```
make levelTester
```
Y correrlo con el argumento de main "game":
```
./levelTester game
```


