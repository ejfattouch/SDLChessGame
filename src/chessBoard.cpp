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

                Piece* creatorPiece;
                switch (tolower(i)) {
                    case 'r':
                        // create rook piece
                        creatorPiece = new Rook(team, pos, handler);
                        boardArr[file][rank] = creatorPiece;
                        break;
                    case 'p':
                        // create pawn piece
                        creatorPiece = new Pawn(team, pos, handler);
                        boardArr[file][rank] = creatorPiece;
                        break;
                    case 'b':
                        // create bishop
                        creatorPiece = new Bishop(team, pos, handler);
                        boardArr[file][rank] = creatorPiece;
                        break;
                    case 'n':
                        // create knight
                        creatorPiece = new Knight(team, pos, handler);
                        boardArr[file][rank] = creatorPiece;
                        break;
                    case 'q':
                        // create queen
                        creatorPiece = new Queen(team, pos, handler);
                        boardArr[file][rank] = creatorPiece;
                        break;
                    case 'k':
                        // create king
                        creatorPiece = new King(team, pos, handler);
                        boardArr[file][rank] = creatorPiece;
                        break;

                }
                if (creatorPiece->getPieceType() != Piece::KING){
                    addPieceToPieceList(creatorPiece);
                }
                else{
                    if (creatorPiece->getTeam() == Piece::WHITE){
                        whiteKing = dynamic_cast<King *>(creatorPiece);
                    }
                    else{
                        blackKing = dynamic_cast<King *>(creatorPiece);
                    }
                }

                file++;
            }
        }
    }

    // check whose turn it is
    currentTurn = fen.back() == 'w' ? Piece::WHITE : Piece::BLACK;
}


std::string ChessBoard::getFENFromPos() {
    std::string positionFEN;
    for (int rank = 7; rank >= 0; rank--){
        int numOfEmptySquares = 0 ;
        for (int file = 0; file < 8; file++){
            Piece* pieceOnSquare = boardArr[file][rank];

            if (pieceOnSquare == nullptr){
                numOfEmptySquares++;
            }
            if (rank == 7 || pieceOnSquare != nullptr){
                if (numOfEmptySquares > 0){
                    positionFEN += std::to_string(numOfEmptySquares);
                    numOfEmptySquares = 0;
                }
            }

            if (pieceOnSquare != nullptr){
                Piece::PieceType pType = pieceOnSquare->getPieceType();
                Piece::Team pTeam = pieceOnSquare->getTeam();

                char fenChar;
                switch (pType) {
                    case Piece::PAWN:
                        fenChar = 'p';
                        break;
                    case Piece::ROOK:
                        fenChar = 'r';
                        break;
                    case Piece::BISHOP:
                        fenChar = 'b';
                        break;
                    case Piece::KNIGHT:
                        fenChar = 'n';
                        break;
                    case Piece::KING:
                        fenChar = 'k';
                        break;
                    case Piece::QUEEN:
                        fenChar = 'q';
                        break;
                    default:
                        std::cout << "Piece Type was Either NULL or Unrecognized";
                        break;
                }
                if (pTeam == Piece::WHITE){
                    fenChar = toupper(fenChar);
                }
                positionFEN.push_back(fenChar);
            }
        }
        if (numOfEmptySquares > 0){
            positionFEN += std::to_string(numOfEmptySquares);
        }

        if (rank != 0){
            positionFEN.push_back('/');
        }
    }

    positionFEN.push_back(' ');

    if (currentTurn == Piece::WHITE){
        positionFEN.push_back('w');
    }
    else{
        positionFEN.push_back('b');
    }

    return positionFEN;
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

    // Set the square where the piece departed to nullptr
    boardArr[startPos.xCoord][startPos.yCoord] = nullptr;

    int x = endPos.xCoord;
    int y = endPos.yCoord;

    bool squareWasEmpty = true;
    // If piece is captured then delete the piece
    if (boardArr[x][y] != nullptr){
        Piece* capturedPiece = boardArr[x][y];
        capturePiece(capturedPiece);
        squareWasEmpty = false;
    }
    // Move the piece to the desired square
    boardArr[x][y] = piece;

    // Check if piece is king and deactivate castling if true
    Piece::PieceType pType = piece->getPieceType();
    if (pType == Piece::KING){
        King* kingPtr = dynamic_cast<King *>(piece);
        kingPtr->deactivateCastling();
    }
    else if (pType == Piece::PAWN){
        // Check if move enables en passant
        checkForEnPassantPawns(piece);
       // If the pawn moved to an empty square check if there is a pawn underneath it
        if (squareWasEmpty){
            // If there is a pawn underneath it then delete it as it was an en passant
            int checkY = piece->getTeam() == Piece::WHITE ? y-1 : y+1;
            Piece* pieceUnderPawn = boardArr[x][checkY];
            if (pieceUnderPawn != nullptr){
                if (pieceUnderPawn->getPieceType() == Piece::PAWN){
                    capturePiece(pieceUnderPawn);
                    boardArr[x][checkY] = nullptr;
                }
            }
        }

        Pawn* pawn = dynamic_cast<Pawn*>(piece);
        pawn->deactivateEnPassant();
    }

    changePlayerTurn();

    if (checkForChecks(Piece::WHITE)){
        std::cout << "White is in check" << std::endl;
    }
    if (checkForChecks(Piece::BLACK)){
        std::cout << "Black is in check" << std::endl;
    }
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
    std::vector<Position> pseudoLegalMoves;
    std::vector<Position> legalMoves {};
    Piece::Team pTeam = piece->getTeam();

    switch (piece->getPieceType()){
        case Piece::KING:
            pseudoLegalMoves = piece->calculatePseudoMoves();
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
            pseudoLegalMoves = piece->calculatePseudoMoves();
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
            legalMoves = calculateSlidingPieceLegalMove(dynamic_cast<Rook*>(piece)->getDirections(), piece);
            break;
        case Piece::BISHOP:
            legalMoves = calculateSlidingPieceLegalMove(dynamic_cast<Bishop*>(piece)->getDirections(), piece);
            break;
        case Piece::QUEEN:
            legalMoves = calculateSlidingPieceLegalMove(dynamic_cast<Queen*>(piece)->getDirections(), piece);
            break;
        case Piece::PAWN:
            Position pawnStartPos = piece->getPosition();
            pseudoLegalMoves = piece->calculatePseudoMoves();
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
            // Checking for enPassant
            Position enPassant = dynamic_cast<Pawn*>(piece)->getEnPassantMove();

            if (enPassant.yCoord != 0){
                legalMoves.push_back(enPassant);
            }

            break;
    }

    return legalMoves;
}

