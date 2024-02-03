#include "rook.h"

Rook::Rook(Piece::Team team, Position pos, SDLHandler *handler)
    : Piece(team, pos, handler, ROOK) {
    if (team == WHITE){
        filename = "../resources/w_rook.png";
    }
    else{
        filename = "../resources/b_rook.png";
    }
    pieceTexture = handler->loadImageFromPng(filename);
}

std::vector<Position> Rook::calculatePseudoMoves() {
    /* Rook moves along the ranks and files
     * ---x----
     * ---x----
     * ---x----
     * xxxRxxxx
     * ---x----
     * ---x----
     * ---x----
     *
     * To calculate all possible pseudoMoves for the rook, we check every square
     * along the ranks and files and stop once we reach an out-of-bounds target.
     */

    std::vector<Position> pseudoMoves;


    // Iterate over possible directions
    for (const auto& direction : directions) {
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

