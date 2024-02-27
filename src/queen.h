#pragma once
#include "slidingPiece.h"

class Queen : public SlidingPiece{
public:
    // Constructor
    Queen(Team team, Position pos, SDLHandler* handler);
    Queen* clone() const override{
        return new Queen(*this);
    }

    std::vector<Position> calculatePseudoMoves() override;

};