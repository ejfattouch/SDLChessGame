#include "piece.h"

Piece::Piece(Piece::Team team, Position pos, SDLHandler *handler, Piece::PieceType type)
    : team(team), pPos(pos), pieceHandler(handler), pType(type){
}

Piece::~Piece() {
    // Destroy texture and call drawSquare to replace it.
//  SDL_DestroyTexture(pieceTexture);
    pieceHandler->drawSquare(pPos.xCoord, pPos.yCoord);
}

bool Piece::isOutOfBounds(int rank, int file) {
    return rank > 7 || rank < 0 || file > 7 || file < 0;
}
