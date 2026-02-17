import Main
import pytest


def saltarAlFinalDeLinea(archivo) -> None:
    c = archivo.read(1)
    while c != "\n" and c != "":
        c = archivo.read(1)


def leerTablero(direccion: str) -> Main.Tablero:
    fila = 1
    columna = 1
    tablero = dict()
    archivo = open(direccion, "r")
    while fila <= 8:
        while columna <= 8:
            c = archivo.read(1)
            if c != "X":
                tablero[(fila, columna)] = c
            columna += 1
        fila += 1
        columna = 1
        saltarAlFinalDeLinea(archivo)
    archivo.close()
    Main.printTablero(tablero) # ELIMINAR ESTA LINEA
    return tablero 


def test_casillasAdyacentesVacias() -> None:
    tab = leerTablero("tests/TableroMedias.txt")
    assert Main.casillasAdyacentesVacias(tab, (3, 4)) == {(2, 3), (2, 4)}
    assert Main.casillasAdyacentesVacias(tab, (5, 1)) == {(4, 1), (5, 2), (6, 1), (6, 2)}


def test_encontrarCasillasAdyacentes() -> None:
    tabMedias = leerTablero("tests/TableroPaso.txt")
    tabCompleto = leerTablero("tests/TableroCompleto.txt")
    tabIncompletoSinJugadas = leerTablero("tests/TableroSinJugadas.txt")
    assert Main.encontrarCasillasAdyacentes(tabMedias) == {(2, 8), (3, 8), (4, 8), (5, 8), (6, 8), (2, 7), (2, 6), (3, 6), (1, 5), (2, 5)}
    assert Main.encontrarCasillasAdyacentes(tabCompleto) == set() # El tablero está lleno, no hay mas jugadas posibles
    assert Main.encontrarCasillasAdyacentes(tabIncompletoSinJugadas) == {(8, 8)} # A pesar de no haber jugadas, el tablero no está lleno.


def test_fila2indice() -> None:
    assert Main.fila2indice("1") == 1
    assert Main.fila2indice("8") == 8
    assert Main.fila2indice("9") == -1
    assert Main.fila2indice("B") == -1


def test_columna2indice() -> None:
    assert Main.columna2indice("A") == 1
    assert Main.columna2indice("H") == 8
    assert Main.columna2indice("I") == -1
    assert Main.columna2indice("1") == -1


def test_hayFichaTrasDireccion() -> None:
    tablero = leerTablero("tests/TableroMedias.txt");
    assert Main.hayFichaTrasDireccion(tablero, (5, 3), (0, 1), "N") == True # Encuentra casilla negra
    assert Main.hayFichaTrasDireccion(tablero, (5, 1), (0, -1), "N") == False # Llega al borde del tablero


def test_CapturarDireccion() -> None:
    tablero = leerTablero("tests/TableroCaptura.txt")
    esperado = leerTablero("tests/TableroEsperadoCapturarDireccion.txt")
    Main.capturarDireccion(tablero, (5, 5), (1, 0), "B") # Captura hacia abajo
    assert tablero == esperado


def test_hacerJugada() -> None:
    tab = leerTablero("tests/TableroCaptura.txt")
    inicial = leerTablero("tests/TableroCaptura.txt")
    # Problemas de prerrequisitos 
    assert Main.hacerJugada(tab, (9, 4), 'N') == 0 # Casilla fuera del tablero
    assert Main.hacerJugada(tab, (4, 2), 'N') == 0 # Casilla ocupada

    # No puede capturar
    assert Main.hacerJugada(tab, (8, 8), 'B') == 0 # No encuentra ningún vecino con su color opuesto
    assert Main.hacerJugada(tab, (6, 4), 'B') == 0 # Adyacente de otro color pero no captura
    assert tab == inicial # Como la jugada es invalida el tablero no se modifica

    # Jugada válida
    esperado = leerTablero("tests/TableroEsperadoColocarFicha.txt")
    assert Main.hacerJugada(tab, (4, 5), 'B', False) != 0 # Revisa que es válida la jugada
    assert tab == inicial # El tablero no se modifica
    assert Main.hacerJugada(tab, (4, 5), 'B') == 8 # Hace la jugada
    assert tab == esperado # El tablero se modifica


def test_hayJugada() -> None:
    tab1 = leerTablero("tests/TableroInicial.txt")
    tab2 = leerTablero("tests/TableroCompleto.txt")
    tab3 = leerTablero("tests/TableroPaso.txt")
    adyacentesTab1 = Main.encontrarCasillasAdyacentes(tab1)
    adyacentesTab2 = Main.encontrarCasillasAdyacentes(tab2) 
    adyacentesTab3 = Main.encontrarCasillasAdyacentes(tab3)
    # Tienen jugada ambos jugadores
    assert Main.hayJugada(tab1, adyacentesTab1, 'B')
    assert Main.hayJugada(tab1, adyacentesTab1, 'N')
    # Nadie tiene jugada
    assert not Main.hayJugada(tab2, adyacentesTab2, 'B')
    assert not Main.hayJugada(tab2, adyacentesTab2, 'N')
    # Uno tiene y el otro no
    assert not Main.hayJugada(tab3, adyacentesTab3, 'B')
    assert Main.hayJugada(tab3, adyacentesTab3, 'N')


def test_turnoBot0() -> None:
    tab1 = leerTablero("tests/TableroPaso.txt")
    adyacentes = Main.encontrarCasillasAdyacentes(tab1)
    tab2 = leerTablero("tests/TableroMedias.txt")
    adyacentes2 = Main.encontrarCasillasAdyacentes(tab2)
    referencia = leerTablero("tests/TableroMedias.txt")
    assert Main.turnoBot0(tab1, adyacentes, 'B', False) == False # El bot no tiene jugada
    assert Main.turnoBot0(tab2, adyacentes2, 'N', False) == True # El bot tiene jugada
    assert tab2 != referencia # El tablero se modificó


def test_turnoBot1() -> None:
    tab1 = leerTablero("tests/TableroPaso.txt")
    adyacentes = Main.encontrarCasillasAdyacentes(tab1)
    tab2 = leerTablero("tests/TableroBot1.txt")
    adyacentes2 = Main.encontrarCasillasAdyacentes(tab2)
    esperadoTab2 = leerTablero("tests/TableroEsperadoBot1.txt")
    assert Main.turnoBot1(tab1, adyacentes, 'B', False) == False # El bot no tiene jugada
    assert Main.turnoBot1(tab2, adyacentes2, 'N', False) == True # El bot tiene jugada
    assert tab2 == esperadoTab2 # La función eligió la mejor jugada.


def test_DarGanador() -> None:
    tab1 = leerTablero("tests/TableroCompleto.txt")
    tab2 = leerTablero("tests/TableroEmpate.txt")
    assert Main.determinarGanador(tab1) == 'B' # Ganan las blancas
    ganadorTab2 = Main.determinarGanador(tab2)
    assert ganadorTab2 != 'N' and ganadorTab2 != 'B' # Empate, no gana nadie

