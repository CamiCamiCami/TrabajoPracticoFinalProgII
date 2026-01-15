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

void chequearTestFormato(int resultado, int esperado, int numeroTest) {
    if (esperado == resultado) {
        printf("Test de Formato %i: Exitoso\n", numeroTest);
    } else {
        printf("Test de Formato %i: Fallado. Esperaba %i pero encontro %i\n", numeroTest, esperado, resultado);
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
    resultado = llamar("%s entradas/entrada10 > consolas/consola10", ejecutable); // Falta un argumento
    chequearTestUso(resultado, MALUSO, 1);

    resultado = llamar("%s entradas/entrada10 salidas/salida10 > consolas/consola11", ejecutable); // No existe el archivo entrada0
    chequearTestUso(resultado, NOPUDOABRIR, 2);


    /* Tests de Formato */
    resultado = llamar("%s entradas/entrada21 salidas/salida21 > consolas/consola21", ejecutable); // El archivo termina antes de terminar la cabecera
    chequearTestFormato(resultado, FINPREMATURO, 1);

    resultado = llamar("%s entradas/entrada22 salidas/salida22 > consolas/consola22", ejecutable); // Falta informacion en la cabecera
    chequearTestFormato(resultado, NOENCONTROINFO, 2);

    resultado = llamar("%s entradas/entrada23 salidas/salida23 > consolas/consola23", ejecutable); // Uno de los nombres dados excede el limite de 50 caracteres
    chequearTestFormato(resultado, NOMRELARGO, 3);

    resultado = llamar("%s entradas/entrada24 salidas/salida24 > consolas/consola24", ejecutable); // Encontro algo distinto de 'B' o 'N' cuando esperaba un color
    chequearTestFormato(resultado, COLORINVALIDO, 4);

    resultado = llamar("%s entradas/entrada25 salidas/salida25 > consolas/consola25", ejecutable); // Ambos jugadores fueron dados el mismo nombre
    chequearTestFormato(resultado, MISMOCOLOR, 5);

    resultado = llamar("%s entradas/entrada26 salidas/salida26 > consolas/consola26", ejecutable); // Ambos jugadores fueron dados el mismo color
    chequearTestFormato(resultado, MISMONOMBRE, 6);

    resultado = llamar("%s entradas/entrada27 salidas/salida27 > consolas/consola27", ejecutable); // Una de las lineas tienes un formato desconocido
    chequearTestFormato(resultado, MALFORMATOLINEA, 7);

    return 0;
}