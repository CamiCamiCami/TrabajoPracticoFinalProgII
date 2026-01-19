#include "../Tablero.h"
#include <assert.h>
#include "../Constantes.h"

void testCrearTablero() {
    printf("testCrearTablero: ");
    Tablero tab = crearTablero();
    int pasaTest = 1;
    for (int fila = 0; fila < 8; fila++) {
        for (int col = 0; col < 8; col++) {
            if (fila < 4 || fila > 5 || col < 4 || col > 5) {
                assert(tab[col][fila] == CASILLA_VACIA);
            } else if((fila == 4 && col == 4) || (fila == 5 && col == 5)) {
                assert(tab[col][fila] == FICHA_BLANCA);
            } else {
                assert(tab[col][fila] == FICHA_NEGRA);
            }
        }
    }
}

void test



int main() {
    testCrearTablero();

}