#include "gameLoop.h"
#include "promotionWindow.h"
#include <memory>
#include <iostream>
#include <unordered_set>
#include <sstream>

#define QUEEN_SIDE 0
#define KING_SIDE 1

gameLoop::gameLoop()
: running{true}, firstClickOnPiece{false}, pieceAtFirstPos{nullptr}{
//    chessBoard = std::make_unique<ChessBoard>(&handler);
    chessBoard = std::make_unique<ChessBoard>(&handler, "r3k2r/8/8/8/8/8/8/R3K2R w");

    // Add initial position of the board to the positionCountMap
    std::istringstream ss(chessBoard->getFENFromPos());
    std::string FENPart;
    ss >> FENPart;
    positionCountMap[FENPart] = 1;
}

gameLoop::gameLoop(const std::string& FEN)
        : running{true}, firstClickOnPiece{false}, pieceAtFirstPos{nullptr}{
    chessBoard = std::make_unique<ChessBoard>(&handler, FEN);
}

void gameLoop::run() {

    // Initial rendering
    renderGameElements();

    while (running) {
        // SDL_WaitEvent() pauses cpu execution until the next event is recorded.
        while (SDL_WaitEvent(&handler.event)) {
            if (pawnIsAtPromotionSquare){
                goto pawnPromotionScreen;
            }

            mainGameLoop:
            if (handler.event.type == SDL_QUIT) {
                return;
            }
            else if (handler.event.type == SDL_MOUSEBUTTONDOWN){
                mouseDownEvent();

                if (isCheckMate()){
                    std::string winningTeam = chessBoard->getCurrentTurn() == Piece::WHITE ? "Black" : "White";
                    std::cout << winningTeam + " has won." << std::endl;
                    running = false;
                    goto gameEndScreen;
                }
                if (isDraw()){
                    std::cout << "The Game Ended in a draw" << std::endl;
                    running = false;
                    goto gameEndScreen;
                }
            }
            else if (handler.event.type == SDL_KEYUP){
                // If 'f' is pressed, flip the board
                if (handler.event.key.keysym.sym == 102){
                    chessBoard->flipBoardOrientation();
                    std::cout << "The board has been flipped" << std::endl;
                }
                // If 'D' is pressed ask for draw by agreement
                else if (handler.event.key.keysym.sym == 100){
                    if (!userRequestsDraw){
                        userRequestsDraw = true;
                        std::cout << "Would you like to accept the draw? Press D to accept the draw or X to refuse it (1/2)" << std::endl;
                    }
                    else {
                        std::cout << "The Game Ended in a draw by agreement" << std::endl;
                        running = false;
                        goto gameEndScreen;
                    }
                }
                // If 'R' is pressed, then ask for confirmation of resignation
                else if (handler.event.key.keysym.sym == 114){
                    if (!userRequestResignation){
                        std::cout << "Press R again to confirm your resignation. Press X to cancel." << std::endl;
                        userRequestResignation = true;
                    }
                    else{
                        std::string winningTeam = chessBoard->getCurrentTurn() == Piece::WHITE ? "Black" : "White";
                        std::cout << winningTeam + " has won by resignation." << std::endl;
                        running = false;
                        goto gameEndScreen;
                    }
                }
                // If X is pressed, cancel draw or resignation request
                else if (handler.event.key.keysym.sym == 120){
                    userRequestsDraw = false;
                    userRequestResignation = false;
                }
            }
            renderGameElements();
        }
    }



    gameEndScreen:
    // Render all game elements one last time
    // Quit only when user chooses to exit the application
    renderGameElements();
    while (SDL_WaitEvent(&handler.event)){
        if (handler.event.type == SDL_QUIT){
            return;
        }
    }

    pawnPromotionScreen:
    Piece::Team promotionTeam;
    if (chessBoard->getCurrentTurn() == Piece::WHITE){
        promotionTeam = Piece::BLACK;
    }
    else{
        promotionTeam = Piece::WHITE;
    }
    PromotionWindow promotionWindow(&handler, promotionTeam);

    handler.drawBoard();
    promotionWindow.displaySelectionScreen();
    while (SDL_WaitEvent(&handler.event)){
        if (handler.event.type == SDL_QUIT){
            return;
        }
        if (handler.event.type == SDL_MOUSEBUTTONDOWN){
            Position mousePos = promotionWindow.getMouseClickCoords();

            std::cout << mousePos.xCoord << " " << mousePos.yCoord << std::endl;

            if (mousePos.xCoord != -1){
                Piece* promotionPiece;

                int selection = mousePos.xCoord/(promotionWindow.getWindowWidth()/4);
                switch (selection) {
                    case (0):
                        std::cout << "Queen" << std::endl;
                        promotionPiece = new Queen(promotionTeam, pieceAtFirstPos->getPosition(), &handler);
                        break;
                    case (1):
                        std::cout << "Knight" << std::endl;
                        promotionPiece = new Knight(promotionTeam, pieceAtFirstPos->getPosition(), &handler);
                        break;
                    case (2):
                        std::cout << "Rook" << std::endl;
                        promotionPiece = new Rook(promotionTeam, pieceAtFirstPos->getPosition(), &handler);
                        break;
                    case (3):
                        std::cout << "Bishop" << std::endl;
                        promotionPiece = new Bishop(promotionTeam, pieceAtFirstPos->getPosition(), &handler);
                        break;
                    default:
                        std::cout << "The value is unnexpected" << std::endl;
                        continue;
                }
                chessBoard->removeFromPieceList(pieceAtFirstPos);
                chessBoard->addToPieceList(promotionPiece);
                chessBoard->setPieceAtCoord(promotionPiece, pieceAtFirstPos->getPosition());

                delete pieceAtFirstPos;

                break;

            }
        }
        handler.drawBoard();
        promotionWindow.displaySelectionScreen();
    }

    pawnIsAtPromotionSquare = false;
    renderGameElements();
    handleFenAndChecks();
    lastPawnMoveOrCapture = turnNumber;
    firstClickOnPiece = false;

    goto mainGameLoop;
}

