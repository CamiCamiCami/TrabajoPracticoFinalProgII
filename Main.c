#include "Tablero.h"
#include "Lectura.h"
#include "Constantes.h"
#include <stdio.h>


/* Imprime un mensaje de error especificando el formato correcto con el que se debe invocar el programa.
 */
void imprimirErrorDeUso() {
    printf("El programa toma dos argumentos posicionales: [direccion archivo entrada] y [direccion archivo salida]\n");
}


/* Imprime un mensaje de error diciendo que no se pudo abrir el archivo y especifica el formato correcto con el
 *  que se debe invocar el programa.
 */
void imprimirErrorDeArchivo(char path[]) {
    printf("No se pudo abrir el archivo \"%s\"\n", path);
    imprimirErrorDeUso(); // Un error de apertura puede deberse a un error de uso.
} 


/* Imprime un mensaje de error para cada tipo de error de formato. Toma un entero representando un codigo de error.
 */
void imprimirErrorFormato(int ERROR) {
    switch (ERROR)
    {
    case NOPUDOLEER:
        printf("El archivo terminó   inesperadamente antes de llegar al fin de la cabecera.\n");
        printf("El formato de la cabecera debe ser:\n\n[Nombre del Jugador1],[Color del Jugador1]\n[Nombre del Jugador2],[Color del Jugador2]\n[Color que empieza el juego]\n");
        break;
    case MALFORMATOCABECERA:
        printf("El formato de la cabecera debe ser:\n\n[Nombre del Jugador1],[Color del Jugador1]\n[Nombre del Jugador2],[Color del Jugador2]\n[Color que empieza el juego]\n");
        break;
    case NOMRELARGO:
        printf("El nombre de uno de los jugadores es demasiado largo. El largo maximo para un nombre es de %i caracteres (contando espacios).\n", MAXLARGONOMBRE);
        break;
    case COLORINVALIDO:
        printf("Unos de los colores en la cabecera es invalido. Los colores deben aparecer como %c (blanco) o %c (negro).\n", FICHA_BLANCA, FICHA_NEGRA);
        break;
    case MISMOCOLOR:
        printf("Los dos jugadores estaban asignados al mismo color. Por favor asigneles colores distintos\n");
        break;
    case MISMONOMBRE:
        printf("Los dos jugadores fueron dados el mismo nombre. Por favor asigneles nombres distintos\n");
        break;
    case MALFORMATOLINEA:
        printf("El formato de cada una de las lineas debe ser:\n\n[columna de la jugada][fila de la jugada]\n");
        break;
    default:
        printf("Hubo un error en el formato del archivo de entrada.\n");
        break;
    }
}

/* Imprime un mensaje de error para cada tipo de jugada invalida. Toma un entero representando el tipo de jugada invalida,
 *  el numero del turno en el que se produjo la jugada y el nombre del jugador responsable por el error.
 */
void imprimirJugadaInvalida(int error, int nroJugada, char jugador[]) {
    switch (error)
    {
    case PASOILEGAL:
        printf("En la jugada Nro %i, %s pasó cuando tenía jugadas posibles\n", nroJugada, jugador);
        break;
    case FICHAILEGAL:
        printf("En la jugada Nro %i, %s quiso poner una ficha en una posición no válida\n", nroJugada, jugador);
        break;
    default:
        printf("En la jugada Nro %i, %s hizo una jugada inválida\n", nroJugada, jugador);
        break;
    }
}


/* Abre un archivo en el modo pedido. Toma una dirección, un modo y un puntero a la varaible donde se guardara
 *  el descriptor de archivo. Devuelve 0 si el archivo se abrió exitosamente y NOPUDOABRIR en caso contrario.
 */
int abrirArchivo(char direccion[], const char modo[], FILE** archivo) {
    *archivo = fopen(direccion, modo);
    if (*archivo == NULL) {
        // No pudo abrir correctamente el archivo.
        imprimirErrorDeArchivo(direccion);
        return NOPUDOABRIR;
    }
    return 0;
}


int main(int args, char** argv) {
    int errorUso = 0, errorFormato = 0, hizoJugadaInvalida = 0; // Variables de error
    if(args != 3) {
        // Faltan/sobran argumentos.
        imprimirErrorDeUso();
        return MALUSO;
    }

    char *direccionEntrada = argv[1], *direccionSalida = argv[2];
    FILE* archivoEntrada;
    errorUso = abrirArchivo(direccionEntrada, "r", &archivoEntrada);
    if(errorUso) return errorUso;

    char jugadorN[MAXLARGONOMBRE + 1], jugadorB[MAXLARGONOMBRE + 1];
    char colorJugando;
    errorFormato = leerCabecera(archivoEntrada, jugadorN, jugadorB, &colorJugando);
    if (errorFormato) {
        // Hubo un error leyendo la cabecera.
        imprimirErrorFormato(errorFormato);
        fclose(archivoEntrada);
        return errorFormato;
    }

    Tablero tablero = crearTablero();
    char filajugada, columnajugada;
    int nroLinea = 3; // La cabecera ocupa 3 lineas
    while(!hizoJugadaInvalida && !errorFormato && !feof(archivoEntrada)) {
        nroLinea++;
        if(lineaVacia(archivoEntrada)) {
            if(tieneJugada(tablero, colorJugando)) {
                // El jugador paso cuando tenia una jugada.
                hizoJugadaInvalida = PASOILEGAL;
            } else {
                colorJugando = colorOpuesto(colorJugando);
            }
        } else {
            errorFormato = leerLinea(archivoEntrada, &filajugada, &columnajugada);
            if(!errorFormato) {
                hizoJugadaInvalida = colocarFicha(tablero, filajugada, columnajugada, colorJugando);
                if(!hizoJugadaInvalida) {
                    colorJugando = colorOpuesto(colorJugando);
                }
            }
        }
    }
    fclose(archivoEntrada);

    if(errorFormato) {
        imprimirErrorFormato(errorFormato);
        liberarTablero(tablero);
        return errorFormato;
    } 
    if(hizoJugadaInvalida) {
        // Se produjo una jugada ilegal
        imprimirJugadaInvalida(hizoJugadaInvalida, nroLinea, colorJugando == FICHA_BLANCA ? jugadorB : jugadorN);
        imprimirTablero(tablero);
        liberarTablero(tablero);
        return 0; //  ????
    }
    if(!tieneJugada(tablero, FICHA_BLANCA) && !tieneJugada(tablero, FICHA_NEGRA)) { // Es necesario?
        // Termino el juego
        switch (darGanador(tablero)) {
        case FICHA_NEGRA:
            printf("Gano %s, quien jugaba com las fichas negras.\n", jugadorN);
            break;
        case FICHA_BLANCA:
            printf("Gano %s, quien jugaba com las fichas blancas.\n", jugadorB);
            break;
        default:
            printf("El juego resulto en un empate.\n");
            break;
        }
        liberarTablero(tablero);
        return 0;
    }
        
    // El juego quedo a medias
    FILE* archivoSalida;
    errorUso = abrirArchivo(direccionSalida, "w", &archivoSalida);
    if (errorUso) {
        // No pudo abrir correctamente el archivo.
        liberarTablero(tablero);
        return errorUso;
    }
    escribirTablero(archivoSalida, tablero);
    fprintf(archivoSalida, "%c\n", colorJugando);
    liberarTablero(tablero);
    fclose(archivoSalida);
    return 0;
}


