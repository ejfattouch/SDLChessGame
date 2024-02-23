#include "king.h"

King::King(Piece::Team team, Position pos, SDLHandler *handler)
    : Piece(team, pos, handler, KING) {
    if (team == WHITE){
        filename = "../resources/w_king.png";
    }
    else{
        filename = "../resources/b_king.png";
    }
    pieceTexture = handler->loadImageFromPng(filename);
}

std::vector<Position> King::calculatePseudoMoves() {
    /* Kings moves as follows
     * --------
     * --------
     * --xxx---
     * --xKx---
     * --xxx---
     * --------
     * --------
     * --------
     * To calculate all possible pseudoMoves for the king we check every square
     * with distance 1 from the starting square of the king and remove any out of bounds targets.
     */
    std::vector<Position> pseudoMoves;

    int offsets[][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},            {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    for (const auto& offset: offsets){
        int newRank = pPos.xCoord+offset[0];
        int newFile = pPos.yCoord+offset[1];

        if (isOutOfBounds(newRank, newFile)){
            continue;
        }

        pseudoMoves.push_back(Position{newRank, newFile});
    }

    return pseudoMoves;
}
