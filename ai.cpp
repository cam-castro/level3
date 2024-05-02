#include <cstdlib>
#include <iostream>

#include "ai.h"
#include "view.h"
#include "controller.h"

static int counter = 0;

void populateTree(Tree* father, Moves fatherValidMoves, int iterator)
{
    // Crea el enesimo nodo hijo
    father->sons.push_back(new Tree());
    Tree* thisNode = father->sons[iterator];
    thisNode->move = fatherValidMoves[iterator];

    // Juega el movimiento válido en la simulación
    thisNode->sim = father->sim;
    playMove(thisNode->sim, thisNode->move);

    // Busca todos los valid moves
    Moves thisNodeValidMoves;
    getValidMoves(thisNode->sim, thisNodeValidMoves);
    Player aiPlayer =
                (thisNode->sim.humanPlayer == PLAYER_WHITE)
                ? PLAYER_BLACK
                : PLAYER_WHITE;
    
    // ¿Es un nodo hoja, o el máximo de nodos?
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
    
    // ¿Es un nodo hoja?
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

            // Max
            if (father->sim.currentPlayer == aiPlayer)
            {
                if(minMaxVal < childValue)
                    minMaxVal = childValue;
            }
            else
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
    // ¿Tiene hijos para liberar?
    if (node->sons.size() == 0) 
    {
        counter = 0;
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

    // Genera el árbol
    Tree root;
    root.sim = model;

    getValidMoves(root.sim, validMoves);

    for(auto i = 0 ; i < validMoves.size() ; i++)
    {
        drawView(model);
        populateTree(&root, validMoves, i);
    }

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