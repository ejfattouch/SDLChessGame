#pragma once
#import "slidingPiece.h"

class Bishop: public SlidingPiece{
public:
    // Constructor
    Bishop(Team team, Position pos, SDLHandler* handler);
    std::vector<Position> calculatePseudoMoves() override;
};
