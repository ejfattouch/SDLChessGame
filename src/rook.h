#pragma once
#import "slidingPiece.h"

class Rook : public SlidingPiece{
private:
    bool moved = false;
public:
    // Constructor
    Rook(Team team, Position pos, SDLHandler* handler);
    Rook* clone() const override{
        return new Rook(*this);
    }

    [[nodiscard]] bool hasMoved() const {return moved;};
    std::vector<Position> calculatePseudoMoves() override;

};