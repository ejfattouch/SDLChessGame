#pragma once
#import "piece.h"

class Pawn : public Piece{
public:
    // Constructor
    Pawn(Team team, Position pos, SDLHandler* handler);
    std::vector<Position> calculatePseudoMoves() override;
};