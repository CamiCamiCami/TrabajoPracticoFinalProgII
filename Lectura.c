#include "Lectura.h"



/* Funciones Auxiliares */



/* Elimina los espacios al final de una cadena de caracteres. Toma la cadena y su largo.
 */
void eliminarEspaciosFinales(char nombre[MAXLARGONOMBRE + 1], int largo) {
    if(nombre[largo-1] == ' ') {
        nombre[largo-1] = '\0';
        eliminarEspaciosFinales(nombre, largo-1);
    }
}


/* Revisa que una cadena de caracteres no exceda MAXLARGONOMBRE.
 */
int chequearLargo(char nombre[MAXLARGONOMBRE + 1]) {
    char ultimo_caracter_array = nombre[MAXLARGONOMBRE];
    nombre[MAXLARGONOMBRE] = '\0'; // Necesario para poder asegurar que strlen se comporta correctamente
    int largo = strlen(nombre);
    return largo < MAXLARGONOMBRE || ultimo_caracter_array == '\0';
}


/* Lee una linea del archivo y asocia un nombre de jugador con un color. Toma el archivo, una cadena en la que guardar el nombre que leera
 *  y un puntero a int donde expresar valores de error.
 * De encontrar un error de formato devolvera '\0' y llenara errorFormato con el valor correspondiente.
 */
char leerNombreConColor(FILE* archivo, char nombre[MAXLARGONOMBRE + 1], int* errorFormato) {
    char color;
    int asignado = fscanf(archivo, " %[^,\n], %c ", nombre, &color);
    if(asignado == EOF) {
        // No pudo leer.
        *errorFormato = NOPUDOLEER;
        return '\0'; // Return invalido porque la funcion finaliza con un error.
    }
    if (asignado != 2) {
        // No pudo leer ambos campos.
        *errorFormato = CAMPOSINSUFICIENTES;
        return '\0';
    }
    if(!colorValido(color)) {
        // El color asignado al jugador no es un color valido (blanco o negro).
        *errorFormato = COLORINVALIDO;
        return '\0';
    }
    if (!chequearLargo(nombre)) {
        // El nombre es demasiado largo.
        *errorFormato = NOMRELARGO;
        return '\0';
    }
    eliminarEspaciosFinales(nombre, strlen(nombre));
    return color;
}


/* Lee las dos primeras lineas del archivo y asocia a ambos jugadores con sus respectivos colores. Toma el archivo, dos cadenas en la que guardar los nombre que lea
 *  y un puntero a int donde expresar valores de error. De encontrar un error de formato llenara errorFormato con el valor correspondiente.
 */
void leerNombreJugadores(FILE* archivo, char nombreNegro[MAXLARGONOMBRE + 1], char nombreBlanco[MAXLARGONOMBRE + 1], int* errorFormato) {
    char jugador1[MAXLARGONOMBRE + 1], jugador2[MAXLARGONOMBRE + 1];
    char color1 = leerNombreConColor(archivo, jugador1, errorFormato);
    if (*errorFormato != 0) return;
    char color2 = leerNombreConColor(archivo, jugador2, errorFormato);
    if (*errorFormato != 0) return;
    if(!strcmp(jugador1, jugador2)) {
        // Los nombres son iguales.
        *errorFormato = MISMONOMBRE;
        return;
    }
    if(color1 == FICHA_BLANCA && color2 == FICHA_NEGRA) {
        strcpy(nombreBlanco, jugador1);
        strcpy(nombreNegro, jugador2);
    } else if (color2 == FICHA_BLANCA && color1 == FICHA_NEGRA) {
        strcpy(nombreBlanco, jugador2);
        strcpy(nombreNegro, jugador1);
    } else {
        // Ambos jugadores tienen el mismo color.
        *errorFormato = MISMOCOLOR;
    }
}



/* Funciones de Libreria */



char leerCabecera(FILE* archivo, char nombreNegro[MAXLARGONOMBRE + 1], char nombreBlanco[MAXLARGONOMBRE + 1], int* errorFormato) {
    leerNombreJugadores(archivo, nombreNegro, nombreBlanco, errorFormato);
    if (*errorFormato != 0) return '\0';
    char color_inicio;
    int leidos = fscanf(archivo, " %c ", &color_inicio);
    if(leidos == EOF) {
        // No pudo leer.
        *errorFormato = NOPUDOLEER;
        return '\0'; // Return invalido porque la funcion finaliza con un error.
    }
    if (leidos != 1) {
        // No pudo leer el campo.
        *errorFormato = CAMPOSINSUFICIENTES;
        return '\0';
    }
    if(!colorValido(color_inicio)) {
        // El color que arranca no es un color valido (blanco o negro).
        *errorFormato = COLORINVALIDO;
        return '\0';
    }

    return color_inicio;
}