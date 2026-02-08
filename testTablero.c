#include "../Tablero.h"
#include <assert.h>
#include "../Constantes.h"


int tablerosIguales(Tablero tab1, Tablero tab2) {
    int iguales = 1;
    for (int fila = 0; fila < 8 && iguales; fila++) {
        for (int col = 0; col < 8 && iguales; col++) {
            iguales = tab1[col][fila] == tab2[col][fila];
        }
    }
    return iguales;
}


Tablero generarTablero(char comando[], char colorEmpieza) {
    int i = 0;
    Tablero t = crearTablero();
    for(char columna = comando[0], fila = comando[1]; fila && columna; i++, columna = comando[2*i], fila = comando[2*i+1]) {
        DEBUG_PRINT("generarTablero: Jugando %c%c\n", columna, fila);
        colocarFicha(t, crearCasilla(fila, columna), i % 2 == 0 ? colorEmpieza : colorOpuesto(colorEmpieza));
    }
    return t;
}


void testCrearTablero() {
    Tablero tab = crearTablero();
    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            DEBUG_PRINT("testCrearTablero: probando %i|%i\n", fila, col);
            if (fila < 3 || fila > 4 || col < 3 || col > 4) {
                assert(tab[col][fila] == CASILLA_VACIA);
            } else if((fila == 3 && col == 3) || (fila == 4 && col == 4)) {
                assert(tab[col][fila] == FICHA_BLANCA);
            } else {
                assert(tab[col][fila] == FICHA_NEGRA);
            }
        }
    }
    liberarTablero(tab);
}


void testDentroTablero() {
    assert(!dentroTablero(crearCasilla('9', 'A'))); // Afuera en el borde
    assert(!dentroTablero(crearCasilla('9', 'I'))); // Afuera en la esquina
    assert(!dentroTablero(crearCasilla('A', '5'))); // Invertidos
    for (Casilla casilla = crearCasilla('1', 'A'); casilla.fila <= '8'; casilla = siguienteFila(casilla)) {
        for (casilla.columna = 'A'; casilla.columna <= 'H'; casilla = siguienteColumna(casilla)) {
            assert(dentroTablero(casilla)); // Todos los casos
        }
    }
}


void testColocarFicha() {
    Tablero tab = crearTablero(), referencia = crearTablero();
    assert(colocarFicha(tab, crearCasilla('8', 'H'), 'B') == FICHA_ILEGAL); // Ficha alejada
    assert(tablerosIguales(tab, referencia)); // El tablero no se modifico
    assert(colocarFicha(tab, crearCasilla('3', 'C'), 'N') == FICHA_ILEGAL); // Adyacente de otro color pero no captura
    assert(tablerosIguales(tab, referencia)); // El tablero no se modifico

    assert(colocarFicha(tab, crearCasilla('3', 'D'), 'N') == 0); // Jugada exitosa
    assert(verCasillaTablero(tab, crearCasilla('3', 'D')) == FICHA_NEGRA); // El tablero se modifico
    assert(verCasillaTablero(tab, crearCasilla('4', 'D')) == FICHA_NEGRA); // Y la captura se hizo
    liberarTablero(tab); liberarTablero(referencia);
}


void testVerCasillaTablero() {
    Tablero tab = crearTablero();
    assert(verCasillaTablero(tab, crearCasilla('4', 'D')) == FICHA_BLANCA); 
    assert(verCasillaTablero(tab, crearCasilla('5', 'D')) == FICHA_NEGRA); 
    colocarFicha(tab, crearCasilla('3', 'E'), 'B'); colocarFicha(tab, crearCasilla('3', 'F'), 'N'); colocarFicha(tab, crearCasilla('4', 'F'), 'B'); colocarFicha(tab, crearCasilla('5', 'F'), 'N'); colocarFicha(tab, crearCasilla('5', 'G'), 'B'); colocarFicha(tab, crearCasilla('3', 'D'), 'N'); colocarFicha(tab, crearCasilla('5', 'C'), 'B'); colocarFicha(tab, crearCasilla('6', 'F'), 'N'); colocarFicha(tab, crearCasilla('2', 'D'), 'B'); colocarFicha(tab, crearCasilla('5', 'H'), 'N');
    assert(verCasillaTablero(tab, crearCasilla('5', 'H')) == FICHA_NEGRA); // Casilla en el borde
    assert(verCasillaTablero(tab, crearCasilla('8', 'H')) == CASILLA_VACIA); // Casilla en la esquina
    liberarTablero(tab);
}


void testTieneJugada() {
    Tablero tab1 = crearTablero();
    Tablero tab2 = generarTablero("E3F3F4F5G5D3C5F6D2C4B4G3H3E2F7C1D1H2E6F8G6H6G4G7H4C3B3B2G8A3A1H8B1G2H5D6B5A5E7F1H7D7D8E8F2C8A6A7H1A2C7B6B7B8C6E1A4A8C2G1\0", 'B');
    assert(tieneJugada(tab1, 'B')); // Tienen jugada ambos jugadores
    assert(tieneJugada(tab1, 'N'));

    assert(!tieneJugada(tab2, 'B')); // Nadie tiene jugada
    assert(!tieneJugada(tab2, 'N'));
    liberarTablero(tab1); liberarTablero(tab2);
}


void testColorValido() {
    assert(colorValido('B'));
    assert(colorValido('N'));
    assert(!colorValido('C'));
    assert(!colorValido('\0'));
}


void testColorOpuesto() {
    assert(colorOpuesto('B') == 'N');
    assert(colorOpuesto('N') == 'B');
}


void testDarGanador() {
    Tablero tab1 = generarTablero("E3F3F4F5G5D3C5F6D2C4B4G3H3E2F7C1D1H2E6F8G6H6G4G7H4C3B3B2G8A3A1H8B1G2H5D6B5A5E7F1H7D7D8E8F2C8A6A7H1A2C7B6B7B8C6E1A4A8C2G1\0", 'B');
    // TODO: Empate
    assert(darGanador(tab1) == 'B');
}


int casillasIguales(Casilla c1, Casilla c2) {
    return c1.fila == c2.fila && c1.columna == c2.columna;
}


void testSumarCasilla() {
    Casilla casilla = crearCasilla('5', 'D');
    assert(casillasIguales(moverCasilla(casilla, 2, 3), crearCasilla('7', 'G'))); // Suma
    assert(casillasIguales(moverCasilla(casilla, -2, -3), crearCasilla('3', 'A'))); // Resta
}


int main() {
    testCrearTablero();
    printf("testCrearTablero: Exitoso\n");
    testDentroTablero();
    printf("testDentroTablero: Exitoso\n");
    testVerCasillaTablero();
    printf("testVerCasillaTablero: Exitoso\n");
    testColocarFicha();
    printf("testColocarFicha: Exitoso\n");
    testTieneJugada();
    printf("testTieneJugada: Exitoso\n");
    testColorValido();
    printf("testColorValido: Exitoso\n");
    testColorOpuesto();
    printf("testColorOpuesto: Exitoso\n");
    testDarGanador();
    printf("testDarGanador: Exitoso\n");
    testSumarCasilla();
    printf("testSumarCasilla: Exitoso\n");
}