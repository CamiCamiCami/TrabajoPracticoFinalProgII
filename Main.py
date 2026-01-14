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


def actualizarCasillasAdyacentes(tablero: Tablero, casillasAdyacentes: set[Casilla], jugada: Casilla):
    casillasAdyacentes.remove(jugada)
    casillasAdyacentes.union(casillasAdyacentesVacias(tablero, jugada))


def jugadaEsValida(tablero: Tablero, casilla: Casilla, color: ColorFicha):
    return not hacerJugada(tablero, casilla, color, modificarTablero=False) == 0


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


# ###   leerArchivoEntrada
#   Lee el archivo de entrada. Toma la direccion en la que se puede encontrar el archivo de entrada y lee el tablero sobre el que se va a jugar 
# y el color del jugador que empieza. Devuelve una tupla con el tablero resultante y el color del jugador que va a arrancar.
def leerArchivoEntrada(direccionEntrada: str) -> Tuple[Tablero, ColorFicha]:
    tablero: Tablero = dict()
    archivoEntrada = open(direccionEntrada, "r")
    for fila in range(1, 9):
        for columna in range(1, 9):
            casilla = archivoEntrada.read(1)
            if not casilla == CASILLA_VACIA:
                # Solo las casillas no vacias se agregan al tablero
                tablero[(fila, columna)] = assertColorFicha(casilla)
        assert archivoEntrada.read(1) == '\n' # Deberia cumplirse siempre si el formato del archivo de entrada es el correcto
    fichaInicial = assertColorFicha(archivoEntrada.read(1))
    archivoEntrada.close()
    return tablero, fichaInicial


CASILLA_VACIA = " "

# ###   printTablero
#   Imprime el tablero. Toma un tablero y lo imprime a la consola, añadiendo un borde que facilita la identificacion de las coordenadas de cada casilla.
def printTablero(tablero: Tablero) -> None:
    print("X|ABCDEFGH")
    print("-----------")
    for fila in range(1, 9):
        print(f"{fila}|", end="")
        for columna in range(1, 9):
            if (fila, columna) not in tablero:
                print(CASILLA_VACIA, end="")
            else:
                print(tablero[(fila, columna)], end="")
        print("\n", end="")


# ###   casillasAdyacentesVacias
#   Devuelve un conjunto con las casillas vacias adyacentes (ortogonal y diagonalmente) a la casilla dada. Toma el tablero y una casilla.
def casillasAdyacentesVacias(tablero: Tablero, casilla: Casilla) -> set[Casilla]:
    fila, columna = casilla
    adyacentes = set()
    for movFila in [-1, 0, 1]:
        for movCol in [-1, 0, 1]:
            casillaAdyacente = (fila + movFila, columna + movCol)
            if casillaValida(casillaAdyacente) and casillaAdyacente not in tablero:
                # Agrega casillas vacias adyacentes a casillas con fichas
                adyacentes.add(casillaAdyacente)
    return adyacentes


# ###   encontrarCasillasAdyacentes
#   Toma un tablero y devuelve el conjunto de todas las casillas vacias que estan adyacentes a una casilla no vacia. Aunque no todas las casillas de este conjunto
# son jugadas validas, si se cumple que todas las jugadas validas pertenecen al conjunto. Esto facilita eliminar candidatos de jugadas.
def encontrarCasillasAdyacentes(tablero: Tablero) -> set[Casilla]:
    adyacentes: set[Casilla] = set()
    for fila in range(1, 9):
        for columna in range(1, 9):
            if (fila, columna) in tablero:
                adyacentes.union(casillasAdyacentesVacias(tablero, (fila, columna)))
    return adyacentes


# ###   hayFichaTrasDireccion
#   Revisa una direccion para ver si eventualmente aparecen una ficha del color buscado. Toma el tablero, la primer casilla que aparece en la direccion buscada, 
# una direccion hacia la que se dirige y el color de la ficha buscada.
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


# ###   capturarDireccion
#   Captura las fichas hacia una direccion dada, frenando cuando se encuentra una ficha del color buscado. Toma el tablero, la primer casilla que aparece en la , 
# direccion buscada una direccion hacia la que se dirige y el color de la ficha buscada. Adicionalmente, el parametro opcional modificarTablero determina si las
# fichas se capturan (son rempalazadas por fichas del color opuesto) o si solo cuenta las fichas que ese movimiento capturaria.
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
        return capturarDireccion(tablero, (fila + movFila, columna + movCol), direccion, color, modificarTablero) + 1

