#ifndef CONSTANTES_OTHELLO
#define CONSTANTES_OTHELLO

/* DEGUG */

//#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(...) fprintf( stderr, __VA_ARGS__ )
#else
#define DEBUG_PRINT(...) do{ } while ( 0 )
#endif

/* Constantes */

#define CASILLA_VACIA 'X'
#define FICHA_NEGRA 'N'
#define FICHA_BLANCA 'B'
#define MAXLARGONOMBRE 50

/* Errores de Usuario */

#define MALUSO 1
#define NOPUDOABRIR 2

/* Errores de Formato */

#define FINPREMATURO 3
#define NOENCONTROINFO 4
#define NOMBRELARGO 5
#define COLORINVALIDO 6
#define MISMOCOLOR 7
#define MISMONOMBRE 8
#define MALFORMATOLINEA 9

/* Errores de Juego */

#define PASOILEGAL 10
#define FICHAILEGAL 11


#endif