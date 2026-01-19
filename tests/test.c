#include <stdlib.h>
#include <stdio.h>
#include "../Constantes.h"
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>


int esEspacio(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\0';
}

char siguienteNoEspacio(FILE* archivo) {
    char c = '\0';
    while (!feof(archivo) && esEspacio(c)) {
        fscanf(archivo, "%c", &c);
    }
    return c;
}

int archivosIguales(const char* direccion1, const char* direccion2) {
    FILE *archivo1 = fopen(direccion1, "r"), *archivo2 = fopen(direccion2, "r");
    if(archivo1 == NULL) {
        if (archivo2 != NULL) fclose(archivo2);
        printf("No se pudieron abrir los archivos a comparar\n");
        exit(1);
    } else if (archivo2 == NULL) {
        fclose(archivo1);
        printf("No se pudieron abrir los archivos a comparar\n");
        exit(1);
    }

    int iguales = 1;
    char c1 = 'a', c2 = 'a';
    while (!feof(archivo1) && !feof(archivo2) && iguales) {
        iguales = c1 == c2;
        c1 = siguienteNoEspacio(archivo1);
        c2 = siguienteNoEspacio(archivo2);
    }
    if (!(feof(archivo1) && feof(archivo2))) {
        // Uno termino antes que el otro
        iguales = 0;
    }
    fclose(archivo1); fclose(archivo2);
    return iguales;
}

void imprimirArchivo(char* direccion) {
    FILE* archivo = fopen(direccion, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo a imprimir\n");
        exit(1);
    }
    char c = '\0';
    while (!feof(archivo)) {
        printf("%c", c);
        fscanf(archivo, "%c", &c);
    }
    fclose(archivo);
}

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

void chequearTestJugadasInvalidas(int resultado, int numeroTest) {
    char direccionResultado[50], direccionEsperado[50];
    sprintf(direccionResultado, "consolas/consola3%i", numeroTest);
    sprintf(direccionEsperado, "consolas_esperadas/consola3%i", numeroTest);
    if (resultado != 0) {
        printf("Test de Jugada Invalida %i: Fallado. Esperaba salir con 0 pero encontro %i\n", numeroTest, resultado);
    } else if(!archivosIguales(direccionResultado, direccionEsperado)) {
        printf("Test de Jugada Invalida %i: Fallado\n", numeroTest);
        printf("\n        --- Esperaba ---        \n");
        imprimirArchivo(direccionEsperado);
        printf("\n        --- Encontro ---        \n");
        imprimirArchivo(direccionResultado);
    } else {
        printf("Test de Jugada Invalida %i: Exitoso\n", numeroTest);
    }
}

void chequearTestPartidaIncompleta(int resultado, int numeroTest) {
    char direccionResultado[50], direccionEsperado[50];
    sprintf(direccionResultado, "salidas/salida4%i", numeroTest);
    sprintf(direccionEsperado, "salidas_esperadas/salida4%i", numeroTest);
    if (resultado != 0) {
        printf("Test de Partida Incompleta %i: Fallado. Esperaba salir con 0 pero encontro %i\n", numeroTest, resultado);
    } else if(!archivosIguales(direccionResultado, direccionEsperado)) {
        printf("Test de Partida Incompleta %i: Fallado\n", numeroTest);
        printf("        --- Esperaba ---        \n");
        imprimirArchivo(direccionEsperado);
        printf("        --- Encontro ---        \n");
        imprimirArchivo(direccionResultado);
    } else {
        printf("Test de Partida Incompleta %i: Exitoso\n", numeroTest);
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

    resultado = llamar("%s entradas/entrada10 salidas/salida10 > consolas/consola11", ejecutable); // No existe el archivo entrada10
    chequearTestUso(resultado, NOPUDOABRIR, 2);


    /* Tests de Formato */
    resultado = llamar("%s entradas/entrada21 salidas/salida21 > consolas/consola21", ejecutable); // El archivo termina antes de terminar la cabecera
    chequearTestFormato(resultado, FINPREMATURO, 1);

    resultado = llamar("%s entradas/entrada22 salidas/salida22 > consolas/consola22", ejecutable); // Falta informacion en la cabecera
    chequearTestFormato(resultado, NOENCONTROINFO, 2);

    resultado = llamar("%s entradas/entrada23 salidas/salida23 > consolas/consola23", ejecutable); // Uno de los nombres dados excede el limite de 50 caracteres
    chequearTestFormato(resultado, NOMBRELARGO, 3);

    resultado = llamar("%s entradas/entrada24 salidas/salida24 > consolas/consola24", ejecutable); // Encontro algo distinto de 'B' o 'N' cuando esperaba un color
    chequearTestFormato(resultado, COLORINVALIDO, 4);

    resultado = llamar("%s entradas/entrada25 salidas/salida25 > consolas/consola25", ejecutable); // Ambos jugadores fueron dados el mismo nombre
    chequearTestFormato(resultado, MISMOCOLOR, 5);

    resultado = llamar("%s entradas/entrada26 salidas/salida26 > consolas/consola26", ejecutable); // Ambos jugadores fueron dados el mismo color
    chequearTestFormato(resultado, MISMONOMBRE, 6);

    resultado = llamar("%s entradas/entrada27 salidas/salida27 > consolas/consola27", ejecutable); // Una de las lineas tienes un formato desconocido
    chequearTestFormato(resultado, MALFORMATOLINEA, 7);


    /* Tests de jugadas invalidas */
    resultado = llamar("%s entradas/entrada31 salidas/salida31 > consolas/consola31", ejecutable); // La cuarta jugada es incorrecta.
    chequearTestJugadasInvalidas(resultado, 1);

    resultado = llamar("%s entradas/entrada32 salidas/salida32 > consolas/consola32", ejecutable); // Se pasa cuando todavia quedan jugadas posibles.
    chequearTestJugadasInvalidas(resultado, 2);


    /* Tests de partidas incompletas */
    resultado = llamar("%s entradas/entrada41 salidas/salida41 > consolas/consola41", ejecutable); // 
    chequearTestPartidaIncompleta(resultado, 1);

    resultado = llamar("%s entradas/entrada42 salidas/salida42 > consolas/consola42", ejecutable);
    chequearTestPartidaIncompleta(resultado, 2);
    return 0;
}