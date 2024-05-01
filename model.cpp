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

bool analyseInDir(GameModel &model, Square square, Piece anPiece, const int dx, const int dy)
{
    if ( isSquareValid({ square.x + dx, square.y + dy}) && getBoardPiece(model, { square.x + dx, square.y + dy }) != PIECE_EMPTY 
        && getBoardPiece(model, { square.x + dx, square.y + dy }) != anPiece)
        for ( int i = dx, j = dy; isSquareValid({ square.x + i , square.y + j }) && 
                getBoardPiece(model, { square.x + dx, square.y + dy }) != PIECE_EMPTY ; i += dx, j+= dy )
            if (getBoardPiece(model, { square.x + i , square.y + j }) == anPiece)
                return true;
    
    return false;
}

void flipInDir(GameModel &model, Square square, Piece toFlip, int dx, int dy)
{
    for ( int i = dx, j = dy; isSquareValid({ square.x + i , square.y + j }) && 
        getBoardPiece(model, { square.x + i, square.y + j }) != toFlip; i += dx, j+= dy )
        setBoardPiece(model, { square.x + i, square.y + j }, toFlip);
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
                bool flag = analyseInDir(model, move, selfPiece, -1, -1) || analyseInDir(model, move, selfPiece, -1, 0) ||
                    analyseInDir(model, move, selfPiece, -1, 1) || analyseInDir(model, move, selfPiece, 0, -1) ||
                    analyseInDir(model, move, selfPiece, 0, 1) || analyseInDir(model, move, selfPiece, 1, -1) ||
                    analyseInDir(model, move, selfPiece, 1, 0) || analyseInDir(model, move, selfPiece, 1, 1);

                if (flag)
                    validMoves.push_back(move);
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
    
    Square auxSquare;


    if (analyseInDir(model, move, piece, -1, -1))
        flipInDir(model, move, piece, -1, -1);

    if (analyseInDir(model, move, piece, -1, 0))
        flipInDir(model, move, piece, -1, 0);

    if (analyseInDir(model, move, piece, -1, 1))
        flipInDir(model, move, piece, -1, 1);

    if (analyseInDir(model, move, piece, 0, -1))
        flipInDir(model, move, piece, 0, -1);
          
    if (analyseInDir(model, move, piece, 0, 1))
        flipInDir(model, move, piece, 0, 1);

    if (analyseInDir(model, move, piece, 1, -1))
        flipInDir(model, move, piece, 1, -1);

    if (analyseInDir(model, move, piece, 1, 0))
        flipInDir(model, move, piece, 1, 0);

    if (analyseInDir(model, move, piece, 1, 1))
        flipInDir(model, move, piece, 1, 1);

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

