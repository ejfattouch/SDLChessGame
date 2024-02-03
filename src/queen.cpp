#include "queen.h"


Queen::Queen(Piece::Team team, Position pos, SDLHandler *handler)
    : Piece(team, pos, handler, QUEEN) {
    if (team == WHITE){
        filename = "../resources/w_queen.png";
    }
    else{
        filename = "../resources/b_queen.png";
    }
    pieceTexture = handler->loadImageFromPng(filename);
}

std::vector<Position> Queen::calculatePseudoMoves() {
    /* Queen moves like a bishop and rook along the files, ranks and diagonals
     * x--x--x-
     * -x-x-x--
     * --xxx---
     * xxxQxxxx
     * --xxx---
     * -x-x-x--
     * x--x--x-
     *
     * To calculate all possible pseudoMoves for the queen, we combine the
     * movements of the rook and bishop.
     */
    std::vector<Position> pseudoMoves;

   // Calculate bishop moves
   // Taken from bishop class
    int bishopDirections[][2] = {
            {-1, -1}, {-1, 1},  // Diagonal directions
            {1, -1}, {1, 1}
    };

    for (const auto& direction : bishopDirections) {
        int newRow = pPos.xCoord;
        int newCol = pPos.yCoord;

        while (true) {
            newRow += direction[0];
            newCol += direction[1];

            if (isOutOfBounds(newRow, newCol)) {
                break;
            }
            Position newPosition{newRow, newCol};
            pseudoMoves.push_back(newPosition);
        }
    }
    // Calculate rook moves
    // Taken from rook class
    int rookDirections[][2] = {
            {-1, 0}, {1, 0},  // Vertical directions
            {0, -1}, {0, 1}   // Horizontal directions
    };
    // Iterate over possible directions
    for (const auto& direction : rookDirections) {
        int newRow = pPos.xCoord;
        int newCol = pPos.yCoord;

        // Move along the direction until out of bounds
        while (true) {
            newRow += direction[0];
            newCol += direction[1];

            // Check if the new position is within bounds (assuming a chessboard)
            if (isOutOfBounds(newRow, newCol)) {
                break;  // Stop if out of bounds
            }

            Position newPosition{newRow, newCol};
            pseudoMoves.push_back(newPosition);
        }
    }
    return pseudoMoves;
}