std::vector<Position> ChessBoard::calculateSlidingPieceLegalMove(std::vector<std::pair<int, int>> directions, Piece* piece) {
    Position pPos = piece->getPosition();
    std::vector<Position> legalMoves;

    for (const auto& direction : directions) {
        int newRow = pPos.xCoord;
        int newCol = pPos.yCoord;

        // Move along the direction until out of bounds
        while (true) {
            newRow += direction.first;
            newCol += direction.second;

            // Check if the new position is within bounds (assuming a chessboard)
            if (Piece::isOutOfBounds(newRow, newCol)) {
                break;  // Stop if out of bounds
            }
            Piece* pieceOnBoard = boardArr[newRow][newCol];

            if (pieceOnBoard != nullptr){
                if (pieceOnBoard->getTeam() != piece->getTeam()){
                    legalMoves.push_back(Position{newRow, newCol}); // Capture can be added to legal moves
                    break;
                }
                else{
                    break;
                }
            }

            Position newPosition{newRow, newCol};
            legalMoves.push_back(newPosition);
        }
    }

    return legalMoves;
}

void ChessBoard::changePlayerTurn() {
    currentTurn = currentTurn == Piece::WHITE ? Piece::BLACK : Piece::WHITE;
}

bool ChessBoard::checkForEnPassantPawns(Piece* initPawn) {
    Position initPos = initPawn->getPosition();
    Piece::Team initTeam = initPawn->getTeam();

    int x = initPos.xCoord;
    int y = initPos.yCoord;

    Piece* rightPiece = nullptr;
    Piece* leftPiece = nullptr;
    if ((y == 3 && initTeam == Piece::WHITE) || (y==4 && initTeam == Piece::BLACK)){
        if (x == 0){
            // Check only right piece
            rightPiece = boardArr[x+1][y];
        }
        else if (x == 7){
            leftPiece = boardArr[x-1][y];
        }
        else{
            leftPiece = boardArr[x-1][y];
            rightPiece = boardArr[x+1][y];
        }
        Piece* tempArr[2] = {leftPiece, rightPiece};

        for (int i = 0; i < 2; i++){
            Piece* p = tempArr[i];
            if (p != nullptr){
                // Check if the piece on the side is a pawn of the opposite team
                if (p->getPieceType() == Piece::PAWN && p->getTeam() != initTeam){
                    Pawn* sidePawn = dynamic_cast<Pawn*>(p);
                    if (i == 0){
                        sidePawn->activateEnPassant(RIGHT_EN_PASSANT);
                    }
                    else{
                        sidePawn->activateEnPassant(LEFT_EN_PASSANT);
                    }
                    return true;
                }
            }
        }
        return false;
    }
}

bool ChessBoard::checkForChecks(Piece::Team pTeam) {
    std::vector<Piece*> v;
    King* king;

    if (pTeam == Piece::WHITE){
        v = blackPieces;
        king = whiteKing;
    }
    else{
        v = whitePieces;
        king = blackKing;
    }
    for (Piece* ennemyPiece: v){
        std::vector<Position> pseudoMoves = calculateLegalMoves(ennemyPiece);

        auto it = std::find(pseudoMoves.begin(), pseudoMoves.end(), king->getPosition());

        if (it != pseudoMoves.end()){
            return true;
        }
    }

    return false;
}

void ChessBoard::addPieceToPieceList(Piece* piece) {
    if (piece->getTeam() == Piece::WHITE){
        whitePieces.push_back(piece);
    }
    else{
        blackPieces.push_back(piece);
    }
}

void ChessBoard::capturePiece(Piece * capturedPiece) {
    if (capturedPiece->getTeam() == Piece::WHITE){
        for (int i = 0; i < whitePieces.size(); i++){
            if (whitePieces[i] == capturedPiece){
                whitePieces.erase(whitePieces.begin() + i);
                break;
            }
        }
    }
    else {
        for (int i = 0; i < blackPieces.size(); i++){
            if (blackPieces[i] == capturedPiece){
                blackPieces.erase(blackPieces.begin() + i);
                break;
            }
        }
    }

    delete capturedPiece;
}
