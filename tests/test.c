#include <stdlib.h>
#include <stdio.h>
#include "../Constantes.h"
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

void chequearTestUso(int resultado, int esperado, int numeroTest) {
    if (esperado == resultado) {
        printf("Test de Uso %i: Exitoso\n", numeroTest);
    } else {
        printf("Test de Uso %i: Fallado. Esperaba %i pero encontro %i\n", numeroTest, esperado, resultado);
    }
}


int llamar(const char* llamado, char* ejecutable) {
    char comando[100];
    sprintf(comando, llamado, ejecutable);
    int status = system(comando);
    assert(WIFEXITED(status));
    return WEXITSTATUS(status);
}


int main(int args, char** argv) {
    if (args < 2) return 1;
    char* ejecutable = argv[1];
    int resultado;

    /* Tests de uso */
    resultado = llamar("%s entradas/entrada0", ejecutable); // Falta un argumento
    chequearTestUso(resultado, MALUSO, 1);

    resultado = llamar("%s entradas/entrada0 salidas/salida0", ejecutable); // No existe el archivo entrada0
    chequearTestUso(resultado, NOPUDOABRIR, 2);

    return 0;
}