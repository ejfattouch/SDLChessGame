#pragma once
#include "piece.h"
#include "bishop.h"
#include "rook.h"

class Queen : public Piece{
public:
    // Constructor
    Queen(Team team, Position pos, SDLHandler* handler);
    std::vector<Position> calculatePseudoMoves() override;

};