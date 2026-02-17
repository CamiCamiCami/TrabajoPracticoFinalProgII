from typing import Tuple, Literal
import sys
import random


FICHA_BLANCA = "B"
FICHA_NEGRA = "N"
CASILLA_VACIA = "X"
FILAS = ["1", "2", "3", "4", "5", "6", "7", "8"]
COLUMNAS = ["A", "B", "C", "D", "E", "F", "G", "H"]


### Tipo de datos
Casilla = Tuple[int, int] # El primer entero representa la fila y el segundo la columna. Ambos deben estar entre 1 y 8
ColorFicha = str # Sera siempre FICHA_BLANCA o FICHA_NEGRA
Tablero = dict[Casilla, ColorFicha]
JugadasPosibles = set[Casilla]
Direccion = Tuple[int, int]


def casilla2str(casilla: Casilla) -> str:
    return COLUMNAS[casilla[1] - 1] + FILAS[casilla[0] - 1]


def casillaValida(casilla: Casilla):
    fila, col = casilla
    return 1 <= fila and fila <= 8 and 1 <= col and col <= 8


def assertColorFicha(ficha: str) -> None:
    assert ficha == FICHA_BLANCA or ficha == FICHA_NEGRA


def colorOpuesto(ficha: ColorFicha) -> ColorFicha:
    return FICHA_BLANCA if ficha == FICHA_NEGRA else FICHA_NEGRA


def actualizarCasillasAdyacentes(tablero: Tablero, casillasAdyacentes: JugadasPosibles, jugada: Casilla):
    casillasAdyacentes.remove(jugada)
    casillasAdyacentes.update(casillasAdyacentesVacias(tablero, jugada))


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


def esInvisible(caracter: str) -> bool:
    return ord(caracter) <= ord(" ") # Todos los caracteres hasta el espacio son invisibles


# ###   saltarAlFinalDeLinea
#   Toma un archivo y lee caracteres hasta llegar al final de la linea o al final del archivo.
# Se asume que el descriptor del archivo ya paso la información de la linea actual por lo que los caracteres restantes
# deben ser invisibles (espacios, sangrías, etc), la función alza AssertionError si esto no se cumple.
def saltarAlFinalDeLinea(archivo) -> None:
    c = archivo.read(1)
    while c != "\n" and c != "":
        c = archivo.read(1)
        assert esInvisible(c) # Debería cumplirse siempre si el formato del archivo de entrada es el correcto


# ###   leerArchivoEntrada
#   Lee el archivo de entrada. Toma la dirección en la que se puede encontrar el archivo de entrada y lee el tablero sobre el que se va a jugar 
# y el color del jugador que empieza. Devuelve una tupla con el tablero resultante y el color del jugador que va a arrancar. La función alza
# AssertionError si el formato del archivo de entrada no es el correcto.
def leerArchivoEntrada(direccionEntrada: str) -> Tuple[Tablero, ColorFicha]:
    tablero: Tablero = dict()
    archivoEntrada = open(direccionEntrada, "r")
    for fila in range(1, 9):
        for columna in range(1, 9):
            casilla = archivoEntrada.read(1)
            assert len(casilla) == 1 # Verifica no haber llegado al final del archivo antes de tiempo
            if not casilla == CASILLA_VACIA:
                # Solo las casillas no vacías se agregan al tablero
                assertColorFicha(casilla)
                tablero[(fila, columna)] = casilla
        saltarAlFinalDeLinea(archivoEntrada)
    fichaInicial = archivoEntrada.read(1)
    assertColorFicha(fichaInicial)
    archivoEntrada.close()
    return tablero, fichaInicial


# ###   printTablero
#   Imprime el tablero. Toma un tablero y lo imprime a la consola, añadiendo un borde que facilita la identificación 
# de las coordenadas de cada casilla.
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


# ###   casillasAdyacentesVacías
#   Devuelve un conjunto con las casillas vacías adyacentes (ortogonal y diagonalmente) a la casilla dada. Toma el tablero y una casilla.
def casillasAdyacentesVacias(tablero: Tablero, casilla: Casilla) -> set[Casilla]:
    fila, columna = casilla
    adyacentes = set()
    for movFila in [-1, 0, 1]:
        for movCol in [-1, 0, 1]:
            casillaAdyacente = (fila + movFila, columna + movCol)
            if casillaValida(casillaAdyacente) and casillaAdyacente not in tablero:
                # Agrega casillas vacías adyacentes a casillas con fichas
                adyacentes.add(casillaAdyacente)
    return adyacentes


# ###   encontrarCasillasAdyacentes
#   Toma un tablero y devuelve el conjunto de todas las casillas vacías que están adyacentes a una casilla no vacía. Aunque no todas las casillas de este conjunto
# son jugadas validas, si se cumple que todas las jugadas validas pertenecen al conjunto. Esto facilita eliminar candidatos de jugadas.
def encontrarCasillasAdyacentes(tablero: Tablero) -> JugadasPosibles:
    adyacentes: JugadasPosibles = set()
    for fila in range(1, 9):
        for columna in range(1, 9):
            if (fila, columna) in tablero:
                adyacentes.update(casillasAdyacentesVacias(tablero, (fila, columna)))
    return adyacentes


