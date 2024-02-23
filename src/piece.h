#pragma once
#include <SDL.h>
#include "sdlHandler.h"
#include <string>

class Piece {
public:
    enum Team {BLACK, WHITE};
    enum PieceType {PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN};

protected:
    std::string filename;
    SDL_Texture* pieceTexture = nullptr;
    SDLHandler* pieceHandler;

    Team team;
    PieceType pType;
    Position pPos;

public:
    // Constructor
    Piece(Team team, Position pos, SDLHandler* handler, PieceType type);
    // Copy Constructor
    Piece(const Piece& piece) = default;
    // Destructor
    virtual ~Piece();

    Team getTeam() {return team;};
    PieceType getPieceType() {return pType;}
    Position getPosition() {return pPos;}
    void setPosition(Position pos) {pPos = pos;}
    SDL_Texture* getReferenceToTexture(){return pieceTexture;}

    // Checks if a position is outOfBounds
    static bool isOutOfBounds(int rank, int file);
    virtual std::vector<Position> calculatePseudoMoves() = 0;
};
