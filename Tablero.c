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

char sumarFila(char fila, int mov) {
    return (char)(fila2int(fila) + mov);
}

char sumarColumna(char columna, int mov) {
    return (char)(columna2int(columna) + mov);
}

int dentroTablero(char fila, char columna) {
    return 0 <= columna2int(columna) && columna2int(columna) < 8 && 0 <= fila2int(fila) && fila2int(fila) < 8;
}

char verCasillaTablero(Tablero tablero, char fila, char columna) {
    assert(dentroTablero(fila, columna));
    return tablero[columna2int(columna)][fila2int(fila)];
}

void setCasillaTablero(Tablero tablero, char fila, char columna, char contenido) {
    assert(dentroTablero(fila, columna));
    assert(contenido == FICHA_BLANCA || contenido == FICHA_NEGRA || contenido == CASILLA_VACIA);
    tablero[columna2int(columna)][fila2int(fila)] = contenido;
}


// Toma: tablero, posicion, color
// 1) Si la posicion esta en el tablero y desocupada -> 2 sino -> 10
// 2) Chequear inicio direcciones
// 2.1) Si la primer casilla pertenece al tablero -> 2.2 sino -> 2.3
// 2.2) Si la primer ficha es del color opuesto -> 3 sino 2.3
// 2.3) Marcar direccion como invalida
// 3) Repetir 2 para todas las direcciones
// 4) Chequear fin direcciones no invalidas
// 4.1) Si la casilla pertenece al tablero -> 4.2 sino 4.6
// 4.2) Si la casilla no está vacia -> 4.3 sino 4.6
// 4.3) Si la casilla es del color opuesto -> 4.4 sino -> 4.5
// 4.4) Avanzar a la siguiente casilla en la direccion -> 4.1
// 4.5) Marcar la direccion como valida
// 4.6) Marcar la direccion como invalida
// 5) Repetir 4 para todas las direcciones no invalidas
// 6) Si hay al menos una direccion valida -> 7 sino -> 10
// 7) Marcar direcciones
// 7.1) Si la ficha es del color opuesto -> 7.2 sino 8
// 7.2) Cambiar el color de la ficha
// 7.3) Avanzar una posicion -> 7.1
// 8) Repetir 7 para todas las direcciones validas
// 9) Devolver 1 -> exit
// 10) Devolver 0 -> exit

char colorOpuesto(char color) {
    return color == FICHA_BLANCA ? FICHA_NEGRA : FICHA_BLANCA;
}

int chequeaPrimeraCasillaDireccion(Tablero tablero, char colorInsertado, char filaInsertado, char columnaInsertado, int mov_fila, int mov_columna) {
    char fila = sumarFila(filaInsertado, mov_fila);
    char columna = sumarColumna(columnaInsertado, mov_columna);
    return dentroTablero(fila, columna) && verCasillaTablero(tablero, fila, columna) == colorOpuesto(colorInsertado); // Funciona siempre por cortocircuito
}

int chequeaUltimaCasillaDireccion(Tablero tablero, char colorInsertado, char filaInsertado, char columnaInsertado, int mov_fila, int mov_columna) {
    // Avanza hasta la segunda casilla en la direccion, porque la primera ya fue chequeada.
    char fila = sumarFila(filaInsertado, 2*mov_fila);
    char columna = sumarColumna(columnaInsertado, 2*mov_columna);
    int direccionValida = 0; // si es valida = 1, si es invalida = -1, si no se sabe = 0
    while (!direccionValida) {
        if (dentroTablero(fila, columna) && verCasillaTablero(tablero, fila, columna) != CASILLA_VACIA) {
            if(verCasillaTablero(tablero, fila, columna) == colorInsertado) {
                direccionValida = 1;
            } else {
                // Avanza a la siguiente casilla
                fila = sumarFila(fila, mov_fila);
                columna = sumarColumna(columna, mov_columna);
            }
        } else {
            direccionValida = -1;
        }
    }
    return direccionValida == 1;
}

int colorValido(char color) {
    return color == FICHA_BLANCA || color == FICHA_NEGRA;
}

int direccionValida(Tablero tablero, char colorInsertado, char filaInsertado, char columnaInsertado, int mov_fila, int mov_columna) {
    return chequeaPrimeraCasillaDireccion(tablero, colorInsertado, filaInsertado, columnaInsertado, mov_fila, mov_columna) 
           && chequeaUltimaCasillaDireccion(tablero, colorInsertado, filaInsertado, columnaInsertado, mov_fila, mov_columna);
}


void marcarDireccion(Tablero tablero, char colorInsertado, char filaInsertado, char columnaInsertado, int mov_fila, int mov_columna) {
    char fila = sumarFila(filaInsertado, mov_fila);
    char columna = sumarColumna(columnaInsertado, mov_columna);
    while(verCasillaTablero(tablero, fila, columna) == colorOpuesto(colorInsertado)) {
        setCasillaTablero(tablero, fila, columna, colorInsertado);
        fila = sumarFila(fila, mov_fila);
        columna = sumarColumna(columna, mov_columna);
    }
}

int colocarFicha(Tablero tablero, char fila, char columna, char color) {
    if(!(dentroTablero(fila, columna) && verCasillaTablero(tablero, fila, columna) == CASILLA_VACIA && colorValido(color))) {
        return 0;
    }
    int encontroDireccionValida = 0;
    for (int mov_fila = -1; mov_fila <= 1; mov_fila++) {
        for (int mov_col = -1; mov_col <= 1; mov_col++) {
            if((mov_col != 0 || mov_fila != 0) && direccionValida(tablero, color, fila, columna, mov_fila, mov_col)) {
                encontroDireccionValida = 1;
                marcarDireccion(tablero, color, fila, columna, mov_fila, mov_col);
            }
        }
    }
    return encontroDireccionValida;
}


int posicionEsValida(Tablero tablero, char fila, char columna, char color) {
    if(!(dentroTablero(fila, columna) && verCasillaTablero(tablero, fila, columna) == CASILLA_VACIA && colorValido(color))) {
        return 0;
    }
    int encontroDireccionValida = 0;
    for (int mov_fila = -1; mov_fila <= 1 && !encontroDireccionValida; mov_fila++) {
        for (int mov_col = -1; mov_col <= 1 && !encontroDireccionValida; mov_col++) {
            if((mov_col != 0 || mov_fila != 0) && direccionValida(tablero, color, fila, columna, mov_fila, mov_col)) {
                encontroDireccionValida = 1;
            }
        }
    }
    return encontroDireccionValida;
}

        /* Decide si un jugador tiene jugada legal. Toma un tablero y un char representando el color de ficha.
    El argumento color debe ser FICHA_NEGRA o FICHA_BLANCA. Devolvera 1 si hay jugadas legales y 0 e caso contrario. */
int tieneJugada(Tablero tablero, char color) {
    int hayPosicionValida = 0;
    for (char fila = "1"; fila <= "8" && !hayPosicionValida; fila = sumarFila(fila, 1)) {
        for (char col = "A"; col <= "H" && !hayPosicionValida; col = sumarColumna(col, 1)){
            if(posicionEsValida(tablero, fila, col, color)) {
                hayPosicionValida = 1;
            }
        }
    }
    return hayPosicionValida;
}


        /* Libera la memoria asignada al Tablero*/
void liberarTablero(Tablero tablero) {
    for (int i = 0; i < 8; i++) {
        free(tablero[i]);
    }
    free(tablero);
}