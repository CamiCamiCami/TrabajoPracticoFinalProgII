/* Programa General */

// 1) Procesar el header.
// 1.1) Encontrar jugadores y asociarlos con su color
// 1.1.1) Si los colores son validos (B ó N) y los nombres no son iguales -> 1.2 sino -> 5
// 1.2) Determinar el color que empieza
// 1.2.1) Si es un color valido (B ó N) -> 2 sino -> 5
// 2) Construir tablero
// 2.0) Asignar memoria para un tablero e inicializarlo
// 2.1) Procesar lineas
// 2.1.1) Si la linea está vacia -> 2.1.1.1 sino -> 2.1.2
// 2.1.1.1) Si el jugador no tiene movimientos disponibles -> 2.1.5 sino -> INV 
// 2.1.2) Si la linea tiene formato correcto -> 2.1.2 sino -> 5
// 2.1.3) Si la jugada es valida -> 2.1.3 sino -> INV
// 2.1.4) Aplicar la jugada en el tablero
// 2.1.5) Cambiar de jugador
// 2.2) Repetir 2.1 hasta que no queden lineas
// 2.3) Si el tablero esta completo o ninguno puede hacer un movimiento -> 3 sino -> 4
// 3) Dar por ganandor al jugador con mas fichas o daclarar empate de tener igual cantidad -> 7
// 4) Guardar progreso
// 4.1) Escribir tablero a archivo
// 4.2) Escribir jugador siguiente -> 7
// 5) Imprimir guia de formato -> 7
// 6) Manejar movimiento invalido
// 6.1) Imprimir Tablero
// 6.2) Imprimir jugador actual -> 7
// 7) Finalizar
// 7.1) Liberar el tablero
// 7.2) Devolver 0 si salio por 3 o 4 y 1 si salio por 5 o 6.


/* Colocar una pieza */

// Toma: tablero, posicion, color
// 1) Si la posicion esta en el tablero y desocupada -> 2 sino -> 8
// 2) Chequear direcciones
// 2.1) Si la ficha pertenece al tablero -> 2.2 sino 2.6
// 2.2) Si la ficha es del color opuesto -> 2.3 sino 2.4
// 2.3) Avanzar una ficha -> 2.1
// 2.4) Si la ficha es del mismo color -> 2.5 sino 2.6
// 2.5) Marcar la direccion como valida
// 2.6) Marcar la direccion como invalida
// 3) Repetir para todas las direcciones
// 4) Si hay al menos una direccion valida -> 5 sino -> 8
// 5) Marcar direcciones
// 5.1) Si la ficha es del color opuesto -> 5.2 sino 6
// 5.2) Cambiar el color de la ficha
// 5.3) Avanzar una posicion -> 5.1
// 6) Repetir para todas las direcciones validas
// 7) Devolver 1
// 8) Devolver 0



