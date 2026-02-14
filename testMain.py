import Main
import pytest

def generarTablero(comoString: str) -> Main.Tablero:
    linea = 1
    columna = 1
    tablero = dict()
    for c in comoString:
        if c == "\n":
            linea += 1
            columna = 1
        else:
            if c != "X":
                tablero[(linea, columna)] = c
            columna += 1
    return tablero 


def test_casillasAdyacentesVacias() -> None:
    tab = generarTablero(
"""XXXXXXXX
XXBNXXXX
XBNNBNNN
BNNBXNNX
XXBBNBNN
XXXXBBNX
XXXXXXXX
XXXXXXXX""")
    print(tab)
    assert Main.casillasAdyacentesVacias(tab, (2, 4)) == {(2, 5), (1, 3), (1, 4), (1, 5)}
    assert Main.casillasAdyacentesVacias(tab, (5, 7)) == {(6, 8), (4, 8)}


def test_encontrarCasillasAdyacentes() -> None:
    tab = generarTablero(
"""XXXXXXXX
XXXXXXXX
XBNNBNNN
BNNBXNNX
XXBBNBNN
XXXXXXXX
XXXXXXXX
XXXXXXXX""")
    assert Main.encontrarCasillasAdyacentes(tab) == {(2, 1), (2, 2), (2, 3), (2, 4), (2, 5), (2, 6), (2, 7), (2, 8), (3, 1), (4, 5), (4, 8), (5, 1), (5, 2), (6, 2), (6, 3), (6, 4), (6, 5), (6, 6), (6, 7), (6, 8)}



