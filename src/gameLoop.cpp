#include "gameLoop.h"
#include <memory>
#include <iostream>

gameLoop::gameLoop()
: running{true}, firstClickOnPiece{false}, pieceAtFirstPos{nullptr}{
    chessBoard = std::make_unique<ChessBoard>(&handler);
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
            }
            else if (handler.event.type == SDL_KEYUP){
                // If 'f' is pressed, flip the board
                if (handler.event.key.keysym.sym == 102){
                    chessBoard->flipBoardOrientation();
                    std::cout << "The board has been flipped" << std::endl;
                }
            }
            // Clear the renderer then render all the pieces after every frame
            renderGameElements();
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
                v = chessBoard->calculateLegalMoves(pieceAtFirstPos);

                firstClickOnPiece = true;
            }
        }
    }
    else{
        // Check if the move is contained in the list of possible moves for the piece
        if (chessBoard->canMoveTo(v, piecePos)){
            chessBoard->moveTo(pieceAtFirstPos, piecePos);

            if (chessBoard->getCurrentTurn() == Piece::WHITE){
                turnNumber++;
            }

            std::cout << "Turn: " << turnNumber << " " << chessBoard->getFENFromPos() << std::endl;
        }
        //chessBoard->flipBoardOrientation();
        firstClickOnPiece = false;
    }
}


