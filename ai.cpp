/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <cstdlib>

#include "ai.h"
#include "controller.h"

void populateTree(Tree* father, Moves fatherValidMoves, int iterator)
{
    static int counter = 0;
    father->sons.push_back(new Tree());
    Tree* thisNode = father->sons[iterator];
    thisNode->move = fatherValidMoves[iterator];
    thisNode->sim = father->sim;
    playMove(thisNode->sim, thisNode->move);

    Moves thisNodeValidMoves;
    getValidMoves(thisNode->sim, thisNodeValidMoves);
    Player aiPlayer =
                (thisNode->sim.humanPlayer == PLAYER_WHITE)
                ? PLAYER_BLACK
                : PLAYER_WHITE;
    
    if (thisNodeValidMoves.size() == 0 || counter == COTA_NIVELES)
    {
       thisNode->value = getScore(thisNode->sim, aiPlayer) - getScore(thisNode->sim, thisNode->sim.humanPlayer);
    }
    else
    {
        counter++;
        for (int i = 0; i < thisNodeValidMoves.size(); i++)
            populateTree(thisNode, thisNodeValidMoves, i);
    }
}

int functionMinMax(Tree * father) 
{
    Player aiPlayer =
        (father->sim.humanPlayer == PLAYER_WHITE)
        ? PLAYER_BLACK
        : PLAYER_WHITE;
    
    if (father->sons.size() == 0)
    {
        return father->value;
    }
    else
    {
        int result = 0;
        int minMaxVal = (father->sim.currentPlayer == aiPlayer) ? -64 : 64; 
        for (auto i = 0; father->sons.size() > i; i++)
        {
            int childValue = functionMinMax(father->sons[i]);

            if (father->sim.currentPlayer == aiPlayer) //max
            {
                if(minMaxVal < childValue)
                    minMaxVal = childValue;
            }
            else //min
            {
                if(minMaxVal > childValue)
                    minMaxVal = childValue;
            }
        }
        return minMaxVal;
    }
}

void freeTrees(Tree * node)
{
    if (node->sons.size() == 0) 
    {
        return;
    }
    for(auto i = 0 ; node->sons.size() > i ; i++)
    {
        freeTrees(node->sons[i]);
    }

    delete node;

}

Square getBestMove(GameModel &model)
{
    Piece oppPiece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
        ? PIECE_BLACK
        : PIECE_WHITE;
    Piece selfPiece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
        ? PIECE_WHITE
        : PIECE_BLACK;

    Moves validMoves;

    Tree root;
    root.sim = model;

    getValidMoves(root.sim, validMoves);

    for(auto i = 0 ; i < validMoves.size() ; i++)
        populateTree(&root, validMoves, i);

    int minMax = -64;
    Square bestMove = {0, 0};

    for (auto i = 0; i < root.sons.size(); i++)
    {
        int sonVal= functionMinMax(root.sons[i]);
        if (minMax < sonVal)
        {
            minMax = sonVal;
            bestMove = root.sons[i]->move;
        }
    }

    for (auto i = 0; i < root.sons.size(); i++)
        freeTrees(root.sons[i]);
    
    return bestMove;
    
}