#pragma once
#include "piece.h"

class Knight : public Piece{
public:
    // Constructor
    Knight(Team team, Position pos, SDLHandler* handler);
    std::vector<Position> calculatePseudoMoves() override;
};