# ###   hayFichaTrasDirección
#   Revisa una dirección para ver si eventualmente aparecen una ficha del color buscado. Toma el tablero, la primer casilla que aparece en la dirección buscada, 
# una dirección hacia la que se dirige y el color de la ficha buscada.
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


# ###   capturarDirección
#   Captura las fichas hacia una dirección dada, frenando cuando se encuentra una ficha del color buscado. Toma el tablero, la primer casilla que aparece en la , 
# dirección buscada una dirección hacia la que se dirige y el color de la ficha buscada. Adicionalmente, el parámetro opcional modificarTablero determina si las
# fichas se capturan (son remplazadas por fichas del color opuesto) o si solo cuenta las fichas que ese movimiento capturaría.
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
#   Intenta hacer una jugada. Toma el tablero, la casilla donde se hará la jugada y el color de la ficha a poner y devuelve
# la cantidad de fichas capturadas (0 si no pudo colocar la ficha). Adicionalmente, el parámetro modificarTablero determina
#  si la función hace cambios en el tablero o si simplemente determina si la jugada se podría hacer.
def hacerJugada(tablero: Tablero, casilla: Casilla, color: ColorFicha, modificarTablero = True) -> int:
    if casilla in tablero or not casillaValida(casilla):
        return 0
    fila, columna = casilla
    fichasCapturadas = 0
    for movFila in [-1, 0, 1]:
        for movCol in [-1, 0, 1]:
            primerCasillaEnDireccion = (fila + movFila, columna + movCol)
            hayFichaInicioDireccion = primerCasillaEnDireccion in tablero and tablero[primerCasillaEnDireccion] == colorOpuesto(color) # Siempre sera falso cuando movFila = movCol = 0
            if hayFichaInicioDireccion and hayFichaTrasDireccion(tablero, primerCasillaEnDireccion, (movFila, movCol), color):
                # Se puede capturar hacia esta dirección
                fichasCapturadas += capturarDireccion(tablero, primerCasillaEnDireccion, (movFila, movCol), color, modificarTablero)
    if fichasCapturadas > 0:
        if modificarTablero: tablero[casilla] = color
        fichasCapturadas += 1
    return fichasCapturadas


# ###   turnoBot
#   Hace una jugada como un bot de nivel dado. Toma el tablero, un conjunto con todas las casillas vacías adyacentes a una ficha, el color del bot y el nivel de este.
# El nivel del bot solo puede tener valores 0 o 1. El conjunto pasado por casillasAdyacentes se modifica para reflejar el nuevo estado del tablero con la ficha. Devuelve
# verdadero si pudo poner una ficha y falso si tuvo que pasar.
def turnoBot(tablero: Tablero, casillasAdyacentes: JugadasPosibles, colorBot: ColorFicha, nivel: int, anteriorPaso: bool) -> bool:
    if nivel == 0:
        return turnoBot0(tablero, casillasAdyacentes, colorBot, anteriorPaso)
    else:
        return turnoBot1(tablero, casillasAdyacentes, colorBot, anteriorPaso)


# ###   hayJugada
#    Determina si el jugador de un color determinado tiene una jugada disponible. Toma el tablero, un conjunto con las casillas vacías adyacentes a una ficha y el color
# del jugador a evaluar.
def hayJugada(tablero: Tablero, casillasAdyacentes: JugadasPosibles, color: ColorFicha) -> bool:
    encontroJugadaValida = False
    for casilla in casillasAdyacentes:
        encontroJugadaValida = encontroJugadaValida or jugadaEsValida(tablero, casilla, color)
    return encontroJugadaValida


# ###   turnoJugador
#   Se encarga del turno del jugador. Toma el tablero, un conjunto con las casillas vacías adyacentes a una ficha, el color del jugador 
# y si el jugador anterior pasó. Devuelve verdadero si el jugador pudo poner una ficha y falso si tuvo que pasar.
def turnoJugador(tablero: Tablero, casillasAdyacentes: JugadasPosibles, colorJugador: ColorFicha, anteriorPaso: bool) -> bool:
    if not hayJugada(tablero, casillasAdyacentes, colorJugador):
        return False

    jugadaEsValida = False
    jugada = (0, 0)
    while not jugadaEsValida:
        jugadaInput = input(f"Ingrese la casilla en la que desea poner su próxima ficha {" (La maquina pasó)" if anteriorPaso else ""}:")
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
#   Elige una jugada aleatoria entre las posibles y la hace. Toma el tablero, un conjunto con las casillas vacías adyacentes a una ficha,
# el color del bot y si el jugador anterior pasó. Devuelve verdadero si encontró una jugada valida para hacer y falso si tuvo que pasar.
def turnoBot0(tablero: Tablero, casillasAdyacentes: JugadasPosibles, colorBot: ColorFicha, anteriorPaso: bool) -> bool:
    hizoJugada = False
    jugada = (0, 0)
    comoLista = list(casillasAdyacentes)
    random.shuffle(comoLista) # Desordena la lista que contiene los elementos del set de casillas adyacentes
    for casilla in comoLista:
        if not hizoJugada:
            capturas = hacerJugada(tablero, casilla, colorBot)
            hizoJugada = not capturas == 0
            jugada = casilla
    if hizoJugada:
        print(f"Turno de la maquina: {casilla2str(jugada)} {"(El jugador pasó)" if anteriorPaso else ""}")
        actualizarCasillasAdyacentes(tablero, casillasAdyacentes, jugada)
    return hizoJugada

