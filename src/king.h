#pragma once
#import "piece.h"

class King : public Piece{
private:
    bool moved = false;
public:
    // Constructor
    King(Team team, Position pos, SDLHandler* handler);
    King* clone() const override{
        return new King(*this);
    }

    // Check if king has moved. Used for castling
    [[nodiscard]] bool hasMoved() const{return moved;};
    void deactivateCastling() {moved = true;}

    std::vector<Position> calculatePseudoMoves() override;
};
