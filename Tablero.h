#ifndef TABLERO_OTHELLO
#define TABLERO_OTHELLO


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Constantes.h"


/* Tablero */
/* Representa un tablero de Othello.
 * En todas las posicion del Tablero solo se pueden guardar los siguientes valores por diseño:
 *  FICHA_NEGRA FICHA_BLANCA CASILLA_VACIA (Ver Constantes.h)
 */
typedef char** Tablero;


/* Casilla */
/* Representa una posicion del tablero.
 * El campo fila y columna deben tener uno de los siguientes valores para que la casilla sea valida (esté dentro del tablero)
 *  Fila: 1 2 3 4 5 6 7 8
 *  Columna: A B C D E F G H
 */
typedef struct __casilla {
    char fila;
    char columna;
} Casilla;


/* Crea e iniciaiza un tablero de Othello 8x8. 
 * El tablero inicial que devuelve se ve siempre de la siguiente forma:
 *      XXXXXXXX
 *      XXXXXXXX
 *      XXXXXXXX
 *      XXXBNXXX
 *      XXXNBXXX
 *      XXXXXXXX
 *      XXXXXXXX
 *      XXXXXXXX
 */
Tablero crearTablero();


/* Devuelve si una casilla esta dentro del tablero. Toma una casilla. 
 */
int dentroTablero(Casilla casilla);


/* Devuelve el contenido del tablero en una posicion. Toma un tablero y una Casilla. 
 *  Siempre devolvera CASILLA_VACIA, FICHA_NEGRA o FICHA_BLANCA. La casilla debe estar dentro del tablero
 */
char verCasillaTablero(Tablero tablero, Casilla casilla);


/* Coloca una ficha en el tablero. Toma un tablero, una casilla y un char representando el color de la ficha. 
 *  El argumento color debe ser FICHA_NEGRA o FICHA_BLANCA. Devolvera 0 si pudo colocar la ficha y FICHAILEGAL en caso contrario. 
 */
int colocarFicha(Tablero tablero, Casilla casilla, char color);


/* Decide si un jugador tiene jugada legal. Toma un tablero y un char representando el color de ficha.
 *  El argumento color debe ser FICHA_NEGRA o FICHA_BLANCA. Devolvera 1 si hay jugadas legales y 0 e caso contrario. 
 */
int tieneJugada(Tablero tablero, char color);


/* Escribe los contenidos del tablero a un archivo. El contenido del archivo tendrá el formato:
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 * Donde C sera X (si la casilla estaba vacia), B (si habia una ficha blanca) o N (si habia una ficha negra) segun corresponda.
 */
void escribirTablero(FILE* archivo, Tablero tablero);


/* Imprime los contenidos del tablero. Tendrá el formato:
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 *      CCCCCCCC
 * Donde C sera X (si la casilla estaba vacia), B (si habia una ficha blanca) o N (si habia una ficha negra) segun corresponda.
 */
void imprimirTablero(Tablero tablero);


/* Toma el color de una ficha y determina si es valido.
 */
int colorValido(char color);


/* Toma el color de una ficha y devuelve su opuesto.
 */
char colorOpuesto(char color);


/* Toma un tablero y determina cual jugador tiene mas fichas. Devuelve siempre FICHA_NEGRA, FICHA_BLANCA o CASILLA_VACIA (en caso de empate).
 */
char darGanador(Tablero Tablero);


/* Libera la memoria asignada al Tablero
 */
void liberarTablero(Tablero tablero);


/* Crea una casilla. Toma la fila y la columna que la componen.
 */
Casilla crearCasilla(char fila, char columna);


/* Devuelve la casilla resultante de sumarles cantidades dadas a la columna y fila de una casilla base. Toma la casilla de referencia,
 *  la cantidad de filas y la cantidad de columnas que se debe mover (pueden tomar valores negativos).
 */
Casilla moverCasilla(Casilla base, int movFila, int movColumna);


/* Devuelve la casilla a la derecha de una casilla dada.
 */
Casilla siguienteColumna(Casilla base);


/* Devuelve la casilla de abajo a una casilla dada.
 */
Casilla siguieteFila(Casilla base);


#endif