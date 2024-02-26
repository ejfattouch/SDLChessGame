#pragma once
#import <map>
#import "piece.h"
#import "pawn.h"
#import "rook.h"
#import "bishop.h"
#import "knight.h"
#import "queen.h"
#import "sdlHandler.h"
#import "king.h"


class ChessBoard {
private:
    SDLHandler* handler;
    Piece* boardArr[8][8];
    Piece::Team currentTurn;
    // Which direction the board is displayed. 0 is white on bottom, 1 is black on bottom
    int boardOrientation = 0;

    // Vector to store the pieces of each team
    std::vector<Piece*> blackPieces;
    std::vector<Piece*> whitePieces;

    King* blackKing;
    King* whiteKing;

public:
    ChessBoard(SDLHandler* handler, std::string fen);
    explicit ChessBoard(SDLHandler* handler);
    ~ChessBoard();

    // Handles everything that involves FEN notation
    // https://www.chess.com/terms/fen-chess#
    void getPosFromFEN(const std::string&);
    std::string getFENFromPos();

    Piece* getPieceAtCoord(Position p) {return boardArr[p.xCoord][p.yCoord];}
    static bool canMoveTo(const std::vector<Position>&, Position);
    void moveTo(Piece*, Position);
    bool isSquareEmpty(Position p){return boardArr[p.xCoord][p.yCoord] == nullptr;}
    Piece::Team getCurrentTurn(){return currentTurn;}

    void flipBoardOrientation();
    void setBoardOrientation(int);

    std::vector<Position> calculateLegalMoves(Piece*);

    // Returns the position of a piece with respect to the orientation of the board.
    Position getPositionOfPieceWOrientation();



    void renderAllPieces();
    void renderAllPossibleMoves(const std::vector<Position>&);

private:
    std::vector<Position> calculateSlidingPieceLegalMove(std::vector<std::pair<int,int>>, Piece*);
    void changePlayerTurn();

    // Initializes the starting position with the starting FEN
    void initStartingPosition();

    bool checkForEnPassantPawns(Piece*);
    bool checkForChecks(Piece::Team);
    void addPieceToPieceList(Piece*);
    void capturePiece(Piece*);
};

inline SDL_Rect createRectangle(int x, int y, int h, int w){
    SDL_Rect rect;
    rect.x  = x;
    rect.y = y;
    rect.h = h;
    rect.w = w;

    return rect;
}