void gameLoop::renderGameElements() {
    SDL_RenderClear(handler.renderer);
    handler.drawBoard();
    if (firstClickOnPiece && !pawnIsAtPromotionSquare){
        handler.drawDarkerSquare(squarePos.xCoord, squarePos.yCoord);
        chessBoard->renderAllPossibleMoves(v);
    }
    chessBoard->renderAllPieces();
    handler.renderFrame();
}

void gameLoop::mouseDownEvent() {
    Position piecePos = chessBoard->getPositionOfPieceWOrientation();
    squarePos = handler.getPositionOnScreen();

    if (!firstClickOnPiece){
        if (!chessBoard->isSquareEmpty(piecePos)){
            pieceAtFirstPos = chessBoard->getPieceAtCoord(piecePos);

            // Check if the piece is of the right team
            if (pieceAtFirstPos->getTeam() == chessBoard->getCurrentTurn()){
                v = calculateLegalMovesWithRespectToChecks(pieceAtFirstPos);

                firstClickOnPiece = true;
            }
        }
    }
    else{
        // Check if the move is contained in the list of possible moves for the piece
        if (chessBoard->canMoveTo(v, piecePos)){
            Piece* pieceOnSquare = chessBoard->getPieceAtCoord(piecePos);

            if (pieceAtFirstPos->getPieceType() == Piece::KING) {
                Position kingPos = pieceAtFirstPos->getPosition();

                if (abs(piecePos.xCoord - kingPos.xCoord) == 2){
                    Piece* rook;
                    Position rookPos{};
                    if (piecePos.xCoord == 6){
                        rookPos = {7, kingPos.yCoord};
                        rook = chessBoard->getPieceAtCoord(rookPos);
                        rookPos.xCoord = 5;
                        chessBoard->moveTo(rook, rookPos, false);
                    }
                    else if (piecePos.xCoord == 2){
                        rookPos = {0, kingPos.yCoord};
                        rook = chessBoard->getPieceAtCoord(rookPos);
                        rookPos.xCoord = 3;
                        chessBoard->moveTo(rook, rookPos, false);
                    }
                }
            }

            chessBoard->moveTo(pieceAtFirstPos, piecePos);
            userRequestsDraw = false; // If the user plays a move after requesting a draw, cancel the draw request

            if (pieceAtFirstPos->getPieceType() == Piece::PAWN){
                Pawn* pushedPawn = dynamic_cast<Pawn*>(pieceAtFirstPos);
                if (pushedPawn->isOnPromotionRank()){
                    pawnIsAtPromotionSquare = true;
                    return;
                }
            }

            handleFenAndChecks();

            // Checks if the move is either a pawn move or a capture to implement 50-move draw
            if (pieceOnSquare != nullptr || pieceAtFirstPos->getPieceType() == Piece::PAWN){
                lastPawnMoveOrCapture = turnNumber;
            }
            std::cout << "Turn: " << turnNumber << " " << chessBoard->getFENFromPos() << std::endl;
            std::cout << "LastPawnMoveOrCapture: " << lastPawnMoveOrCapture << std::endl;
        }
        //chessBoard->flipBoardOrientation();
        firstClickOnPiece = false;
    }
}

