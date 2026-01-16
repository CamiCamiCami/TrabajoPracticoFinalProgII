#include "Tablero.h"



/* Funciones de conveniencia */



int fila2int(char fila) {
    return (int)fila - (int)'1';
}


int columna2int(char columna) {
    return (int)columna - (int)'A';
}


char sumarFila(char fila, int mov) {
    return (char)((int)fila + mov);
}


char sumarColumna(char columna, int mov) {
    return (char)((int)columna + mov);
}



/* Funciones auxiliares */



/* Cambia un valor de una casilla del tablero. Toma el tablero, la posicion y el nuevo contenido de la casilla.
 * El nuevo contenido debera ser FICHA_BLANCA, FICHA_NEGRA o CASILLA_VACIA.
 */
void setCasillaTablero(Tablero tablero, char fila, char columna, char contenido) {
    assert(dentroTablero(fila, columna));
    assert(contenido == FICHA_BLANCA || contenido == FICHA_NEGRA || contenido == CASILLA_VACIA);
    tablero[columna2int(columna)][fila2int(fila)] = contenido;
}


/* Revisa que la casilla adyacente hacia un direccion sea del color opuesto. Toma el tablero, el color de la ficha a insertar, 
 *      la posicion de insercion y la direccion a chequear.
 */
int chequeaPrimeraCasillaDireccion(Tablero tablero, char colorInsertado, char filaInsertado, char columnaInsertado, int mov_fila, int mov_columna) {
    char fila = sumarFila(filaInsertado, mov_fila);
    char columna = sumarColumna(columnaInsertado, mov_columna);
    return dentroTablero(fila, columna) && verCasillaTablero(tablero, fila, columna) == colorOpuesto(colorInsertado); // Funciona siempre por cortocircuito
}


/* Revisa una direccion para ver si al otro lado de las fichas de color opuesto hay un ficha del mismo color. Toma el tablero,
 *      el color de la ficha a insertar, la posicion de insercion y la direccion a chequear.
 */
