#include "Lectura.h"



/* Funciones auxiliares */



/* Determina si un carácter es un espacio, una tabularon o un retorno de carro y deberían, por lo tanto, ser ignorados.
 */
int esEspacio(char c) {
    return c == ' ' || c == '\t' || c == '\r';
}


/* Elimina los espacios al final de una cadena de caracteres. Toma la cadena y su largo.
 */
void eliminarEspaciosFinales(char nombre[MAX_LARGO_NOMBRE + 1], int largo) {
    if(esEspacio(nombre[largo - 1])) {
        nombre[largo - 1] = '\0';
        eliminarEspaciosFinales(nombre, largo - 1);
    }
}


/* Revisa que una cadena de caracteres no exceda MAX_LARGO_NOMBRE.
 */
int chequearLargo(char nombre[MAX_LARGO_NOMBRE + 1]) {
    char ultimo_caracter_array = nombre[MAX_LARGO_NOMBRE];
    nombre[MAX_LARGO_NOMBRE] = '\0'; // Necesario para poder asegurar que strlen se comporta correctamente
    int largo = strlen(nombre);
    return largo < MAX_LARGO_NOMBRE || ultimo_caracter_array == '\0';
}


/* Lee una linea del archivo y asocia un nombre de jugador con un color. Toma el archivo, una cadena en la que guardar el nombre que leerá
 *  y un puntero a char donde guardar su color asociado. Devuelve 0 si leyó correctamente y un error de formato en caso contrario.
 */
int leerNombreConColor(FILE* archivo, char nombre[MAX_LARGO_NOMBRE + 1], char* color) {
    int asignado = fscanf(archivo, " %[^,\n], %c \n", nombre, color);
    if(asignado == EOF) {
        // No pudo leer.
        return FIN_PREMATURO;
    }
    if (asignado != 2) {
        // No pudo leer ambos campos.
        return NO_ENCONTRO_INFO;
    }
    if(!colorValido(*color)) {
        // El color asignado al jugador no es un color valido (blanco o negro).
        return COLOR_INVALIDO;
    }
    if (!chequearLargo(nombre)) {
        // El nombre es demasiado largo.
        return NOMBRE_LARGO;
    }
    eliminarEspaciosFinales(nombre, strlen(nombre));
    return 0;
}


/* Lee las dos primeras lineas del archivo y asocia a ambos jugadores con sus respectivos colores. Toma el archivo y dos cadenas en la que guardar los nombre que lea.
 *  Devuelve 0 si leyó correctamente y un error de formato en caso contrario.
 */
int leerNombreJugadores(FILE* archivo, char nombreNegro[MAX_LARGO_NOMBRE + 1], char nombreBlanco[MAX_LARGO_NOMBRE + 1]) {
    int encontroError = 0;
    char jugador1[MAX_LARGO_NOMBRE + 1], jugador2[MAX_LARGO_NOMBRE + 1];
    char color1, color2;
    encontroError = leerNombreConColor(archivo, jugador1, &color1);
    if (encontroError) return encontroError;
    encontroError = leerNombreConColor(archivo, jugador2, &color2);
    if (encontroError) return encontroError;
    if(!strcmp(jugador1, jugador2)) {
        // Los nombres son iguales.
        return MISMO_NOMBRE;
    }
    if(color1 == FICHA_BLANCA && color2 == FICHA_NEGRA) {
        strcpy(nombreBlanco, jugador1);
        strcpy(nombreNegro, jugador2);
    } else if (color2 == FICHA_BLANCA && color1 == FICHA_NEGRA) {
        strcpy(nombreBlanco, jugador2);
        strcpy(nombreNegro, jugador1);
    } else {
        // Ambos jugadores tienen el mismo color.
        return MISMO_COLOR;
    }
    return 0;
}


/* Revisa si lo único que quedan en la linea actual son espacios. Toma un descriptor de archivo y devuelve 1 si encontró el final de la linea y 0 en caso contrario.
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



/* Funciones de librería */



int leerCabecera(FILE* archivo, char nombreNegro[MAX_LARGO_NOMBRE + 1], char nombreBlanco[MAX_LARGO_NOMBRE + 1], char* colorInicial) {
    int encontroError = 0;
    encontroError = leerNombreJugadores(archivo, nombreNegro, nombreBlanco);
    if (encontroError) return encontroError;
    int leidos = fscanf(archivo, " %c \n", colorInicial);
    if(leidos == EOF) {
        // No pudo leer.
        return FIN_PREMATURO;
    }
    if (leidos != 1) {
        // No pudo leer el campo.
        return NO_ENCONTRO_INFO;
    }
    if(!colorValido(*colorInicial)) {
        // El color que arranca no es un color valido (blanco o negro).
        return COLOR_INVALIDO;
    }

    return 0;
}


int lineaVacia(FILE* archivo) {
    long pos = ftell(archivo);
    if(esFinalDeLinea(archivo)) {
        return 1;
    } else {
        fseek(archivo, pos, SEEK_SET);  // esFinalDeLinea avanzo el cursor del archivo.
        return 0;
    }
}


int leerLinea(FILE* archivo, Casilla* casilla) {
    int escaneado = fscanf(archivo, " %c%c", &casilla->columna, &casilla->fila);
    if (escaneado == 2 && casilla->fila != '\n' && casilla->columna != '\n' && esFinalDeLinea(archivo)) {
        // No hubo problemas en la lectura.
        return 0;
    } else {
        // No pudo leer la linea porque tenia un formato incorrecto.
        return MAL_FORMATO_LINEA;
    }
}