std::vector<Position> gameLoop::calculateLegalMovesWithRespectToChecks(Piece* piece) {
    std::vector<Position> legalMovesBeforeVerif = chessBoard->calculateLegalMoves(piece);
    std::vector<Position> legalMovesAfterVerif;

    for (Position p: legalMovesBeforeVerif) {
        if (simulateMoveAndCheckForCheck(piece, p)){
            legalMovesAfterVerif.push_back(p);
        }
    }

    if (piece->getPieceType() == Piece::KING){
        King* king = dynamic_cast<King*>(piece);
        Position castlePos = king->getPosition();

        if (chessBoard->canCastle(king, QUEEN_SIDE)){
            castlePos.xCoord = 2;
            legalMovesAfterVerif.push_back(castlePos);
        }
        if (chessBoard->canCastle(king, KING_SIDE)){
            castlePos.xCoord = 6;
            legalMovesAfterVerif.push_back(castlePos);
        }
    }

    return legalMovesAfterVerif;
}


bool gameLoop::simulateMoveAndCheckForCheck(Piece *piece, Position p) {
    bool returnValue = false;

    Position initialPiecePos = piece->getPosition();
    Piece* tempPiece = chessBoard->getPieceAtCoord(p);

    if (tempPiece != nullptr){
        chessBoard->removeFromPieceList(tempPiece);

        chessBoard->setPieceAtCoord(piece, p);
        chessBoard->setPieceAtCoord(nullptr, initialPiecePos);

        piece->setPosition(p);

        returnValue = !chessBoard->checkForChecks(piece->getTeam());


        chessBoard->setPieceAtCoord(piece, initialPiecePos);
        piece->setPosition(initialPiecePos);
        chessBoard->setPieceAtCoord(tempPiece, p);

        chessBoard->addToPieceList(tempPiece);
    }
    else if(piece->getPieceType() == Piece::PAWN){
        Pawn* pawn = dynamic_cast<Pawn*>(piece);
        Position enPassant = pawn->getEnPassantMove();

        if (p == enPassant){
            int y = pawn->getTeam() == Piece::WHITE ? 4 : 3;
            Position tempPos {p.xCoord, y};
            tempPiece = chessBoard->getPieceAtCoord(tempPos);

            chessBoard->removeFromPieceList(tempPiece);
            chessBoard->setPieceAtCoord(piece, p);
            chessBoard->setPieceAtCoord(nullptr, initialPiecePos);

            piece->setPosition(p);

            returnValue = !chessBoard->checkForChecks(piece->getTeam());

            chessBoard->setPieceAtCoord(piece, initialPiecePos);
            piece->setPosition(initialPiecePos);
            chessBoard->setPieceAtCoord(tempPiece, tempPos);
            chessBoard->setPieceAtCoord(nullptr, p);

            chessBoard->addToPieceList(tempPiece);
        }
        else{
            chessBoard->setPieceAtCoord(piece, p);
            chessBoard->setPieceAtCoord(nullptr, initialPiecePos);
            piece->setPosition(p);

            returnValue = !chessBoard->checkForChecks(piece->getTeam());

            piece->setPosition(initialPiecePos);
            chessBoard->setPieceAtCoord(nullptr, p);
            chessBoard->setPieceAtCoord(piece, initialPiecePos);
        }
    }
    else{
        chessBoard->moveTo(piece, p, false);

        returnValue = !chessBoard->checkForChecks(piece->getTeam());

        chessBoard->moveTo(piece, initialPiecePos, false);
    }

    return returnValue;
}

bool gameLoop::isDraw() {
    // These three lines of code are just to handle trimming the FEN
    // to keep only the position part of the string
    std::istringstream ss(chessBoard->getFENFromPos());
    std::string FENPart;
    ss >> FENPart;

    return isStalemate() || isFiftyMoveRule() || isThreeFoldRepetition(FENPart)
            || isDeadPosition();
}

bool gameLoop::isThreeFoldRepetition(const std::string& FEN) {
    return positionCountMap[FEN] >= 3;
}

bool gameLoop::isFiftyMoveRule() const {
    return (turnNumber - lastPawnMoveOrCapture)>=50;
}