# ### turnoBot1
#   Elige la jugada que mas capturas logre entre las posibles y la hace. Toma el tablero, un conjunto con las casillas vacías adyacentes
# a una ficha, el color del bot y si el jugador anterior pasó. Devuelve verdadero si encontró una jugada valida y falso si tuvo que pasar.
def turnoBot1(tablero: Tablero, casillasAdyacentes: JugadasPosibles, colorBot: ColorFicha, anteriorPaso: bool):
    maxCapturas = 0
    mejorJugada = (0, 0)
    for casilla in casillasAdyacentes:
        capturas = hacerJugada(tablero, casilla, colorBot, modificarTablero=False)
        if capturas > maxCapturas:
            maxCapturas = capturas
            mejorJugada = casilla
    if not maxCapturas == 0:
        print(f"Turno de la maquina: {casilla2str(mejorJugada)} {"(El jugador pasó)" if anteriorPaso else ""}")
        hacerJugada(tablero, mejorJugada, colorBot)
        actualizarCasillasAdyacentes(tablero, casillasAdyacentes, mejorJugada)
    return not maxCapturas == 0


# ### determinarGanador
#   Determina que color tiene mas fichas en el tablero al final del juego. Toma un tablero terminado y devuelve FICHA_BLANCA en caso de que haya mas fichas blancas, FICHA_NEGRA 
# si hay mas fichas negras y otra cosa (específicamente CASILLA_VACÍA) en caso de empate.
def determinarGanador(tablero: Tablero) -> str:
    blanco = 0  
    negro = 0
    for _, ficha in tablero.items():
        if ficha == FICHA_BLANCA:
            blanco += 1
        elif ficha == FICHA_NEGRA:
            negro += 1
    if blanco > negro:
        return FICHA_BLANCA
    elif negro > blanco:
        return FICHA_NEGRA
    else:
        return CASILLA_VACIA


def jugar(tablero: Tablero, colorJugando: ColorFicha, colorJugador: ColorFicha, nivelBot: int) -> None:
    colorBot = colorOpuesto(colorJugador)
    printTablero(tablero)
    casillasAdyacentes = encontrarCasillasAdyacentes(tablero)
    pasosSeguidos = 0  # Contador de cuantos jugadores seguidos pasaron (si llega a 2 significa que el juego termino) 
    while pasosSeguidos < 2:
        pudoJugar = False
        if colorJugando == colorBot:
            pudoJugar = turnoBot(tablero, casillasAdyacentes, colorBot, nivelBot, pasosSeguidos > 0)
        else:
            pudoJugar = turnoJugador(tablero, casillasAdyacentes, colorJugador, pasosSeguidos > 0)

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


def imprimirErrorUso():
    print("La aplicación se debe ejecutar de la siguiente forma:")
    print("python Main.py [direccionArchivoEntrada] [colorJugador] [nivelBot]")
    print("direccionArchivoEntrada: La dirección del archivo de entrada que contiene el tablero inicial y el color del jugador que empieza")
    print("colorJugador: El color con el que va a jugar el jugador. Puede ser \"B\" o \"N\"")
    print("nivelBot: El nivel de dificultad del bot. Puede ser 0 o 1")


def imprimirErrorArchivoEntrada():
    print("El formato del archivo de entrada debe ser erl siguiente:")
    print("•    si la casilla está vacía se escribe una X")
    print("•    en la casilla hay una ficha blanca se escribe B")
    print("•    en la casilla hay una ficha negra se escribe N")
    print("•    se escribe un carácter al lado del otro y cada fila del tablero es una línea (no hay líneas vacías entre ellas)")
    print("•    al final del tablero se escribe el color del jugador que empieza (B o N)")


if __name__ == '__main__':
    try:
        assertColorFicha(sys.argv[2])
        colorJugador = sys.argv[2]
        nivelBot = int(sys.argv[3]) # Chequea el nivel del bot
        assert nivelBot == 0 or nivelBot == 1
        tablero, colorJugando = leerArchivoEntrada(sys.argv[1]) # Abre el archivo de entrada
    except (ValueError, IndexError):
        imprimirErrorUso()
    except AssertionError:
        imprimirErrorUso()
        imprimirErrorArchivoEntrada()
    except FileNotFoundError:
        imprimirErrorUso()
        print(f"No se encontró el archivo {sys.argv[1]}")
    else:
        jugar(tablero, colorJugando, colorJugador, nivelBot)
    
    
    
    
    
    
    
    