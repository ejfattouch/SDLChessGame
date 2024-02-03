#include "knight.h"

Knight::Knight(Piece::Team team, Position pos, SDLHandler *handler)
    : Piece(team, pos, handler, KNIGHT) {
    if (team == WHITE){
        filename = "../resources/w_knight.png";
    }
    else{
        filename = "../resources/b_knight.png";
    }
    pieceTexture = handler->loadImageFromPng(filename);
}

std::vector<Position> Knight::calculatePseudoMoves() {
    std::vector<Position> pseudoMoves;

    // Define the possible knight move offsets
    int offsets[][2] = {
            {-2, -1}, {-2, 1},
            {-1, -2}, {-1, 2},
            {1, -2}, {1, 2},
            {2, -1}, {2, 1}
    };

    // Iterate over possible offsets
    for (const auto& offset : offsets) {
        int newRow = pPos.xCoord + offset[0];
        int newCol = pPos.yCoord + offset[1];

        // Check if the new position is within bounds (assuming a chessboard)
        if (!isOutOfBounds(newRow, newCol)) {
            Position newPosition{newRow, newCol};
            pseudoMoves.push_back(newPosition);
        }
    }

    return pseudoMoves;
}