# ###   hacerJugada
#   Intenta hacer una jugada. Toma el tablero, la casilla donde se hara la jugada y el color de la ficha a poner. Adicionalmente, el parametro
# modificarTablero determina si la funcion hace cambios en el tablero o si simplemente determina si la jugada se podria hacer.
def hacerJugada(tablero: Tablero, casilla: Casilla, color: ColorFicha, modificarTablero = True) -> int:
    if casilla in tablero:
        return 0
    print(tablero)
    fila, columna = casilla
    fichasCapturadas = 0
    for movFila in [-1, 0, 1]:
        for movCol in [-1, 0, 1]:
            primerCasillaEnDireccion = (fila + movFila, columna + movCol)
            hayFichaInicioDireccion = primerCasillaEnDireccion in tablero and tablero[primerCasillaEnDireccion] == colorOpuesto(color) # Siempre sera falso cuando movFila = movCol = 0
            if hayFichaInicioDireccion and hayFichaTrasDireccion(tablero, primerCasillaEnDireccion, (movFila, movCol), color):
                # Se puede capturar hacia esta direccion
                fichasCapturadas += capturarDireccion(tablero, primerCasillaEnDireccion, (movFila, movCol), color, modificarTablero)
        tablero[casilla] = color
    return fichasCapturadas


# ###   turnoBot
#   Hace una jugada como un bot de nivel dado. Toma el tablero, un conjunto con todas las casillas vacias adyacentes a una ficha, el color del bot y el nivel de este.
# El nivel del bot solo puede tener valores 0 o 1. El conjunto pasado por casillasAdyacentes se modifica para reflejar el nuevo estado del tablero con la ficha. Devuelve
# verdadero si pudo poner una ficha y falso si tuvo que pasar.
def turnoBot(tablero: Tablero, casillasAdyacentes: set[Casilla], colorBot: ColorFicha, nivel: int) -> bool:
    assertNivelBot(nivel)
    if nivel == 0:
        return turnoBot0(tablero, casillasAdyacentes, colorBot)
    else:
        return turnoBot1(tablero, casillasAdyacentes, colorBot)


# ###   hayJugada
#    Determina si el jugador de un color determinado tiene una jugada disponible. Toma el tablero, un conjunto con las casillas vacias adyacentes a una ficha y el color
# del jugador a evaluar.
def hayJugada(tablero: Tablero, casillasAdyacentes: set[Casilla], color: ColorFicha) -> bool:
    enocontroJugadaValida = False
    for casilla in casillasAdyacentes:
        enocontroJugadaValida = enocontroJugadaValida or jugadaEsValida(tablero, casilla, color)
    return enocontroJugadaValida




# ###   turnoJugador
#   Se encarga del turno del jugador. Toma el tablero, un conjunto con las casillas vacias adyacentes a una ficha y el color del jugador. Devuelve verdadero
# si el jugador pudo poner una ficha y falso si tuvo que pasar.
def turnoJugador(tablero: Tablero, casillasAdyacentes: set[Casilla], colorJugador: ColorFicha) -> bool:
    if not hayJugada(tablero, casillasAdyacentes, colorJugador):
        input("No tiene jugada. Presione enter para continuar")
        return False

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
            elif not (jugada in casillasAdyacentes and hacerJugada(tablero, jugada, colorJugador)):
                print("Solo se pueden poner fichas en lugares donde capturen piezas enemigas")
            else:
                # La ficha se pudo poner
                jugadaEsValida = True
    actualizarCasillasAdyacentes(tablero, casillasAdyacentes, jugada)
    return True
            



# ### turnoBot0
#   Elige una jugada aleatoria entre las posibles y la hace. Toma el tablero, un conjunto con las casillas vacias adyacentes a una ficha y el color del bot.
# Devuelve verdadero si encontro una jugada valida para hacer y falso si tuvo que pasar.
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

# ### turnoBot1
#   Elige la jugada que mas capturas logre entre las posibles y la hace. Toma el tablero, un conjunto con las casillas vacias adyacentes a una ficha y el color del bot.
# devuelve verdadero si encontro una jugada valida y falso si tuvo que pasar.
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


# ### determinarGanador
#   Determina que color tiene mas fichas en el tablero al final del juego. Toma un tablero terminado y devuelve FICHA_BLANCA en caso de que haya mas fichas blancas, FICHA_NEGRA 
# si hay mas fichas negras y otra cosa (especificamente CASILLA_VACIA) en caso de empate.
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