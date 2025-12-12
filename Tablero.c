#include "Tablero.h"


// AAAAAA malloc puede romperse
Tablero crearTablero() {
    Tablero tablero = malloc(sizeof(char*) * 8);
    assert(tablero);
    for (int i = 0; i < 8; i++) {
        tablero[i] = malloc(sizeof(char) * 8);
        assert(tablero[i]);
        for(int j = 0; j < 8; j++) {
            tablero[i][j] = CASILLA_VACIA;
        }
    }
    tablero[3][3] = tablero[4][4] = FICHA_BLANCA;
    tablero[3][4] = tablero[4][3] = FICHA_NEGRA;
    return tablero;
}

int fila2int(char fila) {
    return (int)fila - (int)'1';
}

int columna2int(char columna) {
    return (int)columna - (int)'A';
}

int dentroTablero(char fila, char columna) {
    return 0 <= columna2int(columna) && columna2int(columna) < 8 && 0 <= fila2int(fila) && fila2int(fila) < 8;
}

char verCasillaTablero(Tablero tablero, char fila, char columna) {
    if (dentroTablero(fila, columna)) {
        return tablero[columna2int(columna)][fila2int(fila)];
    } else {
        return CASILLA_VACIA;
    }
}


// Toma: tablero, posicion, color
// 1) Si la posicion esta en el tablero y desocupada -> 2 sino -> 8
// 2) Chequear direcciones
// 2.0) Si la primera ficha es del mismo color -> 2.6
// 2.1) Si la ficha pertenece al tablero -> 2.2 sino 2.6
// 2.2) Si la casilla no está vacia -> 2.3 sino 2.6
// 2.3) Si la casilla es del color opuesto -> 2.4 sino -> 2.5
// 2.4) Avanzar una ficha -> 2.1
// 2.5) Marcar la direccion como valida
// 2.6) Marcar la direccion como invalida
// 3) Repetir para todas las direcciones
// 4) Si hay al menos una direccion valida -> 5 sino -> 8
// 5) Marcar direcciones
// 5.1) Si la ficha es del color opuesto -> 5.2 sino 6
// 5.2) Cambiar el color de la ficha
// 5.3) Avanzar una posicion -> 5.1
// 6) Repetir para todas las direcciones validas
// 7) Devolver 1
// 8) Devolver 0


int chequeaLinea(Tablero tablero, char fila, char columna, char color, int mov_fila, int mov_columna) {
    if (!dentroTablero(fila, columna) || !(verCasillaTablero(tablero, fila, columna) != CASILLA_VACIA)) { // Funciona siempre por cortocircuito
        return 0;
    } 
}

        /* Coloca una ficha en el tablero. Toma un tablero, un char representando la fila y la columna y otro para el color de ficha. 
    El argumento color debe ser FICHA_NEGRA o FICHA_BLANCA. Devolvera 1 si puedo colocar la ficha y 0 en caso contrario. */
int colocarFicha(Tablero tablero, char fila, char columna, char color) {
    if(!dentroTablero(fila, columna) || verCasillaTablero(tablero, fila, columna) != CASILLA_VACIA) {
        return 0;
    }
    
}


        /* Decide si un jugador tiene jugada legal. Toma un tablero y un char representando el color de ficha.
    El argumento color debe ser FICHA_NEGRA o FICHA_BLANCA. Devolvera 1 si hay jugadas legales y 0 e caso contrario. */
int tieneJugada(Tablero tablero, char color) {

}


        /* Libera la memoria asignada al Tablero*/
void liberarTablero(Tablero tablero) {
    for (int i = 0; i < 8; i++) {
        free(tablero[i]);
    }
    free(tablero);
}