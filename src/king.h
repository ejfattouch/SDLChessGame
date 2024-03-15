#pragma once
#import "piece.h"

class King : public Piece{
private:
    bool moved = false;
    bool inCheck = false;

    bool kingSideCastleEnabled = true;
    bool queenSideCastleEnabled = true;
public:
    // Constructor
    King(Team team, Position pos, SDLHandler* handler);

    void setCheck(bool check = true){inCheck = check;}
    bool isInCheck() const{return inCheck;}
    // Check if king has moved. Used for castling
    [[nodiscard]] bool hasMoved() const{return moved;};
    void deactivateCastling() {moved = true;}

    bool isCastleEnabledForPiece() const{
        return !moved && !inCheck && (kingSideCastleEnabled || queenSideCastleEnabled);
    }
    bool canCastleKingSide() const{return kingSideCastleEnabled;}
    bool canCastleQueenSide() const{return queenSideCastleEnabled;}
    void deactivateKingSide() {kingSideCastleEnabled = false;}
    void deactivateQueenSide() {queenSideCastleEnabled = false;}

    std::vector<Position> calculatePseudoMoves() override;
};
