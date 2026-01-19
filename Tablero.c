#include "Tablero.h"



/* Funciones de conveniencia */


int fila2int(char fila) {
    return (int)fila - (int)'1';
}


int columna2int(char columna) {
    return (int)columna - (int)'A';
}


void llenarCasilla(Tablero tablero, Casilla casilla, char color) {
    assert(dentroTablero(casilla));
    tablero[columna2int(casilla.columna)][fila2int(casilla.fila)] = color;
}


/* Funciones auxiliares */


/* Revisa que la casilla adyacente hacia un direccion sea del color opuesto. Toma el tablero, el color de la ficha a insertar, 
 *      la posicion de insercion y la direccion a chequear.
 */
int chequeaPrimeraCasillaDireccion(Tablero tablero, char colorInsertado, Casilla casillaInsertado, int mov_fila, int mov_columna) {
    Casilla casilla = sumarCasilla(casillaInsertado, mov_fila, mov_columna);
    return dentroTablero(casilla) && verCasillaTablero(tablero, casilla) == colorOpuesto(colorInsertado); // Funciona siempre por cortocircuito
}


/* Revisa una direccion para ver si al otro lado de las fichas de color opuesto hay un ficha del mismo color. Toma el tablero,
 *      el color de la ficha a insertar, la posicion de insercion y la direccion a chequear.
 */
