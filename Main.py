from typing import Tuple, Literal
import sys


FICHA_BLANCA = "B"
FICHA_NEGRA = "N"
CASILLA_VACIA = "X"
FILAS = ["1", "2", "3", "4", "5", "6", "7", "8"]
COLUMNAS = ["A", "B", "C", "D", "E", "F", "G", "H"]


### Diseño de datos
Casilla = Tuple[int, int] # El primer entero representa la fila y el segundo la columna. Ambos deben estar entre 1 y 8
ColorFicha = str # Sera siempre FICHA_BLANCA o FICHA_NEGRA
Tablero = dict[Casilla, ColorFicha]
JugadasPosibles = set[Casilla]


def casilla2str(casilla: Casilla) -> str:
    return COLUMNAS[casilla[1] - 1] + FILAS[casilla[0] - 1]


def casillaValida(casilla: Casilla):
    fila, col = casilla
    return 1 <= fila and fila <= 8 and 1 <= col and col <= 8


def assertColorFicha(ficha: str) -> ColorFicha:
    assert ficha == FICHA_BLANCA or ficha == FICHA_NEGRA  # Deberia cumplirse siempre si el formato del archivo de entrada es el correcto
    return ficha


def assertNivelBot(nivel: int) -> int:
    assert nivel == 0 or nivel == 1
    return nivel


def colorOpuesto(ficha: ColorFicha) -> ColorFicha:
    return FICHA_BLANCA if ficha == FICHA_NEGRA else FICHA_NEGRA


def leerArchivoEntrada(direccionEntrada: str) -> Tuple[Tablero, ColorFicha]:
    tablero: Tablero = dict()
    archivoEntrada = open(direccionEntrada, "r")
    for fila in range(1, 9):
        for columna in range(1, 9):
            casilla = archivoEntrada.read(1)
            if not casilla == CASILLA_VACIA:
                tablero[(fila, columna)] = assertColorFicha(casilla)
        assert archivoEntrada.read(1) == '\n' # Deberia cumplirse siempre si el formato del archivo de entrada es el correcto
    fichaInicial = assertColorFicha(archivoEntrada.read(1))
    archivoEntrada.close()
    return tablero, fichaInicial


def printTablero(tablero: Tablero) -> None:
    print("X|ABCDEFGH")
    print("-----------")
    for fila in range(1, 9):
        print(f"{fila}|", end="")
        for columna in range(1, 9):
            if (fila, columna) not in tablero:
                print(" ", end="")
            else:
                print(tablero[(fila, columna)], end="")
        print("\n", end="")


def agregarAdyacentesVacias(tablero: Tablero, adyacentes: set[Casilla], casilla: Casilla) -> None:
    fila, columna = casilla
    for movFila in [-1, 0, 1]:
        for movCol in [-1, 0, 1]:
            casillaAdyacente = (fila + movFila, columna + movCol)
            if casillaValida(casillaAdyacente) and casillaAdyacente not in tablero:
                # Agrega casillas vacias adyacentes a casillas con fichas
                adyacentes.add(casillaAdyacente)


def encontrarCasillasAdyacentes(tablero: Tablero) -> set[Casilla]:
    adyacentes: set[Casilla] = set()
    for fila in range(1, 9):
        for columna in range(1, 9):
            if (fila, columna) in tablero:
                agregarAdyacentesVacias(tablero, adyacentes, (fila, columna))
    return adyacentes


def hayFichaTrasDireccion(tablero: Tablero, primerCasilla: Casilla, direccion: Tuple[int, int], color: ColorFicha) -> bool:
    fila, columna = primerCasilla
    movFila, movCol = direccion
    if primerCasilla not in tablero:
        # La ficha puesta no captura en esta direccion
        return False
    elif tablero[primerCasilla] == color:
        # La ficha puesta captura en esta direccion
        return True
    else:
        # Tiene que seguir buscando
        return hayFichaTrasDireccion(tablero, (fila + movFila, columna + movCol), direccion, color)


