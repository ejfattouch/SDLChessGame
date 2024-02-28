#pragma once
#include <utility>
#include "piece.h"

class SlidingPiece : public Piece{
protected:
    std::vector<std::pair<int,int>> directions;
public:
    SlidingPiece(Team team, Position pos, SDLHandler *handler, PieceType type) :
    Piece(team, pos, handler, type) {
    }

    std::vector<std::pair<int,int>> getDirections(){
        return directions;
    }
};
