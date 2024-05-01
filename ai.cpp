/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <cstdlib>

#include "ai.h"
#include "controller.h"

Square getBestMove(GameModel &model)
{
    

    /*Piece oppPiece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
        ? PIECE_BLACK
        : PIECE_WHITE;
    Piece selfPiece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
        ? PIECE_WHITE
        : PIECE_BLACK;

    Moves validMoves;

    Tree root;
    root.move = model;

    getValidMoves(root.move, validMoves);*/
    

    // +++ TEST
    // Returns a random valid move...
    Moves validMoves;
    getValidMoves(model, validMoves);

    int index = rand() % validMoves.size();
    return validMoves[index];
    // --- TEST
    
    
}

//void populateTree (Tree * arbol, GameModel * model, Moves validMoves)
//{
//    std::array<vector<Tree>, validMoves.size()> sons;
//    if (validMoves.size() != 0)
//    {
//        for (auto i = validMoves.size() - 1; i >= 0; i--)
//        {
//
//        }
//
//    }
//
//}
