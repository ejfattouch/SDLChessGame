#pragma once
#import "slidingPiece.h"

class Rook : public SlidingPiece{
private:
    bool moved = false;
public:
    // Constructor
    Rook(Team team, Position pos, SDLHandler* handler);
    [[nodiscard]] bool hasMoved() const {return moved;};
    std::vector<Position> calculatePseudoMoves() override;

};