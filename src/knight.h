#pragma once
#include "piece.h"

class Knight : public Piece{
public:
    // Constructor
    Knight(Team team, Position pos, SDLHandler* handler);
    Knight* clone() const override{
        return new Knight(*this);
    }
    std::vector<Position> calculatePseudoMoves() override;
};