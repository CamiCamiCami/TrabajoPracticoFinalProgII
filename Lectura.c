#include "Lectura.h"



/* Funciones auxiliares */



/* Determina si un caracter es un espacio, una tabulacion o un retorno de carro y deberian, por lo tanto, ser ignorados.
 */
int esEspacio(char c) {
    return c == ' ' || c == '\t' || c == '\r';
}


/* Elimina los espacios al final de una cadena de caracteres. Toma la cadena y su largo.
 */
void eliminarEspaciosFinales(char nombre[MAXLARGONOMBRE + 1], int largo) {
    if(esEspacio(nombre[largo - 1])) {
        nombre[largo - 1] = '\0';
        eliminarEspaciosFinales(nombre, largo - 1);
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
 *  y un puntero a char donde guardar su color asociado. Devuelve 0 si leyo correctamente y un error de formato en caso contrario.
 */
int leerNombreConColor(FILE* archivo, char nombre[MAXLARGONOMBRE + 1], char* color) {
    int asignado = fscanf(archivo, " %[^,\n], %c \n", nombre, color);
    if(asignado == EOF) {
        // No pudo leer.
        return NOPUDOLEER;
    }
    if (asignado != 2) {
        // No pudo leer ambos campos.
        return MALFORMATOCABECERA;
    }
    if(!colorValido(*color)) {
        // El color asignado al jugador no es un color valido (blanco o negro).
        return COLORINVALIDO;
    }
    if (!chequearLargo(nombre)) {
        // El nombre es demasiado largo.
        return NOMRELARGO;
    }
    eliminarEspaciosFinales(nombre, strlen(nombre));
    return 0;
}


/* Lee las dos primeras lineas del archivo y asocia a ambos jugadores con sus respectivos colores. Toma el archivo y dos cadenas en la que guardar los nombre que lea.
 *  Devuelve 0 si leyo correctamente y un error de formato en caso contrario.
 */
int leerNombreJugadores(FILE* archivo, char nombreNegro[MAXLARGONOMBRE + 1], char nombreBlanco[MAXLARGONOMBRE + 1]) {
    int encontroError = 0;
    char jugador1[MAXLARGONOMBRE + 1], jugador2[MAXLARGONOMBRE + 1];
    char color1, color2;
    encontroError = leerNombreConColor(archivo, jugador1, &color1);
    if (encontroError) return encontroError;
    encontroError = leerNombreConColor(archivo, jugador2, &color2);
    if (encontroError) return encontroError;
    if(!strcmp(jugador1, jugador2)) {
        // Los nombres son iguales.
        return MISMONOMBRE;
    }
    if(color1 == FICHA_BLANCA && color2 == FICHA_NEGRA) {
        strcpy(nombreBlanco, jugador1);
        strcpy(nombreNegro, jugador2);
    } else if (color2 == FICHA_BLANCA && color1 == FICHA_NEGRA) {
        strcpy(nombreBlanco, jugador2);
        strcpy(nombreNegro, jugador1);
    } else {
        // Ambos jugadores tienen el mismo color.
        return MISMOCOLOR;
    }
}


/* Revisa si lo unico que quedan en la linea actual son espacios. Toma un descriptor de archivo y devuelve 1 si encontro el final de la linea y 0 en caso contrario.
 */
int esFinalDeLinea(FILE* archivo) {
    char c = ' ';
    while(esEspacio(c) && !feof(archivo)) {
        fscanf(archivo, "%c", &c); // cuidado cuando llega a EOF
    }
    if(c == '\n' || feof(archivo)) {
        return 1;
    } else {
        return 0;
    }
}



/* Funciones de libreria */



int leerCabecera(FILE* archivo, char nombreNegro[MAXLARGONOMBRE + 1], char nombreBlanco[MAXLARGONOMBRE + 1], char* colorInicial) {
    int encontroError = 0;
    encontroError = leerNombreJugadores(archivo, nombreNegro, nombreBlanco);
    if (encontroError) return encontroError;
    char color_inicio;
    int leidos = fscanf(archivo, " %c \n", &color_inicio);
    if(leidos == EOF) {
        // No pudo leer.
        return NOPUDOLEER;
    }
    if (leidos != 1) {
        // No pudo leer el campo.
        return MALFORMATOCABECERA;
    }
    if(!colorValido(color_inicio)) {
        // El color que arranca no es un color valido (blanco o negro).
        return COLORINVALIDO;
    }

    return color_inicio;
}


int lineaVacia(FILE* archivo) {
    long pos = ftell(archivo);
    if(esFinalDeLinea(archivo)) {
        return 1;
    } else {
        fseek(archivo, pos, SEEK_SET);  // buscarSaltoLinea avanzo el cursor del archivo.
        return 0;
    }
}


int leerLinea(FILE* archivo, char* fila, char* columna) {
    int escaneado = fscanf(archivo, " %c%c", columna, fila);
    if (escaneado == 2 && *fila != '\n' && *columna != '\n' && esFinalDeLinea(archivo)) {
        // No hubo problemas en la lectura.
        return 0;
    } else {
        // Todavia no deberia aparecer un salto de linea.
        return MALFORMATOLINEA;
    }
}