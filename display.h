#ifndef DISPLAY_H
#define DISPLAY_H

#define X_MAX 640              //Ancho maximo de la pantalla. Vamos a tomar la primer posicion como 0.
#define Y_MAX 480              //Alto maximo de la pantalla

#define MARGEN_X 4          //Margen horizontal en el display
#define MARGEN_Y 1          //Margen vertical en el display

#define DESPLAZAMIENTO_X 1  //Indica cuanto de debe mover una nave en la coordenada x.
#define DESPLAZAMIENTO_Y 1  //Indica cuanto se debe mover una nave en la coordenada y.

#define BULLET_UP -1         //Indica cuanto se debe mover una bala aliada en Y
#define BULLET_DOWN 1      //Indica cuanto se debe mover una bala enemiga en Y

#define ESP_ALIENS_X 1      //Espacio vacio entre los aliens en la coordenada X.
#define TAM_ALIENS_X 3      //Tamano que ocupan los aliens en la coordenada X.
#define ESP_ALIENS_Y 1      //Espacio vacio entre los aliens en la coordenada Y.
#define TAM_ALIENS_Y 3      //Tamano que ocupan los aliens en la coordenada Y.

#define DIST_INICIAL_X 6    //Distancia de los aliens respecto del borde izquierdo de la pantalla al iniciar un nivel, sin offset por nave
#define DIST_INICIAL_Y 4    //Distancia de los aliens respecto del borde superior de la pantalla al iniciar un nivel, sin offset por nave

#define CANT_MAX_ALIENS 12  //Cantidad maxima de aliens, dependiendo del nivel se muestra distinta cantidad, con este tope.
#define CANT_MAX_FILAS 4    //CAntidad maxima de filas, dependiendo del nivel se muestra una cantidad distinta, con este tope.

#define BGCOLOR 0,0,0       //Color del background (negro)

typedef enum objectTypes {DANIEL, PABLO, NICOLAS, NAVE, BARRERA, BALA_DANIEL, BALA_PABLO, BALA_NICOLAS, BALA_USUARIO} types_t;

char * images[]= {"alien1.png", "alien2.png", "alien3.png", "nave.png", "escudo.png"};

int showEntity(object_t * entity);

#endif //display.h