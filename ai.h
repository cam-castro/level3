/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#ifndef AI_H
#define AI_H

#include "model.h"

#define COTA_NIVELES 2500

class Tree {
public:
	Square move; // posición del movimiento hecho 
	GameModel sim; // simulación del movimiento en el tablero correspondiente
	int value; // diferencia entre las fichas de la ia y las fichas del humano 
	std::vector<Tree *> sons; // nodos hijos
};

/**
 * @brief Returns the best move for a certain position.
 *
 * @return The best move.
 */
Square getBestMove(GameModel &model);

#endif
