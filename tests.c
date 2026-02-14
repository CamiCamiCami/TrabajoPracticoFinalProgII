#include "./Tablero.c"
#include "./Lectura.c"
#include <assert.h>
#include "./Constantes.h"


/* Funciones Auxiliares */


int tablerosIguales(Tablero tab1, Tablero tab2) {
    int iguales = 1;
    for (int fila = 0; fila < 8 && iguales; fila++) {
        for (int col = 0; col < 8 && iguales; col++) {
            iguales = tab1[col][fila] == tab2[col][fila];
        }
    }
    return iguales;
}


Tablero leerTablero(char direccionEntrada[]) {
    FILE* archivoEntrada = fopen(direccionEntrada, "r");
    assert(archivoEntrada);
    Tablero tablero = crearTablero();
    char color;
    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            fscanf(archivoEntrada, "%c", &color);
            tablero[col][fila] = color;
        }
        assert(esFinalDeLinea(archivoEntrada)); // Salto de linea
    }
    fclose(archivoEntrada);
    return tablero;
}


int casillasIguales(Casilla c1, Casilla c2) {
    return c1.fila == c2.fila && c1.columna == c2.columna;
}


/* Funciones de Prueba */


void testChequeaPrimeraCasillaDireccion() {
    Tablero tablero = leerTablero("tests/TableroMedias.txt");
    assert(chequeaPrimeraCasillaDireccion(tablero, 'B', crearCasilla('3', 'H'), crearDireccion(-1, -1))); // Encuentra casilla negra
    assert(!chequeaPrimeraCasillaDireccion(tablero, 'B', crearCasilla('3', 'H'), crearDireccion(0, -1))); // Encuentra casilla blanca
    assert(!chequeaPrimeraCasillaDireccion(tablero, 'B', crearCasilla('3', 'H'), crearDireccion(0, 1)));  // Se sale del tablero
    liberarTablero(tablero);
}

void testChequeoUltimaCasillaDireccion() {
    Tablero tablero = leerTablero("tests/TableroMedias.txt");
    assert(chequeaUltimaCasillaDireccion(tablero, 'N', crearCasilla('5', 'B'), crearDireccion(0, 1))); // Encuentra casilla negra
    assert(!chequeaUltimaCasillaDireccion(tablero, 'N', crearCasilla('5', 'B'), crearDireccion(0, -1))); // Llega al borde del tablero
    liberarTablero(tablero);
}


void testCapturarDireccion() {
    Tablero tablero = leerTablero("tests/TableroCaptura.txt");
    Tablero esperado = leerTablero("tests/TableroEsperadoCapturarDireccion.txt");
    capturarDireccion(tablero, 'B', crearCasilla('4', 'E'), crearDireccion(1, 0)); // Captura hacia abajo
    assert(tablerosIguales(tablero, esperado));
    liberarTablero(tablero);
    liberarTablero(esperado);
}


void testColocarFichaInterno() {
    Tablero tab = leerTablero("tests/TableroCaptura.txt");
    Tablero inicial = leerTablero("tests/TableroCaptura.txt");
    /* Problemas de prerrequisitos */ 
    assert(colocarFichaInterno(tab, crearCasilla('9', 'D'), 'N', 0) == FICHA_ILEGAL); // Casilla fuera del tablero
    assert(colocarFichaInterno(tab, crearCasilla('3', 'D'), 'C', 0) == FICHA_ILEGAL); // Color inválido
    assert(colocarFichaInterno(tab, crearCasilla('4', 'B'), 'N', 0) == FICHA_ILEGAL); // Casilla ocupada

    /* No puede capturar */
    assert(colocarFichaInterno(tab, crearCasilla('8', 'H'), 'B', 0) == FICHA_ILEGAL); // No encuentra ningún vecino con su color opuesto
    assert(colocarFichaInterno(tab, crearCasilla('6', 'D'), 'B', 0) == FICHA_ILEGAL); // Adyacente de otro color pero no captura
    assert(tablerosIguales(tab, inicial)); // Como la jugada es invalida el tablero no se modifica

    /* Jugada válida */
    Tablero esperado = leerTablero("tests/TableroEsperadoColocarFicha.txt");
    assert(colocarFichaInterno(tab, crearCasilla('4', 'E'), 'B', 1) == 0); // Revisa que es válida la jugada
    assert(tablerosIguales(tab, inicial)); // El tablero no se modifica
    assert(colocarFichaInterno(tab, crearCasilla('4', 'E'), 'B', 0) == 0); // Hace la jugada
    assert(tablerosIguales(tab, esperado)); // El tablero se modifica

    liberarTablero(tab);
    liberarTablero(inicial);
    liberarTablero(esperado);
}

