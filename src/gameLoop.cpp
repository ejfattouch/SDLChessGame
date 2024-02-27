#include "gameLoop.h"
#include <memory>
#include <iostream>

gameLoop::gameLoop()
: running{true}, firstClickOnPiece{false}, pieceAtFirstPos{nullptr}{
    chessBoard = std::make_unique<ChessBoard>(&handler);
//    chessBoard = std::make_unique<ChessBoard>(&handler, "2krr3/4b3/8/8/8/8/8/4K3 w");

}

void gameLoop::run() {

    // Initial rendering
    renderGameElements();

    while (running) {
        // SDL_WaitEvent() pauses cpu execution until the next event is recorded.
        while (SDL_WaitEvent(&handler.event)) {
            if (handler.event.type == SDL_QUIT) {
                running = false;
                break;
            }
            else if (handler.event.type == SDL_MOUSEBUTTONDOWN){
                mouseDownEvent();
                renderGameElements();
            }
            else if (handler.event.type == SDL_KEYUP){
                // If 'f' is pressed, flip the board
                if (handler.event.key.keysym.sym == 102){
                    chessBoard->flipBoardOrientation();
                    std::cout << "The board has been flipped" << std::endl;
                }
                renderGameElements();
            }
        }
    }
}

void gameLoop::renderGameElements() {
    SDL_RenderClear(handler.renderer);
    handler.drawBoard();
    if (firstClickOnPiece){
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
            chessBoard->moveTo(pieceAtFirstPos, piecePos);

            if (chessBoard->checkForChecks(Piece::WHITE)){
                std::cout << "White is in check" << std::endl;
            }
            if (chessBoard->checkForChecks(Piece::BLACK)){
                std::cout << "Black is in check" << std::endl;
            }


            if (chessBoard->getCurrentTurn() == Piece::WHITE){
                turnNumber++;
            }

            std::cout << "Turn: " << turnNumber << " " << chessBoard->getFENFromPos() << std::endl;
        }
        //chessBoard->flipBoardOrientation();
        firstClickOnPiece = false;
    }
}

std::vector<Position> gameLoop::calculateLegalMovesWithRespectToChecks(Piece* piece) {
    std::vector<Position> legalMovesBeforeVerif = chessBoard->calculateLegalMoves(piece);
    std::vector<Position> legalMovesAfterVerif;

    Piece::Team pTeam = piece->getTeam();

    bool initiallyInCheck = chessBoard->checkForChecks(pTeam);

    for (Position p: legalMovesBeforeVerif){
        ChessBoard boardCopy(*chessBoard); // Make a deep copy of the chessboard to simulate moves

        // Use copy of piece since in the copied board, the pointers point to different locations
        Piece* copyEquivalentPiece = boardCopy.getPieceAtCoord(piece->getPosition());
        boardCopy.moveTo(copyEquivalentPiece, p);

        bool currentlyInCheck = boardCopy.checkForChecks(pTeam);

        // This if statement is a band-aid fix
        if (piece->getPieceType() == Piece::KING){
            if (initiallyInCheck){
                if (currentlyInCheck){
                    legalMovesAfterVerif.push_back(p);
                }
                continue;
            }
        }

        if (initiallyInCheck && currentlyInCheck){
            Position initialPiecePos = piece->getPosition();
            Piece* tempPiece = chessBoard->getPieceAtCoord(p);

            if (tempPiece != nullptr){
                chessBoard->removeFromPieceList(tempPiece);

                chessBoard->setPieceAtCoord(piece, p);
                chessBoard->setPieceAtCoord(nullptr, initialPiecePos);

                piece->setPosition(p);

                if (!chessBoard->checkForChecks(piece->getTeam())){
                    legalMovesAfterVerif.push_back(p);
                }

                chessBoard->setPieceAtCoord(piece, initialPiecePos);
                piece->setPosition(initialPiecePos);
                chessBoard->setPieceAtCoord(tempPiece, p);

                chessBoard->addToPieceList(tempPiece);
            }
        }

        if (!boardCopy.checkForChecks(pTeam)){
            legalMovesAfterVerif.push_back(p);
        }
    }

    return legalMovesAfterVerif;
}