#ifndef LECTURA_OTHELLO
#define LECTURA_OTHELLO


#include <stdio.h>
#include <string.h>
#include "Constantes.h"


/* Lee la cabecera del archivo. Toma un descriptor de archivo, dos arreglos en los que pondra los nombres de los jugadores y un argumento de error.
 *  Devuelve el color del jugador que empieza (Siempre sera FICHA_BLANCA o FICHA_NEGRA).
 * De encontrar un error de formato devolvera '\0' y llenara errorFormato con uno de los siguentes valores:
 *    >NOPUDOLEER: Llego al final del archivo antes de que termine la cabecera.
 *    >CAMPOSINSUFICIENTES: No pudo encontrar la informacion que buscaba.
 *    >NOMBRELARGO: El nombre de uno de los jugadores excede MAXLARGONOMBRE.
 *    >COLORINVALIDO: Uno de los campos a rellenar con un color contenia un valor invalido (distinto de FICHA_BLANCA o FICHA_NEGRA).
 *    >MISMOCOLOR: Ambos jugadores fueron asignados al mismo color.
 *    >MISMONOMBRE: Ambos jugadores fueron asignados el mismo nombre.
 */
char leerCabecera(FILE* archivo, char nombreNegro[MAXLARGONOMBRE + 1], char nombreBlanco[MAXLARGONOMBRE + 1], int* errorFormato);


/* Determina si la siguiente linea está vacia. Toma un descriptor de archivo. Asume que el cursor del archivo esta al inicio de un linea.
 *  Si deterina que la linea está vacia, avanza el cursor hasta la siguiente linea sino lo deja igual. Devuelve 0 si la linea no está vacia y 1 en caso contrario.
 */
int lineaVacia(FILE* archivo);


/* Lee una linea del archivo. Toma un decriptor de archivo y punteros donde poner los valores de fila y columna encontrados.
 *  Asume que el cursor del archivo esta al inicio de una linea. Si pudo leer sin problemas devuelve 0 sino devuelve MALFORMATOLINEA.
 *  
 */
int leerLinea(FILE* archivo, char* fila, char* columna);


#endif