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

            // Lists all empty squares...
            if (getBoardPiece(model, move) == PIECE_EMPTY)
            {
                bool flag = true;

                // Vertical arriba
                if (isSquareValid({ move.x, move.y + 1 }) && getBoardPiece(model, { move.x, move.y + 1 }) == oppPiece && flag)
                    for (int i = move.y + 1 ; i < BOARD_SIZE && getBoardPiece(model, { move.x, i }) != PIECE_EMPTY; i++)
                        if (isSquareValid({ move.x , i }) && getBoardPiece(model, { move.x , i }) == selfPiece)
                        {
                            validMoves.push_back(move);
                            flag = false;
                        }

                // Diagonal arriba derecha
                if (isSquareValid({ move.x + 1, move.y + 1 }) && getBoardPiece(model, { move.x + 1, move.y + 1 }) == oppPiece && flag)
                    for (int i = 1; isSquareValid({ move.x + i , move.y + i }) && getBoardPiece(model, { move.x + i , move.y + i }) != PIECE_EMPTY; i++)
                        if (getBoardPiece(model, { move.x + i , move.y + i }) == selfPiece)
                        {
                            validMoves.push_back(move);
                            flag = false;
                        }

                // Horizontal derecha
                if (isSquareValid({ move.x + 1, move.y }) && getBoardPiece(model, {move.x + 1, move.y}) == oppPiece && flag)
                    for (int i = move.x + 1; i < BOARD_SIZE && getBoardPiece(model, {i, move.y}) != PIECE_EMPTY; i++)
                        if (isSquareValid({ i , move.y }) && getBoardPiece(model, { i, move.y }) == selfPiece)
                        {
                            validMoves.push_back(move);
                            flag = false;
                        }


                // Diagonal abajo derecha
                if (isSquareValid({ move.x + 1, move.y - 1 }) && getBoardPiece(model, {move.x + 1, move.y - 1}) == oppPiece && flag)
                    for (int i = 1; isSquareValid({ move.x + i , move.y - i }) && getBoardPiece(model, { move.x + i, move.y - i }) != PIECE_EMPTY; i++)
                        if (getBoardPiece(model, { move.x + i , move.y - i }) == selfPiece)
                        {
                            validMoves.push_back(move);
                            flag = false;
                        }

                // Vertical abajo
                if (isSquareValid({ move.x, move.y - 1 }) && getBoardPiece(model, { move.x, move.y - 1}) == oppPiece && flag)
                    for (int i = move.y - 1; i >= 0 && getBoardPiece(model, { move.x, i }) != PIECE_EMPTY; i--)
                        if (isSquareValid({ move.x , i }) && getBoardPiece(model, { move.x , i }) == selfPiece)
                        {
                            validMoves.push_back(move);
                            flag = false;
                        }

                // Diagonal abajo izquierda
                if (isSquareValid({ move.x - 1, move.y - 1 }) && getBoardPiece(model, { move.x - 1, move.y - 1 }) == oppPiece && flag)
                    for (int i = 1; isSquareValid({ move.x - i , move.y - i }) && getBoardPiece(model, { move.x - i , move.y - i }) != PIECE_EMPTY; i++)
                        if (getBoardPiece(model, { move.x - i , move.y - i }) == selfPiece)
                        {
                            validMoves.push_back(move);
                            flag = false;
                        }

                // Horizontal izquierda
                if (isSquareValid({ move.x - 1, move.y }) && getBoardPiece(model, { move.x - 1, move.y }) == oppPiece && flag)
                    for (int i = move.x - 1; i >= 0 && getBoardPiece(model, { i, move.y }) != PIECE_EMPTY; i--)
                        if (isSquareValid({ i , move.y }) && getBoardPiece(model, { i, move.y }) == selfPiece)
                        {
                            validMoves.push_back(move);
                            flag = false;
                        }

                // Diagonal arriba izquierda
                if (isSquareValid({ move.x - 1, move.y + 1 }) && getBoardPiece(model, auxMove) == oppPiece && flag)
                    for (int i = 1; isSquareValid({ move.x - i , move.y + i }) && getBoardPiece(model, { move.x - i , move.y + i }) != PIECE_EMPTY; i++)
                        if (getBoardPiece(model, { move.x - i , move.y + i }) == selfPiece)
                        {
                            validMoves.push_back(move);
                            flag = false;
                        }

            }

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

    // Horizontal derecha
    if (isSquareValid({ move.x + 1, move.y }) && getBoardPiece(model, { move.x + 1, move.y }) == pieceEnemy)
    {
        bool flag = true;
        for (int i = move.x + 1; i < BOARD_SIZE && flag && getBoardPiece(model, { i, move.y }) != PIECE_EMPTY; i++)
            if (isSquareValid({ i , move.y }) && getBoardPiece(model, { i, move.y }) == piece)
            {
                auxSquare = { i , move.y };
                flag = false;
            }

        if (!flag)
            for (int i = move.x; i <= auxSquare.x; i++)
                setBoardPiece(model, { i, move.y }, piece);
    }

    //horizontal izquierda
    if (isSquareValid({ move.x - 1, move.y }) && getBoardPiece(model, { move.x - 1, move.y }) == pieceEnemy)
    {
        bool flag = true;
        for (int i = move.x - 1; i >= 0 && flag && getBoardPiece(model, { i , move.y }) != PIECE_EMPTY ; i--)
            if( isSquareValid({ i , move.y }) && getBoardPiece(model, { i, move.y }) == piece )
            {
                auxSquare = { i , move.y };
                flag = false;
            }

        if (!flag)
            for (int i = move.x; i > auxSquare.x; i--)
                setBoardPiece(model, { i, move.y }, piece);

    }

    //vertical abajo
    if (isSquareValid({ move.x, move.y - 1 }) && getBoardPiece(model, { move.x , move.y - 1 }) == pieceEnemy)
    {
        bool flag = true;
        for (int j = move.y - 1 ; j >= 0 && flag && getBoardPiece(model, { j , move.y - 1 }) != PIECE_EMPTY ; j--)
            if( isSquareValid({ move.x , j }) && getBoardPiece(model, { move.x , j }) == piece )
            {
                auxSquare = { move.x , j };
                flag = false;
            }

        if (!flag)
            for (int  j = move.y; j > auxSquare.y; j--)
                setBoardPiece(model, { move.x, j }, piece);
    }

    // Vertical arriba
    if (isSquareValid({ move.x, move.y + 1 }) && getBoardPiece(model, { move.x, move.y + 1 }) == pieceEnemy)
    {
        bool flag = true;
        for (int j = move.y + 1; j < BOARD_SIZE && flag && getBoardPiece(model, { move.x , j}) != PIECE_EMPTY; j++)
            if (isSquareValid({ move.x , j }) && getBoardPiece(model, { move.x , j }) == piece)
            {
                auxSquare = { move.x , j };
                flag = false;
            }

        if (!flag)
            for (int j = move.y; j < auxSquare.y ; j++)
                setBoardPiece(model, { move.x, j }, piece);
        
    }

    // Diagonal arriba derecha
    if (isSquareValid({ move.x + 1, move.y + 1 }) && getBoardPiece(model, { move.x + 1, move.y + 1 }) == pieceEnemy)
    {
        bool flag = true;
        for (int i = 1; isSquareValid({ move.x + i , move.y + i }) && flag && getBoardPiece(model, { move.x + i , move.y + i }) != PIECE_EMPTY; i++)
            if (getBoardPiece(model, { move.x + i , move.y + i }) == piece)
            {
                auxSquare = { move.x + i , move.y + i };
                flag = false;
            }

        if (!flag)
            for (int j = 0; move.x + j < auxSquare.x; j++)
                setBoardPiece(model, { move.x + j, move.y + j }, piece);

    }

    // Diagonal arriba izquierda
    if (isSquareValid({ move.x - 1, move.y + 1 }) && getBoardPiece(model, { move.x + 1, move.y + 1 }) == pieceEnemy)
    {
        bool flag = true;
        for (int i = 1; isSquareValid({ move.x - i , move.y + i }) && flag && getBoardPiece(model, { move.x - i , move.y + i }) != PIECE_EMPTY; i++)
            if (getBoardPiece(model, { move.x - i , move.y + i }) == piece)
            {
                auxSquare = { move.x - i , move.y + i };
                flag = false;
            }

        if (!flag)
            for (int j = 0; move.x - j < auxSquare.x; j++)
                setBoardPiece(model, { move.x - j, move.y + j }, piece);
    }

    // Diagonal abajo derecha
    if (isSquareValid({ move.x + 1, move.y - 1 }) && getBoardPiece(model, { move.x + 1, move.y - 1 }) == pieceEnemy)
    {
        bool flag = true;
        for (int i = 1; isSquareValid({ move.x + i , move.y - i }) && flag && getBoardPiece(model, { move.x + i , move.y - i }) != PIECE_EMPTY; i++)
            if (getBoardPiece(model, { move.x + i , move.y - i }) == piece)
            {
                auxSquare = { move.x + i , move.y - i };
                flag = false;
            }

        if (!flag)
            for (int j = 0; move.x + j < auxSquare.x; j++)
                setBoardPiece(model, { move.x + j, move.y - j }, piece);

    }
    
    // Diagonal abajo izquierda
    if (isSquareValid({ move.x - 1, move.y - 1 }) && getBoardPiece(model, { move.x - 1, move.y - 1 }) == pieceEnemy)
    {
        bool flag = true;
        for (int i = 1; isSquareValid({ move.x - i , move.y - i }) && flag && getBoardPiece(model, { move.x - i , move.y - i }) != PIECE_EMPTY; i++)
            if (getBoardPiece(model, { move.x - i , move.y - i }) == piece)
            {
                auxSquare = { move.x - i , move.y - i };
                flag = false;
            }

        if (!flag)
            for (int j = 0; move.x - j < auxSquare.x; j++)
                setBoardPiece(model, { move.x - j, move.y - j }, piece);

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
