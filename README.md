# EDAversi

## Integrantes del grupo y contribución al trabajo de cada integrante

* Camila Agustina Castro: Desarrollo de las funciones playMove, getValidMoves y las correspondientes al archivo ai.cpp con sus funciones auxiliares correspondientes.
* Bautista Manuel Peri: Desarrollo de las funciones playMove, getValidMoves y las correspondientes al archivo ai.cpp con sus funciones auxiliares correspondientes.
* Cristobal Kramer Rodrigo: Desarrollo de las funciones playMove, getValidMoves y las correspondientes al archivo ai.cpp con sus funciones auxiliares correspondientes.

## Parte 1: Generación de movimientos válidos y algoritmo de jugada

Para validar las jugadas se probó el algoritmo (utilizando el algoritmo aleatorio de la función dada por la catedra) con una inteligencia artificial de un juego de Reversi de Internet (link en documentación adicional).

## Parte 2: Implementación del motor de IA

Buscamos que los nodos hoja (ya sea porque no habían más jugadas o porque se llegó al máximo de nodos) tengan el valor de la diferencia entre las fichas de la IA y el jugador humano. Cuando el nivel analizado es del jugador contrario, buscamos el valor más chico, pues asumimos que toma la mejor jugada el oponente (mayor cantidad de fichas del oponente, menor de la IA). Por el contrario, en aquellos niveles donde se analizan las jugadas de la IA, buscamos el número más alto (menor cantidad de fichas del oponente, mayor IA).

## Parte 3: Poda del árbol

Como el árbol sin podar posee n nodos, a mayor cantidad de nodos, tardará mucho más en resolver cuál es la mejor jugada a realizar. Por eso se toma la iniciativa de limitar el número n por uno más accesible y que no tenga una complejidad computacional muy alta.

## Documentación adicional

* Reversi utilizado como referencia: https://cardgames.io/reversi/