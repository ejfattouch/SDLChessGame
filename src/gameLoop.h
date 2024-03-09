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

    bool userRequestsDraw = false;
    bool userRequestResignation = false;

    Piece* pieceAtFirstPos;
    Position squarePos {0, 0};

    int turnNumber = 1;
    int lastPawnMoveOrCapture = 1;

    std::unordered_map<std::string, int> positionCountMap;

public:
    gameLoop();
    gameLoop(const std::string& FEN);
    void run();
private:
    void renderGameElements();
    void mouseDownEvent();
    std::vector<Position> calculateLegalMovesWithRespectToChecks(Piece*);
    bool simulateMoveAndCheckForCheck(Piece* piece, Position p);

    // Verifies if the position is a checkmate
    bool isCheckMate();

    // The below methods are used to handle all draw conditions
    bool isDraw();
    bool isThreeFoldRepetition(const std::string&);
    bool isFiftyMoveRule() const;
    bool isStalemate();

    // Checks for dead position according to rules established in
    // https://www.chess.com/terms/draw-chess
    bool isDeadPosition();
};
