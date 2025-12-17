/* Programa General */

// 0) Procesar argumentos de entrada.
// 1) Procesar la cabecera.
// 1.1) Encontrar jugadores y asociarlos con su color
// 1.1.1) Si los colores son validos (B ó N) y los nombres no son iguales -> 1.2 sino -> 5
// 1.2) Determinar el color que empieza
// 1.2.1) Si es un color valido (B ó N) -> 2 sino -> 5
// 2) Construir tablero
// 2.0) Asignar memoria para un tablero e inicializarlo
// 2.1) Procesar lineas
// 2.1.1) Si la linea está vacia -> 2.1.1.1 sino -> 2.1.2
// 2.1.1.1) Si el jugador no tiene movimientos disponibles -> 2.1.5 sino -> 6 
// 2.1.2) Si la linea tiene formato correcto -> 2.1.2 sino -> 5
// 2.1.3) Si la jugada es valida -> 2.1.4 sino -> 6
// 2.1.4) Aplicar la jugada en el tablero
// 2.1.5) Cambiar de jugador
// 2.2) Repetir 2.1 hasta que no queden lineas
// 2.3) Si el tablero esta completo o ninguno puede hacer un movimiento -> 3 sino -> 4
// 3) Dar por ganandor al jugador con mas fichas o declarar empate de tener igual cantidad -> 7
// 4) Guardar progreso
// 4.1) Escribir tablero a archivo
// 4.2) Escribir jugador siguiente -> 7
// 5) Imprimir guia de formato -> 7
// 6) Manejar movimiento invalido
// 6.1) Imprimir Tablero
// 6.2) Imprimir jugador actual -> 7
// 7) Finalizar
// 7.1) Liberar el tablero
// 7.2) Devolver 0 si salio por 3, 4 o 6 y otro valor si salio por 5.


/* Colocar una ficha */

// Toma: tablero, posicion, color
// 1) Si la posicion esta en el tablero y desocupada -> 2 sino -> 11
// 2) Chequear inicio direcciones
// 2.1) Si la primer casilla pertenece al tablero -> 2.2 sino -> 2.3
// 2.2) Si la primer ficha es del color opuesto -> 3 sino 2.3
// 2.3) Marcar direccion como invalida
// 3) Repetir 2 para todas las direcciones
// 4) Chequear fin direcciones no invalidas
// 4.1) Si la casilla pertenece al tablero -> 4.2 sino 4.6
// 4.2) Si la casilla no está vacia -> 4.3 sino 4.6
// 4.3) Si la casilla es del color opuesto -> 4.4 sino -> 4.5
// 4.4) Avanzar a la siguiente casilla en la direccion -> 4.1
// 4.5) Marcar la direccion como valida
// 4.6) Marcar la direccion como invalida
// 5) Repetir 4 para todas las direcciones no invalidas
// 6) Si hay al menos una direccion valida -> 7 sino -> 11
// 7) Marcar direcciones
// 7.1) Si la ficha es del color opuesto -> 7.2 sino 8
// 7.2) Cambiar el color de la ficha
// 7.3) Avanzar una posicion -> 7.1
// 8) Repetir 7 para todas las direcciones validas
// 9) insertar la ficha
// 10) Devolver 1 -> exit
// 11) Devolver 0 -> exit



#include "Tablero.h"
#include "Lectura.h"
#include "Constantes.h"
#include <stdio.h>



void imprimirErrorDeUso() {
    printf("El programa toma dos argumentos posicionales: [direccion archivo entrada] y [direccion archivo salida]\n");
}


void imprimirErrorDeArchivo(char path[]) {
    printf("No se pudo abrir el archivo \"%s\"\n", path);
    imprimirErrorDeUso(); // Un error de apertura puede deberse a un error de uso.
} 


void imprimirErrorFormato(int ERROR) {
    switch (ERROR)
    {
    case NOPUDOLEER:
        printf("El archivo termino inesperadamente antes de llegar al fin de la cabecera.\n");
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
    default:
        printf("Hubo un error en la lectura de la cabecera.\n");
        break;
    }
}


void imprimirJugadaInvalida(int error, int nroJugada, char jugador[]) {
    switch (error)
    {
    case PASOILEGAL:
        printf("En la jugada Nro %i, %s paso cuando tenia jugadas posibles\n", nroJugada, jugador);
        break;
    case FICHAILEGAL:
        printf("En la jugada Nro %i, %s quiso poner una ficha en una posicion no valida\n", nroJugada, jugador);
        break;
    default:
        printf("En la jugada Nro %i, %s hizo una jugada invalida\n", nroJugada, jugador);
        break;
    }
}


int main(int args, char** argv) {
    if(args != 3) {
        // Faltan/sobran argumentos.
        imprimirErrorDeUso();
        return MALUSO;
    }
    char *direccionEntrada = argv[1], *direccionSalida = argv[2];
    FILE* archivoEntrada = fopen(direccionEntrada, "r");
    if (archivoEntrada == NULL) {
        // No pudo abrir correctamente el archivo.
        imprimirErrorDeArchivo(direccionEntrada);
        return NOPUDOABRIR;
    }

    int errorFormato = 0;
    char jugadorN[MAXLARGONOMBRE + 1], jugadorB[MAXLARGONOMBRE + 1];
    char colorJugando = leerCabecera(archivoEntrada, jugadorN, jugadorB, &errorFormato);
    if (errorFormato) {
        // Hubo un error leyendo la cabecera.
        imprimirErrorFormato(errorFormato);
        fclose(archivoEntrada);
        return errorFormato;
    }

    Tablero tablero = crearTablero();
    char filajugada, columnajugada;
    int jugadaIlegal = 0, nroJugada = 0;
    while(!jugadaIlegal && !errorFormato && !feof(archivoEntrada)) {
        nroJugada++;
        if(lineaVacia(archivoEntrada)) {
            if(tieneJugada(tablero, colorJugando)) {
                // El jugador paso cuando tenia una jugada.
                jugadaIlegal = PASOILEGAL;
            } else {
                colorJugando = colorOpuesto(colorJugando);
            }
        } else {
            errorFormato = leerLinea(archivoEntrada, &filajugada, &columnajugada);
            if(!errorFormato) {
                jugadaIlegal = colocarFicha(tablero, filajugada, columnajugada, colorJugando);
                if(!jugadaIlegal) {
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
    } else if(jugadaIlegal) {
        // Se produjo una jugada ilegal
        imprimirJugadaInvalida(jugadaIlegal, nroJugada, colorJugando == FICHA_BLANCA ? jugadorB : jugadorN);
        imprimirTablero(tablero);
        liberarTablero(tablero);
        return 0; //  ????
    } else if(!tieneJugada(tablero, FICHA_BLANCA) && !tieneJugada(tablero, FICHA_NEGRA)) { // Es necesario?
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
    } else {
        // El juego quedo a medias
        FILE* archivoSalida = fopen(direccionEntrada, "w");
        if (archivoEntrada == NULL) {
            // No pudo abrir correctamente el archivo.
            imprimirErrorDeArchivo(direccionEntrada);
            liberarTablero(tablero);
            return NOPUDOABRIR;
        }
        escribirTablero(archivoSalida, tablero);
        fprintf(archivoSalida, "%c\n", colorJugando);
        liberarTablero(tablero);
        fclose(archivoSalida);
        return 0;
    }

    return 0;







}


