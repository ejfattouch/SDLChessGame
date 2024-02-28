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

    void setDirections(std::vector<std::pair<int,int>> dir){
        directions = std::move(dir);
    }
    std::vector<std::pair<int,int>> getDirections(){
        return directions;
    }
};