def capturarDireccion(tablero: Tablero, primerCasilla: Casilla, direccion: Tuple[int, int], color: ColorFicha, modificarTablero = True) -> int:
    fila, columna = primerCasilla
    movFila, movCol = direccion
    if tablero[primerCasilla] == color:
        # Termino de capturar
        return 0
    else:
        # Tiene que seguir capturando
        if modificarTablero:
            tablero[primerCasilla] = color
        return capturarDireccion(tablero, (fila + movFila, columna + movCol), direccion, color) + 1


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
def hacerJugada(tablero: Tablero, casilla: Casilla, color: ColorFicha, modificarTablero = True) -> int:
    if casilla in tablero:
        return 0
    fila, columna = casilla
    fichasCapturadas = 0
    for movFila in [-1, 0, 1]:
        for movCol in [-1, 0, 1]:
            primerCasillaEnDireccion = (fila + movFila, columna + movCol)
            hayFichaInicioDireccion = primerCasillaEnDireccion in tablero and tablero[primerCasillaEnDireccion] == colorOpuesto(color) # Siempre sera falso cuando movFila = movCol = 0
            if hayFichaInicioDireccion and hayFichaTrasDireccion(tablero, primerCasillaEnDireccion, (movFila, movCol), color):
                fichasCapturadas += capturarDireccion(tablero, primerCasillaEnDireccion, (movFila, movCol), color, modificarTablero)
    if not fichasCapturadas == 0 and modificarTablero:
        tablero[casilla] = color
    return fichasCapturadas


def actualizarCasillasAdyacentes(tablero: Tablero, casillasAdyacentes: set[Casilla], jugada: Casilla):
    casillasAdyacentes.remove(jugada)
    agregarAdyacentesVacias(tablero, casillasAdyacentes, jugada)


def jugadaEsValida(tablero: Tablero, casilla: Casilla, color: ColorFicha):
    return not hacerJugada(tablero, casilla, color, modificarTablero=False) == 0


def turnoBot(tablero: Tablero, casillasAdyacentes: set[Casilla], colorBot: ColorFicha, nivel: int) -> bool:
    assertNivelBot(nivel)
    if nivel == 0:
        return turnoBot0(tablero, casillasAdyacentes, colorBot)
    else:
        return turnoBot1(tablero, casillasAdyacentes, colorBot)


def hayJugada(tablero: Tablero, casillasAdyacentes: set[Casilla], color: ColorFicha) -> bool:
    enocontroJugadaValida = False
    for casilla in casillasAdyacentes:
        enocontroJugadaValida = enocontroJugadaValida or jugadaEsValida(tablero, casilla, color)
    return enocontroJugadaValida


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
def turnoJugador(tablero: Tablero, casillasAdyacentes: set[Casilla], colorJugador: ColorFicha) -> bool:
    if not hayJugada(tablero, casillasAdyacentes, colorJugador):
        input("No tiene jugada. Presione enter para continuar")
        return False
    jugada = pedirJugada(tablero, casillasAdyacentes, colorJugador)
    actualizarCasillasAdyacentes(tablero, casillasAdyacentes, jugada)
    return True


def fila2indice(fila: str) -> int:
    try:
        if 1 <= int(fila) and int(fila) <= 8:
            return int(fila)
        else:
            return -1 
    except ValueError:
        return -1


def columna2indice(columna: str) -> int:
    indice = ord(columna) - ord("A") + 1
    if 1 <= indice and indice <= 8:
        return indice
    else:
        return -1 


def pedirJugada(tablero: Tablero, casillasAdyacente: set[Casilla], color: ColorFicha) -> Casilla:
    jugadaEsValida = False
    jugada = (0, 0)
    while not jugadaEsValida:
        jugadaInput = input("Ingrese la casilla en la que desea poner su proxima ficha: ")
        if not len(jugadaInput) == 2:
            print("Ingrese la casilla en el formato [columna][fila]")
        elif columna2indice(jugadaInput[0]) == -1 or fila2indice(jugadaInput[1]) == -1:
            print("La fila debe ser uno de los siguientes: \"A\", \"B\", \"C\", \"D\", \"E\", \"F\", \"G\" o \"H\"")
            print("La columna debe ser uno de los siguientes: \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\" o \"8\"")
        else:
            jugada = (fila2indice(jugadaInput[1]), columna2indice(jugadaInput[0]))
            if jugada in tablero:
                print("La casilla esta ocupada")
            elif not (jugada in casillasAdyacente and hacerJugada(tablero, jugada, color)):
                print("Solo se pueden poner fichas en lugares donde capturen piezas enemigas")
            else:
                # La ficha se pudo poner
                jugadaEsValida = True
    return jugada
            

