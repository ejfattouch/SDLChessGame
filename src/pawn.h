#pragma once
#import "piece.h"
#define LEFT_EN_PASSANT 0
#define RIGHT_EN_PASSANT 1

class Pawn : public Piece{
private:
    bool rightEnPassant = false;
    bool leftEnPassant = false;
    Position enPassantPos;

public:
    // Constructor
    Pawn(Team team, Position pos, SDLHandler* handler);
    std::vector<Position> calculatePseudoMoves() override;
    void activateEnPassant(int);
    void deactivateEnPassant() {rightEnPassant = false; leftEnPassant = false;}
    Position getEnPassantMove();
};