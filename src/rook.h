#pragma once
#import "piece.h"

class Rook : public Piece{
private:
    bool moved = false;
protected:
    // Define the possible directions for the rook's moves
    int directions[4][2] = {
            {-1, 0}, {1, 0},  // Vertical directions
            {0, -1}, {0, 1}   // Horizontal directions
    };
public:
    // Constructor
    Rook(Team team, Position pos, SDLHandler* handler);
    [[nodiscard]] bool hasMoved() const {return moved;};
    std::vector<Position> calculatePseudoMoves() override;

};