int chequeaUltimaCasillaDireccion(Tablero tablero, char colorInsertado, Casilla casillaInsertado, int mov_fila, int mov_columna) {
    // Avanza hasta la segunda casilla en la direccion, porque la primera ya fue chequeada.
    Casilla casilla = sumarCasilla(casillaInsertado, mov_fila*2, mov_columna*2);
    int direccionValida = 0; // si es valida = 1, si es invalida = -1, si todavia no se sabe = 0
    while (!direccionValida) {
        if (dentroTablero(casilla) && verCasillaTablero(tablero, casilla) != CASILLA_VACIA) {
            if(verCasillaTablero(tablero, casilla) == colorInsertado) {
                direccionValida = 1;
            } else {
                // Avanza a la siguiente casilla
                casilla = sumarCasilla(casilla, mov_fila, mov_columna);
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
int direccionValida(Tablero tablero, char colorInsertado, Casilla casillaInsertada, int mov_fila, int mov_columna) {
    return chequeaPrimeraCasillaDireccion(tablero, colorInsertado, casillaInsertada, mov_fila, mov_columna)
        && chequeaUltimaCasillaDireccion(tablero, colorInsertado, casillaInsertada, mov_fila, mov_columna);
}


/* Captura todas las fichas posibles hacia una direccion. Toma el tablero, el color de la ficha a insertar,
 *      la posicion de insercion y la direccion a capturar.
 */
void marcarDireccion(Tablero tablero, char colorInsertado, Casilla casillaInsertada, int mov_fila, int mov_columna) {
    DEBUG_PRINT("Marcando %i|%i\n", mov_fila, mov_columna);
    Casilla casillaMarcando = sumarCasilla(casillaInsertada, mov_fila, mov_columna);
    while(verCasillaTablero(tablero, casillaMarcando) == colorOpuesto(colorInsertado)) {
        llenarCasilla(tablero, casillaMarcando, colorInsertado);
        casillaMarcando = sumarCasilla(casillaMarcando, mov_fila, mov_columna);
    }
}


/* Coloca una ficha en el tablero. Toma un tablero, una casilla, un char representando el color de la ficha y un entero indicando si la funcion esta en modo chequear.
 *  El argumento color debe ser FICHA_NEGRA o FICHA_BLANCA. Devolvera 0 si pudo colocar la ficha y FICHAILEGAL en caso contrario. 
 *  Cuando la funcion esta en modo chequear no va a cambiar el tablero en lo absoluto, solo devuelve si es posible o no poner la ficha en esa posicion.
 */
int colocarFichaInterno(Tablero tablero, Casilla casilla, char color, int modoChequear) {
    DEBUG_PRINT("Poniendo ficha %c en %c%c\n", color, casilla.columna, casilla.fila);
    if(!(dentroTablero(casilla) && verCasillaTablero(tablero, casilla) == CASILLA_VACIA && colorValido(color))) {
        DEBUG_PRINT("No la pudo poner: no cumplia prerequisitos\n");
        return FICHAILEGAL;
    }
    DEBUG_PRINT("Valida de base\n");
    int encontroDireccionValida = 0;
    for (int mov_fila = -1; mov_fila <= 1 && (!modoChequear || !encontroDireccionValida); mov_fila++) { // Ver la NOTA abajo sobre la condición (!modoChequear || !encontroDireccionValida)
        for (int mov_col = -1; mov_col <= 1  && (!modoChequear || !encontroDireccionValida); mov_col++) {
            DEBUG_PRINT("analizando %i %i: ", mov_fila, mov_col);
            if((mov_col != 0 || mov_fila != 0) && direccionValida(tablero, color, casilla, mov_fila, mov_col)) {
                DEBUG_PRINT("VALIDO\n");
                encontroDireccionValida = 1;
                if(!modoChequear) marcarDireccion(tablero, color, casilla, mov_fila, mov_col);
            } else {
                DEBUG_PRINT("INVALIDO\n");
            }
        }
    }
    if (encontroDireccionValida && !modoChequear) {
        llenarCasilla(tablero, casilla, color);
    }
    return encontroDireccionValida ? 0 : FICHAILEGAL;
}
/* NOTA: Si está en modo chequear apenas encuentra una dirección válida puede terminar el bucle. Es decir, para que siga el bucle si está en modo chequear
 *  no debe haber encontrado una dirección válida. La condición queda modoChequear → ¬encontroDireccion, lo que es equivalente a ¬modoChequear ∨ ¬encontroDireccion.
 */


/* Revisa todas las direcciones en una posicion para determinar si es valido insertar una ficha alli.
 *      Toma el tablero, el color de la ficha a insertar y la posicion de insercion.
 */
int posicionEsValida(Tablero tablero, Casilla casilla, char color) {
    return colocarFichaInterno(tablero, casilla, color, 1) == 0;
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


int dentroTablero(Casilla casilla) {
    return 0 <= columna2int(casilla.columna) && columna2int(casilla.columna) < 8 && 0 <= fila2int(casilla.fila) && fila2int(casilla.fila) < 8;
}


char verCasillaTablero(Tablero tablero, Casilla casilla) {
    assert(dentroTablero(casilla));
    return tablero[columna2int(casilla.columna)][fila2int(casilla.fila)];
}


int colocarFicha(Tablero tablero, Casilla casilla, char color) {
    return colocarFichaInterno(tablero, casilla, color, 0);
}



int tieneJugada(Tablero tablero, char color) {
    int hayPosicionValida = 0;
    for (Casilla casilla = crearCasilla('1', 'A'); casilla.fila <= '8' && !hayPosicionValida; casilla = siguieteFila(casilla)) {
        for (casilla.columna = 'A'; casilla.columna <= 'H' && !hayPosicionValida; casilla = siguienteColumna(casilla)){
            if(posicionEsValida(tablero, casilla, color)) {
                hayPosicionValida = 1;
            }
        }
    }
    return hayPosicionValida;
}


void escribirTablero(FILE* archivo, Tablero tablero) {
    assert(archivo);
    for (Casilla casilla = crearCasilla('1', 'A'); casilla.fila <= '8'; casilla = siguieteFila(casilla)) {
        for (casilla.columna = 'A'; casilla.columna <= 'H'; casilla = siguienteColumna(casilla)) {
            fprintf(archivo, "%c", verCasillaTablero(tablero, casilla));
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
    char color;
    int negras = 0, blancas = 0;
    
    for (Casilla casilla = crearCasilla('1', 'A'); casilla.fila <= '8'; casilla = siguieteFila(casilla)) {
        for (casilla.columna = 'A'; casilla.columna <= 'H'; casilla = siguienteColumna(casilla)) {
            color = verCasillaTablero(tablero, casilla);
            if (color == FICHA_BLANCA) 
                blancas++;
            else if(color == FICHA_NEGRA)
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


Casilla crearCasilla(char fila, char columna) {
    Casilla nueva;
    nueva.fila = fila;
    nueva.columna = columna;
    return nueva;
}


Casilla sumarCasilla(Casilla base, int movFila, int movColumna) {
    Casilla nueva;
    nueva.columna = (char)((int)base.columna + movColumna);
    nueva.fila = (char)((int)base.fila + movFila);
    return nueva;
}


Casilla siguienteColumna(Casilla base) {
    return sumarCasilla(base, 0, 1);
}


Casilla siguieteFila(Casilla base) {
    return sumarCasilla(base, 1, 0);
}