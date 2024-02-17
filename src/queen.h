#pragma once
#include "slidingPiece.h"

class Queen : public SlidingPiece{
public:
    // Constructor
    Queen(Team team, Position pos, SDLHandler* handler);
    std::vector<Position> calculatePseudoMoves() override;

};