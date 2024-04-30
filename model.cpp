/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include "raylib.h"
#include <array>

#include "model.h"

void initModel(GameModel &model)
{
    model.gameOver = true;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
}

void startModel(GameModel &model)
{
    model.gameOver = false;

    model.currentPlayer = PLAYER_BLACK;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;
    model.turnTimer = GetTime();

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2 - 1] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2] = PIECE_BLACK;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2 - 1] = PIECE_BLACK;
}

Player getCurrentPlayer(GameModel &model)
{
    return model.currentPlayer;
}

int getScore(GameModel &model, Player player)
{
    int score = 0;

    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (((model.board[y][x] == PIECE_WHITE) &&
                 (player == PLAYER_WHITE)) ||
                ((model.board[y][x] == PIECE_BLACK) &&
                 (player == PLAYER_BLACK)))
                score++;
        }

    return score;
}

double getTimer(GameModel &model, Player player)
{
    double turnTime = 0;

    if (!model.gameOver && (player == model.currentPlayer))
        turnTime = GetTime() - model.turnTimer;

    return model.playerTime[player] + turnTime;
}

Piece getBoardPiece(GameModel &model, Square square)
{
    return model.board[square.y][square.x];
}

void setBoardPiece(GameModel &model, Square square, Piece piece)
{
    model.board[square.y][square.x] = piece;
}

bool isSquareValid(Square square)
{
    return (square.x >= 0) &&
           (square.x < BOARD_SIZE) &&
           (square.y >= 0) &&
           (square.y < BOARD_SIZE);
}

void getValidMoves(GameModel &model, Moves &validMoves)
{
    Piece oppPiece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
        ? PIECE_BLACK
        : PIECE_WHITE;
    Piece selfPiece = 
        (getCurrentPlayer(model) == PLAYER_WHITE)
        ? PIECE_WHITE
        : PIECE_BLACK;

   
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            Square move = { x, y }, auxMove = move;

            std::array<Square, 8> auxArray = {
                {{ auxMove.x, ++auxMove.y }, { ++auxMove.x, auxMove.y },
                { auxMove.x, --auxMove.y }, { auxMove.x, --auxMove.y }, 
                { --auxMove.x, auxMove.y }, { --auxMove.x, auxMove.y }, 
                { auxMove.x, --auxMove.y }, { auxMove.x, --auxMove.y }}
            };

            // +++ TEST
            // Lists all empty squares...
            if (getBoardPiece(model, move) == PIECE_EMPTY)
            {
                bool flag = false;

                for(auto i = auxArray.begin(); i != auxArray.end() && flag == false; i++)
                    if (isSquareValid(*i) && getBoardPiece(model, *i) == oppPiece)
                    {
                        //horizontal y diagonal "\"
                        for (int i = 0, j = BOARD_SIZE - y - 1 ; i < BOARD_SIZE ; i++, j++)
                            if (((isSquareValid({i, y}) && getBoardPiece(model, {i , y}) == selfPiece) || (isSquareValid({i, j}) && getBoardPiece(model, {i, j}))) && x != i)
                            {
                                validMoves.push_back(move);
                                flag = true;
                            }
                        

                        //vertical y diagonal "/"
                        for(int j = 0, i = BOARD_SIZE - x - 1 ; j < BOARD_SIZE && flag == false ; j++, i--)
                            if (((isSquareValid({ x, j }) &&getBoardPiece(model, { x , j }) == selfPiece) || (isSquareValid({ i, j }) && getBoardPiece(model, { i, j })))&& j != y)
                            {
                                validMoves.push_back(move);
                                flag = true;
                            }
                    }
                
            }
            
            // --- TEST
        }
    
}

