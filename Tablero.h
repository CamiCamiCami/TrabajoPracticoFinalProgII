#ifndef TABLERO_OTHELLO
#define TABLERO_OTHELLO

#define CASILLA_VACIA 'X'
#define FICHA_NEGRA 'N'
#define FICHA_BLANCA 'B'

typedef char** Tablero;


        /* Crea e iniciaiza un tablero de Othello 8x8. 
    El tablero inicial que devuelve se ve siempre de la siguiente forma:
    XXXXXXXX
    XXXXXXXX
    XXXXXXXX
    XXXBNXXX
    XXXNBXXX
    XXXXXXXX
    XXXXXXXX
    XXXXXXXX */
Tablero crearTablero();


        /* Devuelve si una posicion esta dentro del tablero. Toma un tablero, un char representando la fila y otro la columna. */
int dentroTablero(Tablero t, char fila, char columna);


        /* Devuelve el contenido del tablero en una posicion. Toma un tablero, un char representando la fila y otro la columna. 
    Siempre devolvera CASILLA_VACIA, FICHA_NEGRA o FICHA_BLANCA. */
char verCasillaTablero(Tablero t, char fila, char columna);


        /* Coloca una ficha en el tablero. Toma un tablero, un char representando la fila y la columna y otro para el color de ficha. 
    El argumento color debe ser FICHA_NEGRA o FICHA_BLANCA. Devolvera 1 si puedo colocar la ficha y 0 en caso contrario. */
int colocarFicha(Tablero t, char fila, char columna, char color)


        /* Decide si un jugador tiene jugada legal. Toma un tablero y un char representando el color de ficha.
    El argumento color debe ser FICHA_NEGRA o FICHA_BLANCA. Devolvera 1 si hay jugadas legales y 0 e caso contrario. */
int tieneJugada(Tabero t, char color);


        /* Libera la memoria asignada al Tablero*/
void liberarTablero(Tabero t);

#endif