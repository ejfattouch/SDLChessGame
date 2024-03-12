#include "pawn.h"

Pawn::Pawn(Piece::Team team, Position pos, SDLHandler *handler)
    : Piece(team, pos, handler, PAWN) {
    if (team == WHITE){
        filename = "../resources/w_pawn.png";
    }
    else{
        filename = "../resources/b_pawn.png";
    }
    pieceTexture = handler->loadImageFromPng(filename);
}

std::vector<Position> Pawn::calculatePseudoMoves() {
/* Pawn moves along the ranks
 *
 * Regular Move   Initial Move   Capture/en passant
 * --------         --------        --------
 * --------         --------        --x-x---
 * --------         --------        ---P----
 * ---x----         --------        --------
 * ---P----         ---x----        --------
 * --------         ---x----        --------
 * --------         ---P----        --------
 * --------         --------        --------
 *
 * To calculate all possible pseudoMoves for the pawn, we check squares in front
 * of it. The pawn can move one square forward or two squares forward on its
 * initial move. It can also capture if there is a piece at a distance of 1 to its
 * forward diagonals.
 *
 * For the sake of the pseudoMoves, we will always assume a piece can be captured.
 */
    std::vector<Position> pseudoMoves;

    int x = pPos.xCoord;
    int y = pPos.yCoord;

    if (team == Piece::WHITE){
        if (y == 1){
            pseudoMoves.push_back(Position{x, y+2});
        }

        y = y+1;
    }
    else{
        if (y == 6){
            pseudoMoves.push_back((Position{x, y-2}));
        }
        y = y-1;
    }

    if (x==0){
        pseudoMoves.push_back(Position{x,y});
        pseudoMoves.push_back(Position{x+1,y});
    }
    else if (x==7){
        pseudoMoves.push_back(Position{x,y});
        pseudoMoves.push_back(Position{x-1,y});
    }
    else{
        pseudoMoves.push_back(Position{x,y});
        pseudoMoves.push_back(Position{x+1,y});
        pseudoMoves.push_back(Position{x-1,y});
    }

    return pseudoMoves;
}

void Pawn::activateEnPassant(int direction) {
    if (direction == LEFT_EN_PASSANT){
        leftEnPassant = true;
    }
    else{
        rightEnPassant = true;
    }

}

Position Pawn::getEnPassantMove() {
    int y = team == Piece::WHITE ? 5 : 2;

    if (leftEnPassant){
        return Position{pPos.xCoord-1, y};
    }
    else if (rightEnPassant){
        return Position{pPos.xCoord+1, y};
    }
    return {};
}

bool Pawn::isOnPromotionRank() {
    if (team == Piece::WHITE){
        return pPos.yCoord == 7;
    }
    else{
        return pPos.yCoord == 0;
    }
}
