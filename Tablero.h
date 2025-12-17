#ifndef TABLERO_OTHELLO
#define TABLERO_OTHELLO


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Constantes.h"


typedef char** Tablero;


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


/* Devuelve si una posicion esta dentro del tablero. Toma un char representando la fila y otro la columna. 
 */
int dentroTablero(char fila, char columna);


/* Devuelve el contenido del tablero en una posicion. Toma un tablero, un char representando la fila y otro la columna. 
 *  Siempre devolvera CASILLA_VACIA, FICHA_NEGRA o FICHA_BLANCA. Si se ingresa una posicion fuera del tablero devolvera CASILLA_VACIA. 
 */
char verCasillaTablero(Tablero tablero, char fila, char columna);


/* Coloca una ficha en el tablero. Toma un tablero, un char representando la fila y la columna y otro para el color de ficha. 
 *  El argumento color debe ser FICHA_NEGRA o FICHA_BLANCA. Devolvera 0 si pudo colocar la ficha y FICHAILEGAL en caso contrario. 
 */
int colocarFicha(Tablero tablero, char fila, char columna, char color);


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


#endif