//Paths a las musicas del juego
#define MUS_MENU
#define MUS_JUEGO

//Paths a los sonidos de las colisiones
#define SON_COLISION_ALIEN_TOCADO
#define SON_COLISION_ALIEN_MUERTO
#define SON_COLISION_USER_TOCADO
#define SON_COLISION_USER_MUERTO
#define SON_COLISION_MOTHERSHIP_MUERTA
#define SON_COLISION_BARRERA_TOCADA
#define SON_COLISION_BARRERA_MUERTA

//Path al sonido de movimiento de los aliens
#define SON_MOVIMIENTO_ALIENS

//Path al sonido de la aparicion de la mothership
#define SON_MOTHERSHIP_APARECE

//Paths a los sonidos de las balas disparadas
#define SON_BALA_ALIEN
#define SON_BALA_USER

//Paths a los sonidos de acciones dentro del menu
#define SON_SELECT_MENU
#define SON_SWAP_MENU
#define SON_ERROR_MENU
#define SON_SAVED_SCORE
#define SON_SWEEP_LETRA



//Enums de las musicas del juego
enum MUSICA {MUSICA_MENU,MUSICA_JUEGO};

//Enums de los sonidos del juego 
enum SONIDOS {COLISION_ALIEN_TOCADO,COLISION_ALIEN_MUERTO,COLISION_USER_TOCADO,COLISION_USER_MUERTO,COLISION_MOTHERSHIP_MUERTA,COLISION_BARRERA_TOCADA,COLISION_BARRERA_MUERTA,
              MOTHERSHIP_APARECE,
              MOVIMIENTO_ALIENS,
              BALA_USER,BALA_ALIEN,
              SELECT_MENU,SWAP_MENU,ERROR_MENU,SWEEP_LETRA,
              SAVED_SCORE,
              };