int chequeaUltimaCasillaDireccion(Tablero tablero, char colorInsertado, char filaInsertado, char columnaInsertado, int mov_fila, int mov_columna) {
    // Avanza hasta la segunda casilla en la direccion, porque la primera ya fue chequeada.
    char fila = sumarFila(filaInsertado, 2*mov_fila);
    char columna = sumarColumna(columnaInsertado, 2*mov_columna);
    int direccionValida = 0; // si es valida = 1, si es invalida = -1, si todavia no se sabe = 0
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


/* Determina si la ficha insertada puede capturar hacia una direccion. Toma el tablero, el color de la ficha a insertar,
 *      la posicion de insercion y la direccion a chequear.
 */
int direccionValida(Tablero tablero, char colorInsertado, char filaInsertado, char columnaInsertado, int mov_fila, int mov_columna) {
    return chequeaPrimeraCasillaDireccion(tablero, colorInsertado, filaInsertado, columnaInsertado, mov_fila, mov_columna)
        && chequeaUltimaCasillaDireccion(tablero, colorInsertado, filaInsertado, columnaInsertado, mov_fila, mov_columna);
}


/* Captura todas las fichas posibles hacia una direccion. Toma el tablero, el color de la ficha a insertar,
 *      la posicion de insercion y la direccion a capturar.
 */
void marcarDireccion(Tablero tablero, char colorInsertado, char filaInsertado, char columnaInsertado, int mov_fila, int mov_columna) {
    char fila = sumarFila(filaInsertado, mov_fila);
    char columna = sumarColumna(columnaInsertado, mov_columna);
    while(verCasillaTablero(tablero, fila, columna) == colorOpuesto(colorInsertado)) {
        setCasillaTablero(tablero, fila, columna, colorInsertado);
        fila = sumarFila(fila, mov_fila);
        columna = sumarColumna(columna, mov_columna);
    }
}


/* Revisa todas las direcciones en una posicion para determinar si es valido insertar una ficha alli.
 *      Toma el tablero, el color de la ficha a insertar y la posicion de insercion.
 */
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



/* Funciones de libreria */



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


int dentroTablero(char fila, char columna) {
    return 0 <= columna2int(columna) && columna2int(columna) < 8 && 0 <= fila2int(fila) && fila2int(fila) < 8;
}


char verCasillaTablero(Tablero tablero, char fila, char columna) {
    assert(dentroTablero(fila, columna));
    return tablero[columna2int(columna)][fila2int(fila)];
}


int colocarFicha(Tablero tablero, char fila, char columna, char color) {
    DEBUG_PRINT("Poniendo ficha %c en %c%c\n", color, columna, fila);
    if(!(dentroTablero(fila, columna) && verCasillaTablero(tablero, fila, columna) == CASILLA_VACIA && colorValido(color))) {
        DEBUG_PRINT("No la pudo poner: no cumplia prerequisitos\n");
        return FICHAILEGAL;
    }
    DEBUG_PRINT("Valida de base\n");
    int encontroDireccionValida = 0;
    for (int mov_fila = -1; mov_fila <= 1; mov_fila++) {
        for (int mov_col = -1; mov_col <= 1; mov_col++) {
            DEBUG_PRINT("analizando %i %i: ", mov_fila, mov_col);
            if((mov_col != 0 || mov_fila != 0) && direccionValida(tablero, color, fila, columna, mov_fila, mov_col)) {
                DEBUG_PRINT("VALIDO\n");
                encontroDireccionValida = 1;
                marcarDireccion(tablero, color, fila, columna, mov_fila, mov_col);
            } else {
                DEBUG_PRINT("INVALIDO\n");
            }
        }
    }
    if (encontroDireccionValida) {
        setCasillaTablero(tablero, fila, columna, color);
    }
    return encontroDireccionValida ? 0 : FICHAILEGAL;
}


int tieneJugada(Tablero tablero, char color) {
    int hayPosicionValida = 0;
    for (char fila = '1'; fila <= '8' && !hayPosicionValida; fila = sumarFila(fila, 1)) {
        for (char col = 'A'; col <= 'H' && !hayPosicionValida; col = sumarColumna(col, 1)){
            if(posicionEsValida(tablero, fila, col, color)) {
                hayPosicionValida = 1;
            }
        }
    }
    return hayPosicionValida;
}


void escribirTablero(FILE* archivo, Tablero tablero) {
    assert(archivo);
    for (char fila = '1'; fila <= '8'; fila = sumarFila(fila, 1)) {
        for (char col = 'A'; col <= 'H'; col = sumarColumna(col, 1)){
            fprintf(archivo, "%c", verCasillaTablero(tablero, fila, col));
        }
        fprintf(archivo, "\n");
    }
}


void imprimirTablero(Tablero tablero) {
    escribirTablero(stdout, tablero);
}


int colorValido(char color) {
    return color == FICHA_BLANCA || color == FICHA_NEGRA;
}


char colorOpuesto(char color) {
    return color == FICHA_BLANCA ? FICHA_NEGRA : FICHA_BLANCA;
}


char darGanador(Tablero tablero) {
    char casilla;
    int negras = 0, blancas = 0;
    for (char fila = '1'; fila <= '8'; fila = sumarFila(fila, 1)) {
        for (char col = 'A'; col <= 'H'; col = sumarColumna(col, 1)){
            casilla = verCasillaTablero(tablero, fila, col);
            if (casilla == FICHA_BLANCA) 
                blancas++;
            else if(casilla == FICHA_NEGRA)
                negras++;
        }
    }
    return negras > blancas ? FICHA_NEGRA : blancas > negras ? FICHA_BLANCA : CASILLA_VACIA;
}


void liberarTablero(Tablero tablero) {
    for (int i = 0; i < 8; i++) {
        free(tablero[i]);
    }
    free(tablero);
}

