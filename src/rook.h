#pragma once
#import "slidingPiece.h"

class Rook : public SlidingPiece{
public:
    // Constructor
    Rook(Team team, Position pos, SDLHandler* handler);
    std::vector<Position> calculatePseudoMoves() override;
};