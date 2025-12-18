from typing import Tuple


FICHA_BLANCA = "B"
FICHA_NEGRA = "N"


### Diseño de datos
Casilla = Tuple[str, str] # Ambos str son siempre de largo 1. El primer componente estara entre "A" y "H". El segundo componente estara entre "1" y "8".
Ficha = str # Sera siempre FICHA_BLANCA o FICHA_NEGRA
Tablero = dict[Casilla, Ficha]
JugadasPosibles = set[Casilla]


### Programa General 
# 0) Procesar argumentos de entrada
# 1) Leer archivo de C
# 1.0) Crear el diccionar
# 1.1) Leer linea
# 1.1.1) Si el caracter es 'B' o 'N' -> 1.1.2 sino -> 1.1.3
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