### Turno del bot 0
# 0) Si hay jugadas posibles no vistas -> 1 sino -> 5
# 1) Elegir una jugada de las jugadas posibles
# 2) Si la jugada es invalida -> 0 sino -> 3
# 3) Hacer la jugada
# 4) Eliminar la casilla jugada de las jugadas posibles y agregar sus casillas adyacentes vacias
# 5) finalizar
# ###
def turnoBot0(tablero: Tablero, casillasAdyacentes: set[Casilla], colorBot: ColorFicha) -> bool:
    hizoJugada = False
    jugada = (0, 0)
    for casilla in casillasAdyacentes:
        if not hizoJugada:
            capturas = hacerJugada(tablero, casilla, colorBot)
            hizoJugada = not capturas == 0
            jugada = casilla
    if hizoJugada:
        print(f"Turno de la maquina: {casilla2str(jugada)}")
        actualizarCasillasAdyacentes(tablero, casillasAdyacentes, jugada)
    else:
        print("Turno de la maquina: Paso")
    return hizoJugada


### Turno del bot 1
# 0) Si hay jugadas posibles no vistas -> 1 sino -> 4
# 1) Elegir una jugada de las jugadas posibles
# 2) Si la jugada es invalida -> 0 sino -> 3
# 3) Contar cuantas fichas esa jugada capturaria -> 0
# 4) Si encontro alguna jugada valida -> 5 sino -> 7
# 5) Jugar la jugada que capturaria la mayor cantidad de fichas
# 6) Eliminar la casilla jugada de las jugadas posibles y agregar sus casillas adyacentes vacias
# 7) finalizar
# ###
def turnoBot1(tablero: Tablero, casillasAdyacentes: set[Casilla], colorBot: ColorFicha):
    maxCapturas = 0
    mejorJugada = (0, 0)
    for casilla in casillasAdyacentes:
        capturas = hacerJugada(tablero, casilla, colorBot, modificarTablero=False)
        if capturas > maxCapturas:
            maxCapturas = capturas
            mejorJugada = casilla
    if not maxCapturas == 0:
        print(f"Turno de la maquina: {casilla2str(mejorJugada)}")
        hacerJugada(tablero, mejorJugada, colorBot)
        actualizarCasillasAdyacentes(tablero, casillasAdyacentes, mejorJugada)
    else:
        print("Turno de la maquina: Paso")
    return not maxCapturas == 0


def determinarGanador(tablero: Tablero) -> str:
    blanco = 0
    negro = 0
    for fila in range(1, 9):
        for columna in range(1, 9):
            if (fila, columna) in tablero:
                if tablero[(fila, columna)] == FICHA_BLANCA:
                    blanco += 1
                else:
                    negro += 1
    if blanco > negro:
        return FICHA_BLANCA
    elif negro > blanco:
        return FICHA_NEGRA
    else:
        return CASILLA_VACIA


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
def jugar(direccionEntrada: str, colorJugador: ColorFicha, nivelBot: int) -> None:
    colorBot = colorOpuesto(colorJugador)
    tablero, colorJugando = leerArchivoEntrada(direccionEntrada)
    printTablero(tablero)
    casillasAdyacentes = encontrarCasillasAdyacentes(tablero)
    pasosSeguidos = 0  # Contador de cuantos jugadores seguidos pasaron (si llega a 2 significa que el juego termino) 
    while pasosSeguidos < 2:
        pudoJugar = False
        if colorJugando == colorBot:
            pudoJugar = turnoBot(tablero, casillasAdyacentes, colorBot, nivelBot)
        else:
            pudoJugar = turnoJugador(tablero, casillasAdyacentes, colorJugador)
        if not pudoJugar:
            pasosSeguidos += 1
        else:
            pasosSeguidos = 0
        printTablero(tablero)
        colorJugando = colorOpuesto(colorJugando)
    ganador = determinarGanador(tablero)
    if ganador == colorBot:
        print("Gano la maquina!")
    elif ganador == colorJugador:
        print("Gano el jugador!")
    else:
        print("Hubo un empate.")


if __name__ == '__main__':
    colorJugador = assertColorFicha(sys.argv[2])
    nivelBot = assertNivelBot(int(sys.argv[3]))
    jugar(sys.argv[1], colorJugador, nivelBot)