bool gameLoop::isStalemate() {
    Piece::Team currentTeam = chessBoard->getCurrentTurn();
    std::vector<Piece*> teamPieces = chessBoard->getPieceList(currentTeam);

    // Check all possible moves for every piece on the current player's turn
    for (Piece* p : teamPieces){
        std::vector<Position> legalMoves = calculateLegalMovesWithRespectToChecks(p);
        // If move is possible then it is not stalemate.
        if (!legalMoves.empty()){
            return false;
        }
    }
    return true;
}

bool gameLoop::isCheckMate() {
    Piece::Team team = chessBoard->getCurrentTurn();
    King* king = chessBoard->getKing(team);

    // This works because stalemate checks if there are no legal moves left
    // Plus, if the king is in check and there are no legal moves, then the game ends with checkmate
    return king->isInCheck() && isStalemate();
}

bool gameLoop::isDeadPosition() {
    std::vector<Piece*> blackPieces = chessBoard->getPieceList(Piece::BLACK);
    std::vector<Piece*> whitePieces = chessBoard->getPieceList(Piece::WHITE);

    // Return true if the only piece in the piece list is the kings
    if (blackPieces.size() + whitePieces.size() == 2){
        return true;
    }

    // Checking for other dead position criteria
    int blackKnight = 0, whiteKnight = 0, blackBishops = 0, whiteBishops = 0,
            blackWhiteBishop = 0, blackBlackBishop = 0, whiteBlackBishop = 0,
            whiteWhiteBishop = 0;

    // Process black piece list
    for (Piece* piece : blackPieces){
        if (piece->getPieceType() == Piece::KNIGHT){
            blackKnight++;
            if (blackKnight >= 2){
                return false;
            }
        }
        else if (piece->getPieceType() == Piece::BISHOP){
            blackBishops++;
            if (blackBishops >= 2){
                return false;
            }
            Position p = piece->getPosition();
            // Due to the way I have set up the coordinates system if x + y is even
            // then the bishop is on a dark square
            if ((p.xCoord + p.yCoord) % 2 == 0){
                blackBlackBishop++;
            }
            else{
                blackWhiteBishop++;
            }
        }
        else{
            // If piece is not a king, return false since bishop and knight have already been processed
            if (piece->getPieceType() != Piece::KING){
                return false;
            }
        }
    }

    // Process white piece list
    for (Piece* piece: whitePieces){
        if (piece->getPieceType() == Piece::KNIGHT){
            whiteKnight++;
            if (whiteKnight >= 2){
                return false;
            }
        }
        else if (piece->getPieceType() == Piece::BISHOP){
            whiteBishops++;
            if (whiteBishops >= 2){
                return false;
            }
            Position p = piece->getPosition();
            // Due to the way I have set up the coordinates system if x + y is even
            // then the bishop is on a dark square
            if ((p.xCoord + p.yCoord) % 2 == 0){
                whiteBlackBishop++;
            }
            else{
                whiteWhiteBishop++;
            }
        }
        else{
            // If piece is not a king, return false since bishop and knight have already been processed
            if (piece->getPieceType() != Piece::KING){
                return false;
            }
        }
    }
    // When the function reaches this point, the only possible
    // pieces remaining are the kings, the bishops and the knights

    // Checks for king vs king + bishop
    bool kingAndBishopVsKing = abs(blackBishops - whiteBishops) == 1 &&
                                (whiteKnight + blackKnight == 0);
    // Checks for king + bishop vs king + bishop of the same color
    bool bishopVsSameColor = (whiteWhiteBishop + blackWhiteBishop == 2) ||
                                (whiteBlackBishop + blackBlackBishop == 2);
    // Checks for king vs king + knight
    bool kingAndKnightVsKing = abs(blackKnight - whiteKnight) == 1 &&
                                (whiteBishops + blackBishops == 0);

    return kingAndBishopVsKing || bishopVsSameColor || kingAndKnightVsKing;
}

void gameLoop::handleFenAndChecks() {
    // These three lines of code are just to handle trimming the FEN
    // to keep only the position part of the string
    std::istringstream ss(chessBoard->getFENFromPos());
    std::string FENPart;
    ss >> FENPart;
    positionCountMap[FENPart]++; // Add the FEN of the position to the positionCountMap

    if (chessBoard->checkForChecks(Piece::WHITE)){
        std::cout << "White is in check" << std::endl;
    }
    if (chessBoard->checkForChecks(Piece::BLACK)){
        std::cout << "Black is in check" << std::endl;
    }

    if (chessBoard->getCurrentTurn() == Piece::WHITE){
        turnNumber++;
    }
}
