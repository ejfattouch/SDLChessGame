#include <string>
#include <iostream>
#include <random>
#include "chessBoard.h"

void ChessBoard::getPosFromFEN(const std::string& fen) {
    //set file to 0 and rank to 7 since FEN notation starts from top to bottom.
    int file = 0;
    int rank = 7;
    for (auto i : fen){
        if (i == '/'){
            file = 0;
            rank--;
        }
        else if (i == ' '){
            break;
        }
        else{
            if (isdigit(i)){
                file = file + (i - '0'); // Subtract by '0' to convert to int
            }
            else{
                Piece::Team team = isupper(i) ? Piece::WHITE : Piece::BLACK;
                Position pos{};
                pos.xCoord = file;
                pos.yCoord = rank;
                switch (tolower(i)) {
                    case 'r':
                        // create rook piece
                        boardArr[file][rank] = new Rook(team, pos, handler);
                        break;
                    case 'p':
                        // create pawn piece
                        boardArr[file][rank] = new Pawn(team, pos, handler);
                        break;
                    case 'b':
                        // create bishop
                        boardArr[file][rank] = new Bishop(team, pos, handler);
                        break;
                    case 'n':
                        // create knight
                        boardArr[file][rank] = new Knight(team, pos, handler);
                        break;
                    case 'q':
                        // create queen
                        boardArr[file][rank] = new Queen(team, pos, handler);
                        break;
                    case 'k':
                        // create king
                        boardArr[file][rank] = new King(team, pos, handler);
                        break;
                    default:
                        break;
                }
                file++;
            }
        }
    }

    // check whose turn it is
    currentTurn = fen.back() == 'w' ? Piece::WHITE : Piece::BLACK;
}


std::string ChessBoard::getFENFromPos() {
    return "";
}

void ChessBoard::initStartingPosition() {
    std::string startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
    getPosFromFEN(startingFen);
}

ChessBoard::ChessBoard(SDLHandler *handler, std::string fen)
    : handler(handler){
    getPosFromFEN(fen);
}


ChessBoard::ChessBoard(SDLHandler *handler)
    : handler(handler){
    initStartingPosition();
}

void ChessBoard::renderAllPieces() {
    int inverseCoordsVar = boardOrientation == 1 ? 0 : 7;

    for (int x = 0; x < 8; x++){
        for (int y = 0; y < 8; y++){
            Piece* piece = boardArr[x][y];
            if (piece != nullptr){
                int screenW = handler->SCREEN_W;
                int screenH = handler->SCREEN_H;
                // Draw pawns smaller than other pieces
                int offset = piece->getPieceType() == Piece::PAWN ? 15 : 8;

                SDL_Rect container = createRectangle((abs(7-inverseCoordsVar - x))*screenW/8 + offset, (abs(inverseCoordsVar-y)*screenH/8 + offset), screenH/8-(2*offset), screenW/8-(2*offset));
                handler->renderPiece(container, piece->getReferenceToTexture());
            }
        }
    }
}

void ChessBoard::moveTo(Piece* piece, Position endPos) {
    Position startPos = piece->getPosition();
    piece->setPosition(endPos);

    // Check if piece is king and deactivate castling if true
    if (piece->getPieceType() == Piece::KING){
        King* kingPtr = dynamic_cast<King *>(piece);
        kingPtr->deactivateCastling();
    }

    // Set the square where the piece departed to nullptr
    boardArr[startPos.xCoord][startPos.yCoord] = nullptr;

    int x = endPos.xCoord;
    int y = endPos.yCoord;

    // If piece is captured then delete the piece
    if (boardArr[x][y] != nullptr){
        delete boardArr[x][y];
    }
    // Move the piece to the desired square
    boardArr[x][y] = piece;

}

ChessBoard::~ChessBoard() {
    // Goes through boardArr and delete all Pieces
    for (auto & i : boardArr){
        for (auto & j : i){
            delete j;
        }
    }
}

void ChessBoard::flipBoardOrientation() {
    // Flips the value of boardOrientation between 0 and 1
    boardOrientation = 1 - boardOrientation;
}