bool playMove(GameModel &model, Square move)
{
    // Set game piece
    Piece piece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
            ? PIECE_WHITE
            : PIECE_BLACK;

    setBoardPiece(model, move, piece);

    Piece pieceEnemy =
        (getCurrentPlayer(model) == PLAYER_WHITE)
        ? PIECE_BLACK
        : PIECE_WHITE;
    // To-do: your code goes here...
    
    Square auxSquare;
    int difference = 0;

    //horizontal derecha
    if (isSquareValid({move.x + 1, move.y}) && getBoardPiece(model, {move.x + 1, move.y}) == pieceEnemy)
    {
        auxSquare = {0, 0};
        for(int i = BOARD_SIZE - 1 ; i != move.x + 1 ; i--)
            if (isSquareValid({i , move.y }) && getBoardPiece(model, { i, move.y }) == piece && isSquareValid({ i - 1, move.y }) && getBoardPiece(model, { i - 1, move.y }) == pieceEnemy)
                auxSquare = { i , move.y };
            else if (isSquareValid({i, move.y }) && getBoardPiece(model, { i, move.y }) == piece && isSquareValid({ i - 1, move.y }) && getBoardPiece(model, { i - 1, move.y }) == PIECE_EMPTY)
                auxSquare = move;

        if (auxSquare.x > move.x) 
        {
            difference = auxSquare.x - move.x;
            
            for (int i = 1; i < difference; i++)
                setBoardPiece(model, { move.x + i, move.y }, piece);
        }
    }

    //horizontal izquierda
    if (isSquareValid({ move.x - 1, move.y }) && getBoardPiece(model, { move.x - 1, move.y }) == pieceEnemy)
    {
        auxSquare = { BOARD_SIZE-1, BOARD_SIZE-1};
        for (int i = 0; i != move.x - 1; i++)
            if (isSquareValid({i, move.y }) && getBoardPiece(model, { i, move.y }) == piece && isSquareValid({ i + 1, move.y }) && getBoardPiece(model, { i + 1, move.y }) == pieceEnemy)
                auxSquare = { i , move.y };
            else if (isSquareValid({ i, move.y }) && getBoardPiece(model, { i, move.y }) == piece && isSquareValid({ i + 1, move.y }) && getBoardPiece(model, { i + 1, move.y }) == PIECE_EMPTY)
                auxSquare = move;

        if (auxSquare.x < move.x)
        {
            difference = move.x - auxSquare.x;

            for (int i = difference; i > 0; i--)
                setBoardPiece(model, { move.x - i, move.y }, piece);
        }
    }

    //vertical abajo
    if (getBoardPiece(model, { move.x, move.y + 1 }) == pieceEnemy)
    {
        auxSquare = { 0, 0 };
        for (int j = BOARD_SIZE - 1; j != move.y + 1; j--)
            if ((isSquareValid({move.x, j }) && getBoardPiece(model, { move.x , j })) == piece && (isSquareValid({move.x, j - 1}) && getBoardPiece(model, { move.x, j - 1 }) == pieceEnemy))
                auxSquare = { move.x , j };
            else if ((isSquareValid({move.x, j }) && getBoardPiece(model, { move.x, j }) == piece) && (isSquareValid({move.x, j }) && getBoardPiece(model, { move.x, j - 1 }) == PIECE_EMPTY))
                auxSquare = move;

        if (auxSquare.y > move.y)
        {
            difference = auxSquare.y - move.y;

            for (int  j= 1; j < difference; j++)
                setBoardPiece(model, { move.x, move.y + j}, piece);
        }
    }

    //vertical arriba
    if (isSquareValid({ move.x, move.y - 1 }) && getBoardPiece(model, { move.x, move.y - 1 }) == pieceEnemy)
    {
        auxSquare = { BOARD_SIZE-1, BOARD_SIZE-1 };
        for (int j = 0; j != move.y - 1; j++)
            if (isSquareValid({ move.x, j }) &&  getBoardPiece(model, { move.x, j }) == piece && isSquareValid({ move.x, j + 1 }) && getBoardPiece(model, { move.x, j + 1 }) == pieceEnemy)
                auxSquare = { move.x , j };
            else if (isSquareValid({ move.x, j }) && getBoardPiece(model, { move.x, j }) == piece && isSquareValid({ move.x, j + 1}) && getBoardPiece(model, { move.x, j + 1 }) == PIECE_EMPTY)
                auxSquare = move;

        if (auxSquare.y < move.y)
        {
            difference = move.y - auxSquare.y;

            for (int j = difference; j > 0; j--)
                setBoardPiece(model, { move.x , move.y - j }, piece);
        }
    }

    

    // Update timer
    double currentTime = GetTime();
    model.playerTime[model.currentPlayer] += currentTime - model.turnTimer;
    model.turnTimer = currentTime;

    // Swap player
    model.currentPlayer =
        (model.currentPlayer == PLAYER_WHITE)
            ? PLAYER_BLACK
            : PLAYER_WHITE;

    // Game over?
    Moves validMoves;
    getValidMoves(model, validMoves);

    if (validMoves.size() == 0)
        model.gameOver = true;

    return true;
}
