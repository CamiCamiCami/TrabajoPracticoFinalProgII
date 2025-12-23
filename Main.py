from typing import Tuple, Literal
import sys


FICHA_BLANCA = "B"
FICHA_NEGRA = "N"
CASILLA_VACIA = "X"
FILAS = ["1", "2", "3", "4", "5", "6", "7", "8"]
COLUMNAS = ["A", "B", "C", "D", "E", "F", "G", "H"]


### Diseño de datos
Casilla = Tuple[str, str] # Ambos str son siempre de largo 1. El primer componente estara entre "A" y "H". El segundo componente estara entre "1" y "8".
ColorFicha = str # Sera siempre FICHA_BLANCA o FICHA_NEGRA
Tablero = dict[Casilla, ColorFicha]
JugadasPosibles = set[Casilla]


### Programa General 
# 0) Procesar argumentos de entrada
# 1) Leer archivo de C
# 1.0) Crear el diccionario
# 1.1) Leer linea
# 1.1.1) Si el caracter no es CASILLA_VACIA -> 1.1.2 sino -> 1.1.3
# 1.1.2) Agregar la Ficha correspondiente en la casilla
# 1.1.3) Repetir 1.1.1 hasta llegar al final de la linea
# 1.2) Repetir 1.1 para todas las lineas
# 1.3) Leer el ultimo caracter para decidir quien empieza
# 1.4) Imprimir el tablero
# 2) Jugar
# 2.0) Si empieza el jugador -> 2.1 sino -> 2.4
# 2.1) Turno del jugador
# 2.2) Imprimir el tablero
# 2.3) Si no termino el juego -> 2.4 sino -> 3
# 2.4) Turno del bot
# 2.5) Imprimir el tablero
# 2.6) Si no termino el juego -> 2.1 sino -> 3
# 3) Fin del juego
# ###

### Turno del Jugador
# 1) Si el jugador tiene jugada posible -> 2 sino -> 5
# 2) Pedir una jugada
# 2.1) Si el formato de la jugada es valido -> 2.2 sino -> 3
# 2.2) Si la jugada es valida -> 2.3 sino -> 4
# 2.3) Hacer la jugada
# 2.4) Eliminar la casilla jugada de las jugadas posibles y agregar sus casillas adyacentes vacias -> 5
# 3) Imprimir mensaje de error de formato -> 2
# 4) Imprimir formato de jugada invalida -> 2
# 5) Finalizar
# ###

### Turno del bot 0
# 0) Si hay jugadas posibles no vistas -> 1 sino -> 5
# 1) Elegir una jugada de las jugadas posibles
# 2) Si la jugada es invalida -> 0 sino -> 3
# 3) Hacer la jugada
# 4) Eliminar la casilla jugada de las jugadas posibles y agregar sus casillas adyacentes vacias
# 5) finalizar
# ###

### Turno del bot 1
# 0) Si hay jugadas posibles no vistas -> 1 sino -> 4
# 1) Elegir una jugada de las jugadas posibles
# 2) Si la jugada es invalida -> 0 sino -> 3
# 3) Contar cuantas fichas esa jugada capturaria -> 0
# 4) Si encontro alguna jugada valida -> 4 sino -> 7
# 5) Jugar la jugada que capturaria la mayor cantidad de fichas
# 6) Eliminar la casilla jugada de las jugadas posibles y agregar sus casillas adyacentes vacias
# 7) finalizar
# ###

### Hacer una jugada
# 1) Si la posicion no esta en el tablero -> 2 sino -> 8
# 2) Chequear inicio direcciones
# 2.1) Si la primer casilla pertenece al tablero -> 2.2 sino -> 5
# 2.2) Si la primer ficha es del color opuesto -> 3 sino -> 5
# 3) Chequear fin direcciones
# 3.1) Si la casilla pertenece al tablero -> 3.2 sino -> 5
# 3.2) Si la ficha es del color opuesto -> 3.3 sino -> 4
# 3.3) Avanzar a la siguiente casilla en la direccion -> 3.1
# 4) Cambiar fichas de color en la direccion
# 4.1) Si la ficha es del color opuesto -> 4.2 sino -> 5
# 4.2) Cambiar el color de la ficha
# 4.3) Avanzar una posicion -> 4.1
# 5) Repetir 2 para todas las direcciones
# 6) Si hay al menos una direccion valida -> 7 sino -> 8
# 7) Colocar la ficha en el lugar propuesto
# 8) Finalizar
# ###



def assertColorFicha(ficha: str) -> ColorFicha:
    assert ficha == FICHA_BLANCA or ficha == FICHA_NEGRA  # Deberia cumplirse siempre si el formato del archivo de entrada es el correcto
    return ficha


def assertNivelBot(nivel: int) -> int:
    assert nivel == 0 or nivel == 1
    return nivel



def leerArchivoEntrada(direccionEntrada: str) -> Tuple[Tablero, ColorFicha]:
    tablero: Tablero = dict()
    with open(direccionEntrada, "r") as archivoEntrada:
        for fila in FILAS:
            for columna in COLUMNAS:
                casilla = archivoEntrada.read(1)
                if not casilla == CASILLA_VACIA:
                    tablero[(fila, columna)] = assertColorFicha(casilla)
            assert archivoEntrada.read(1) == '\n' # Deberia cumplirse siempre si el formato del archivo de entrada es el correcto
        fichaInicial = assertColorFicha(archivoEntrada.read(1))
    return tablero, fichaInicial


def printTablero(tablero[])



def jugar(direccionEntrada: str, colorJugador: ColorFicha, nivelBot: int) -> None:
    tablero, colorJugando = leerArchivoEntrada(direccionEntrada)



if __name__ == '__main__':
    colorJugador = assertColorFicha(sys.argv[2])
    nivelBot = assertNivelBot(int(sys.argv[3]))
    jugar(sys.argv[1], colorJugador, nivelBot)