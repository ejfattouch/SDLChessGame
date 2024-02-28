#include "bishop.h"

Bishop::Bishop(Piece::Team team, Position pos, SDLHandler *handler)
    : SlidingPiece(team, pos, handler, BISHOP) {
    if (team == WHITE){
        filename = "../resources/w_bishop.png";
    }
    else{
        filename = "../resources/b_bishop.png";
    }
    pieceTexture = handler->loadImageFromPng(filename);

    // Define the possible directions for the bishop's moves
    directions = {
            {-1, -1}, {-1, 1},  // Diagonal directions
            {1, -1}, {1, 1}
    };
}

std::vector<Position> Bishop::calculatePseudoMoves() {
    /* Bishop moves along the diagonals
     * x-----x-
     * -x---x--
     * --x-x---
     * ---B----
     * --x-x---
     * -x---x--
     * x-----x-
     *
     * To calculate all possible pseudoMoves for the bishop we check every square
     * along the diagonals and stop once we reach and out of bounds target.
     */

    std::vector<Position> pseudoMoves;

    // Iterate over possible diagonal directions
    for (const auto& direction : directions) {
        int newRow = pPos.xCoord;
        int newCol = pPos.yCoord;

        // Move along the diagonal until out of bounds
        while (true) {
            newRow += direction.first;
            newCol += direction.second;

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