void testCrearTablero() {
    Tablero tablero = crearTablero();
    Tablero esperado = leerTablero("tests/TableroInicial.txt");
    assert(tablerosIguales(tablero, esperado)); // Los tableros siempre se inicializan al mismo estado
    liberarTablero(tablero);
    liberarTablero(esperado);
}


void testDentroTablero() {
    assert(!dentroTablero(crearCasilla('9', 'A'))); // Afuera
    assert(!dentroTablero(crearCasilla('A', '5'))); // Invertidos
    assert(dentroTablero(crearCasilla('1', 'A'))); // Dentro
}


void testVerCasillaTablero() {
    Tablero tablero = leerTablero("tests/TableroMedias.txt");
    assert(verCasillaTablero(tablero, crearCasilla('2', 'E')) == FICHA_NEGRA); 
    assert(verCasillaTablero(tablero, crearCasilla('3', 'C')) == FICHA_BLANCA); 
    assert(verCasillaTablero(tablero, crearCasilla('8', 'H')) == CASILLA_VACIA);
    liberarTablero(tablero);
}


void testTieneJugada() {
    Tablero tab1 = crearTablero();
    Tablero tab2 = leerTablero("tests/TableroCompleto.txt");
    Tablero tab3 = leerTablero("tests/TableroPaso.txt");
    /* Tienen jugada ambos jugadores */
    assert(tieneJugada(tab1, 'B'));
    assert(tieneJugada(tab1, 'N'));
    /* Nadie tiene jugada */
    assert(!tieneJugada(tab2, 'B')); 
    assert(!tieneJugada(tab2, 'N')); 
    /* Uno tiene y el otro no */
    assert(!tieneJugada(tab3, 'B'));
    assert(tieneJugada(tab3, 'N'));
    liberarTablero(tab1); 
    liberarTablero(tab2);
    liberarTablero(tab3);
}


void testDarGanador() {
    Tablero tab1 = leerTablero("tests/TableroCompleto.txt");
    Tablero tab2 = leerTablero("tests/TableroEmpate.txt");
    assert(darGanador(tab1) == 'B'); // Ganan las blancas
    char ganadorTab2 = darGanador(tab2);
    assert(ganadorTab2 != 'N' && ganadorTab2 != 'B'); // Empate, no gana nadie
    liberarTablero(tab1);
    liberarTablero(tab2);
}


void testSumarCasilla() {
    Casilla casilla = crearCasilla('5', 'D');
    assert(casillasIguales(moverCasilla(casilla, 2, 3), crearCasilla('7', 'G'))); // Suma
    assert(casillasIguales(moverCasilla(casilla, -2, -3), crearCasilla('3', 'A'))); // Resta
}


void testEliminarEspaciosFinales() {
    char nombreEspaciado[51] = "Jorge \t\r";
    char nombreSinEspaciar[51] = "Jorge";
    eliminarEspaciosFinales(nombreEspaciado, strlen(nombreEspaciado));
    assert(strcmp(nombreEspaciado, "Jorge") == 0); // Elimina los espacios (y otros caracteres molestos) del final
    eliminarEspaciosFinales(nombreSinEspaciar, strlen(nombreSinEspaciar));
    assert(strcmp(nombreSinEspaciar, "Jorge") == 0); // No hace nada porque no tenia espacios al final
}





int main() {
    testCrearTablero();
    printf("testCrearTablero: Exitoso\n");
    testDentroTablero();
    printf("testDentroTablero: Exitoso\n");
    testVerCasillaTablero();
    printf("testVerCasillaTablero: Exitoso\n");
    testColocarFichaInterno();
    printf("testColocarFichaInterno: Exitoso\n");
    testTieneJugada();
    printf("testTieneJugada: Exitoso\n");
    testDarGanador();
    printf("testDarGanador: Exitoso\n");
    testSumarCasilla();
    printf("testSumarCasilla: Exitoso\n");
    testChequeaPrimeraCasillaDireccion();
    printf("testChequeaPrimeraCasillaDireccion: Exitoso\n");
    testChequeoUltimaCasillaDireccion();
    printf("testChequeoUltimaCasillaDireccion: Exitoso\n");
    testCapturarDireccion();
    printf("testCapturarDireccion: Exitoso\n");
    testEliminarEspaciosFinales();
    printf("testEliminarEspaciosFinales: Exitoso\n");
    return EXIT_SUCCESS;
}