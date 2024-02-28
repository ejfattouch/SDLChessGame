#pragma once

#include "sdlHandler.h"
#include "chessBoard.h"
#include "piece.h"
#include <string>

class gameLoop {
private:
    SDLHandler handler;
    std::unique_ptr<ChessBoard> chessBoard;
    std::vector<Position> v = {};

    bool running;
    bool firstClickOnPiece;

    Piece* pieceAtFirstPos;
    Position squarePos {0, 0};

    int turnNumber = 1;
public:
    gameLoop();
    void run();
private:
    void renderGameElements();
    void mouseDownEvent();
    std::vector<Position> calculateLegalMovesWithRespectToChecks(Piece*);
    bool simulateMoveAndCheckForCheck(Piece* piece, Position p);
};
