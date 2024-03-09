#pragma once
#import "piece.h"

class King : public Piece{
private:
    bool moved = false;
    bool inCheck = false;
public:
    // Constructor
    King(Team team, Position pos, SDLHandler* handler);

    void setCheck(bool check = true){inCheck = check;}
    bool isInCheck(){return inCheck;}
    // Check if king has moved. Used for castling
    [[nodiscard]] bool hasMoved() const{return moved;};
    void deactivateCastling() {moved = true;}

    std::vector<Position> calculatePseudoMoves() override;
};