Position ChessBoard::getPositionOfPieceWOrientation() {
    int inverseCoordsVar = boardOrientation == 1 ? 0:7;

    int xPos = abs(((7-inverseCoordsVar) - handler->event.button.x / (handler->SCREEN_W/8)));
    int yPos = abs(inverseCoordsVar - (handler->event.button.y / (handler->SCREEN_H/8)));

    std::cout << xPos << " " << yPos << std::endl;

    Position pos {xPos, yPos};
    return pos;
}

void ChessBoard::renderAllPossibleMoves(const std::vector<Position>& possibleMoves) {
    int inverseCoordsVar = boardOrientation == 1 ? 0:7;
    for (Position pos : possibleMoves){
        handler->drawDarkerSquare(abs((7-inverseCoordsVar) - pos.xCoord), abs(inverseCoordsVar - pos.yCoord), 1);
    }
}

bool ChessBoard::canMoveTo(const std::vector<Position>& positionVector, Position positionToCheck) {
    for (Position p : positionVector){
        if (p == positionToCheck){
            return true;
        }
    }

    return false;
}

/**
 * Sets the orientation of the board.
 * @param orientation Black on bottom if negative, white if positive and random if equal to 0.
 */
void ChessBoard::setBoardOrientation(int orientation) {
    if (orientation == 0){
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, 1);
        boardOrientation = distrib(gen);  // generates number in the range 1..6
    }
    else if (orientation < 0){
        boardOrientation = 1;
    }
    else {
        boardOrientation = 0;
    }

}

std::vector<Position> ChessBoard::calculateLegalMoves(Piece* piece) {
    std::vector<Position> pseudoLegalMoves = piece->calculatePseudoMoves();
    std::vector<Position> legalMoves {};
    Piece::Team pTeam = piece->getTeam();

    switch (piece->getPieceType()){
        case Piece::KING:
            for (Position p : pseudoLegalMoves){
                int x = p.xCoord;
                int y = p.yCoord;

                // Checks for regular king moves
                if (boardArr[x][y] == nullptr){
                    legalMoves.push_back(p);
                    continue;
                }
                else if (boardArr[x][y]->getTeam() != pTeam){
                    legalMoves.push_back(p);
                }

                // Implement checks for castling and checks.
            }
            break;
        case Piece::KNIGHT:
            for (Position p : pseudoLegalMoves){
                Piece* piecePtr = boardArr[p.xCoord][p.yCoord];
                if (piecePtr == nullptr){
                    legalMoves.push_back(p);
                }
                else if (piecePtr->getTeam() != pTeam){
                    legalMoves.push_back(p);
                }
            }
            break;
        case Piece::ROOK:
            legalMoves = piece->calculatePseudoMoves();
            break;
        case Piece::BISHOP:
            legalMoves = piece->calculatePseudoMoves();
            break;
        case Piece::QUEEN:
            legalMoves = piece->calculatePseudoMoves();
            break;
        case Piece::PAWN:
            Position pawnStartPos = piece->getPosition();

            for (Position p : pseudoLegalMoves){
                // Check for capture
                if (p.xCoord != pawnStartPos.xCoord){
                    if (boardArr[p.xCoord][p.yCoord] == nullptr){
                        continue;
                    }
                    else if (boardArr[p.xCoord][p.yCoord]->getTeam() != pTeam){
                        legalMoves.push_back(p);
                    }
                }
                else {
                    if (boardArr[p.xCoord][p.yCoord] == nullptr) {
                        if (abs(p.yCoord - pawnStartPos.yCoord) == 2) {
                            // If two square move is possible check the in between square first
                            Piece *pPtr = (p.yCoord - pawnStartPos.yCoord) > 0 ? boardArr[p.xCoord][pawnStartPos.yCoord + 1]: boardArr[p.xCoord][pawnStartPos.yCoord - 1];
                            // Check if in between square is empty
                            if (pPtr == nullptr) {
                                legalMoves.push_back(p);
                            }
                        } else {
                            legalMoves.push_back(p);
                        }
                    }
                }
            }
            break;
    }

    return legalMoves;
}
