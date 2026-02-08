#ifndef LECTURA_OTHELLO
#define LECTURA_OTHELLO


#include <stdio.h>
#include <string.h>
#include "Constantes.h"
#include "Tablero.h"
#include <errno.h>


/* Lee la cabecera del archivo. Toma un descriptor de archivo, dos arreglos en los que pondrá los nombres de los jugadores y un puntero a char 
 *  donde poner el color inicial (Siempre sera FICHA_BLANCA o FICHA_NEGRA). Devuelve 0 si pudo leer correctamente, si no va a
 *  devolver uno de los siguientes códigos de error:
 *    >NO_PUDO_LEER: Llego al final del archivo antes de que termine la cabecera.
 *    >CAMPOS_INSUFICIENTES: No pudo encontrar la información que buscaba.
 *    >NOMBRE_LARGO: El nombre de uno de los jugadores excede MAX_LARGO_NOMBRE.
 *    >COLOR_INVALIDO: Uno de los campos a rellenar con un color contenía un valor invalido (distinto de FICHA_BLANCA o FICHA_NEGRA).
 *    >MISMO_COLOR: Ambos jugadores fueron asignados al mismo color.
 *    >MISMO_NOMBRE: Ambos jugadores fueron asignados el mismo nombre.
 */
int leerCabecera(FILE* archivo, char nombreNegro[MAX_LARGO_NOMBRE + 1], char nombreBlanco[MAX_LARGO_NOMBRE + 1], char* colorInicial);


/* Determina si la siguiente linea está vacía. Toma un descriptor de archivo. Asume que el cursor del archivo esta al inicio de un linea.
 *  Si determina que la linea está vacía, avanza el cursor hasta la siguiente linea, sino lo deja igual. Devuelve 0 si la linea no está vacía y 1 en caso contrario.
 */
int lineaVacia(FILE* archivo);


/* Lee una linea del archivo. Toma un descriptor de archivo y un puntero a una casilla. Asume que el cursor del archivo esta al inicio de una linea.
 *  Si pudo leer sin problemas devuelve 0 y llena la casilla de la que recibió un puntero con la información leída. Si encontró un problema devuelve MALFORMATOLINEA.
 */
int leerLinea(FILE* archivo, Casilla* casilla);


#endif