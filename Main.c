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


/* Imprime un mensaje de error para cada tipo de error de formato. Toma un entero representando un código de error.
 */
void imprimirErrorFormato(int ERROR) {
    switch (ERROR)
    {
    case FIN_PREMATURO:
        printf("El archivo terminó   inesperadamente antes de llegar al fin de la cabecera.\n");
        printf("El formato de la cabecera debe ser:\n\n[Nombre del Jugador1],[Color del Jugador1]\n[Nombre del Jugador2],[Color del Jugador2]\n[Color que empieza el juego]\n");
        break;
    case NO_ENCONTRO_INFO:
        printf("El formato de la cabecera debe ser:\n\n[Nombre del Jugador1],[Color del Jugador1]\n[Nombre del Jugador2],[Color del Jugador2]\n[Color que empieza el juego]\n(El largo máximo para cada nombre es de %i caracteres contando espacios)\n", MAX_LARGO_NOMBRE);
        break;
    case COLOR_INVALIDO:
        printf("Unos de los colores en la cabecera es invalido. Los colores deben aparecer como %c (blanco) o %c (negro).\n", FICHA_BLANCA, FICHA_NEGRA);
        break;
    case MISMO_COLOR:
        printf("Los dos jugadores estaban asignados al mismo color. Por favor asígneles colores distintos.\n");
        break;
    case MISMO_NOMBRE:
        printf("Los dos jugadores fueron dados el mismo nombre. Por favor asígneles nombres distintos.\n");
        break;
    case MAL_FORMATO_LINEA:
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
    case PASO_ILEGAL:
        printf("En la jugada Nro %i, %s pasó cuando tenía jugadas posibles\n", nroJugada, jugador);
        break;
    case FICHA_ILEGAL:
        printf("En la jugada Nro %i, %s quiso poner una ficha en una posición no válida\n", nroJugada, jugador);
        break;
    default:
        printf("En la jugada Nro %i, %s hizo una jugada inválida\n", nroJugada, jugador);
        break;
    }
}


int main(int args, char** argv) {
    int errorFormato = 0, hizoJugadaInvalida = 0; // Variables de error
    if(args != 3) {
        // Faltan/sobran argumentos.
        imprimirErrorDeUso();
        return MAL_USO;
    }

    // Abre el archivo del que se va a leer
    char *direccionEntrada = argv[1], *direccionSalida = argv[2];
    FILE* archivoEntrada = fopen(direccionEntrada, "r");
    if(archivoEntrada == NULL) {
        imprimirErrorDeArchivo(direccionEntrada);
        return NO_PUDO_ABRIR;
    }
    

    // Lee las primeras tres lineas del archivo de entrada
    char jugadorN[MAX_LARGO_NOMBRE + 1], jugadorB[MAX_LARGO_NOMBRE + 1];
    char colorJugando;
    errorFormato = leerCabecera(archivoEntrada, jugadorN, jugadorB, &colorJugando);
    if (errorFormato) {
        // Hubo un error leyendo la cabecera.
        imprimirErrorFormato(errorFormato);
        fclose(archivoEntrada);
        return errorFormato;
    }

    // Recorre el archivo de entrada 
    Tablero tablero = crearTablero();
    Casilla casillaJugada;
    int nroLinea = 0;
    while(!hizoJugadaInvalida && !errorFormato && !feof(archivoEntrada)) {
        nroLinea++;
        if(lineaVacia(archivoEntrada)) {
            if(tieneJugada(tablero, colorJugando)) {
                // El jugador paso cuando tenia una jugada.
                hizoJugadaInvalida = PASO_ILEGAL;
            } else {
                colorJugando = colorOpuesto(colorJugando);
            }
        } else {
            errorFormato = leerLinea(archivoEntrada, &casillaJugada);
            if(!errorFormato) {
                hizoJugadaInvalida = colocarFicha(tablero, casillaJugada, colorJugando);
                if(!hizoJugadaInvalida) {
                    colorJugando = colorOpuesto(colorJugando);
                }
            }
        }
    }
    fclose(archivoEntrada);

    if(errorFormato) {
        // El archivo de entrada tenia un formato inesperado
        imprimirErrorFormato(errorFormato);
        liberarTablero(tablero);
        return errorFormato;
    } 
    if(hizoJugadaInvalida) {
        // Se produjo una jugada ilegal
        imprimirJugadaInvalida(hizoJugadaInvalida, nroLinea, colorJugando == FICHA_BLANCA ? jugadorB : jugadorN);
        imprimirTablero(tablero);
        liberarTablero(tablero);
        return EXIT_SUCCESS;
    }
    if(!tieneJugada(tablero, FICHA_BLANCA) && !tieneJugada(tablero, FICHA_NEGRA)) {
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
        return EXIT_SUCCESS;
    }
        
    // El juego no termino
    FILE* archivoSalida = fopen(direccionSalida, "w");
    if (archivoSalida == NULL) {
        // No pudo abrir correctamente el archivo de salida.
        imprimirErrorDeArchivo(direccionSalida);
        liberarTablero(tablero);
        return NO_PUDO_ABRIR;
    }
    escribirTablero(archivoSalida, tablero);
    fprintf(archivoSalida, "%c\n", colorJugando);
    liberarTablero(tablero);
    fclose(archivoSalida);
    return EXIT_SUCCESS;
}


