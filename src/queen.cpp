#include "queen.h"

Queen::Queen(Piece::Team team, Position pos, SDLHandler *handler)
    : SlidingPiece(team, pos, handler, QUEEN) {
    if (team == WHITE){
        filename = "../resources/w_queen.png";
    }
    else{
        filename = "../resources/b_queen.png";
    }
    pieceTexture = handler->loadImageFromPng(filename);

    // Define the possible directions for the queen's moves
    // Combine rook and bishop directions
    std::vector<std::pair<int, int>> queenDirections = {
            {-1, 0}, {1, 0},  // Vertical directions
            {0, -1}, {0, 1},   // Horizontal directions
            {-1, -1}, {-1, 1},  // Diagonal directions
            {1, -1}, {1, 1}
    };

    setDirections(queenDirections);
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
     * movements of the rook and bishop in the directions vector.
     */
    std::vector<Position> pseudoMoves;

    for (const auto& direction : directions) {
        // Take initial coordinates of the piece before starting to check for
        // Pseudo-legal moves
        int newRow = pPos.xCoord;
        int newCol = pPos.yCoord;

        while (true) {
            newRow += direction.first;
            newCol += direction.second;

            if (isOutOfBounds(newRow, newCol)) {
                break;
            }
            Position newPosition{newRow, newCol};
            pseudoMoves.push_back(newPosition);
        }
    }

    return pseudoMoves;
}
