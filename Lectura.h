#ifndef LECTURA_OTHELLO
#define LECTURA_OTHELLO


#include <stdio.h>
#include <string.h>
#include "Tablero.h"


#define MAXLARGONOMBRE 50
/* Errores de Usuario */
#define MALUSO 1
#define NOPUDOABRIR 2
/* Errores de Formato */
#define NOPUDOLEER 3
#define CAMPOSINSUFICIENTES 4
#define NOMRELARGO 5
#define COLORINVALIDO 6
#define MISMOCOLOR 7
#define MISMONOMBRE 8


/* Lee la cabecera del archivo. Toma el archivo del que leer, dos arreglos en los que pondra los nombres de los jugadores y un argumento de error.
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


#endif