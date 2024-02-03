#pragma once
#import "piece.h"

class Bishop: public Piece{
protected:
    // Define the possible diagonal directions for the bishop's moves
    int directions[4][2] = {
            {-1, -1}, {-1, 1},  // Diagonal directions
            {1, -1}, {1, 1}
    };
public:
    // Constructor
    Bishop(Team team, Position pos, SDLHandler* handler);
    std::vector<Position> calculatePseudoMoves() override;
};
