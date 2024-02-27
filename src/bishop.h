#pragma once
#import "slidingPiece.h"

class Bishop: public SlidingPiece{
public:
    // Constructor
    Bishop(Team team, Position pos, SDLHandler* handler);
    Bishop* clone() const override{
        return new Bishop(*this);
    }

    std::vector<Position> calculatePseudoMoves() override;
};
