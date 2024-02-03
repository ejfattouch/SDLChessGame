#include "gameLoop.h"
#include <memory>
#include <iostream>

void gameLoop::run() {
    SDLHandler handler;
    // Initial rendering of the board
    handler.drawBoard();
    // Initializes the board with the default starting position
    std::unique_ptr<ChessBoard> chessBoard = std::make_unique<ChessBoard>(&handler);
    chessBoard->renderAllPieces();
    handler.renderFrame();

    bool running = true;
    bool firstClickOnPiece = false;

    Piece* pieceAtFirstPos = nullptr;
    Position squarePos {0, 0};

    std::vector<Position> v = {};
    while (running) {
        // SDL_WaitEvent() pauses cpu execution until the next event is recorded.
        while (SDL_WaitEvent(&handler.event)) {
            if (handler.event.type == SDL_QUIT) {
                running = false;
                break;
            }
            else if (handler.event.type == SDL_MOUSEBUTTONDOWN){
                Position piecePos = chessBoard->getPositionOfPieceWOrientation();
                squarePos = handler.getPositionOnScreen();

                if (!firstClickOnPiece){
                    if (!chessBoard->isSquareEmpty(piecePos)){
                        pieceAtFirstPos = chessBoard->getPieceAtCoord(piecePos);

                        v = chessBoard->calculateLegalMoves(pieceAtFirstPos);

                        firstClickOnPiece = true;
                    }
                }
                else{
                    // Check if the move is contained in the list of possible moves for the piece
                    if (chessBoard->canMoveTo(v, piecePos)){
                        chessBoard->moveTo(pieceAtFirstPos, piecePos);
                    }
//                    chessBoard->flipBoardOrientation();
                    firstClickOnPiece = false;
                }
            }
            else if (handler.event.type == SDL_KEYUP){
                // If 'f' is pressed, flip the board
                if (handler.event.key.keysym.sym == 102){
                    chessBoard->flipBoardOrientation();
                    std::cout << "The board has been flipped" << std::endl;
                }
            }
            // Clear the renderer then render all the pieces after every frame
            SDL_RenderClear(handler.renderer);
            handler.drawBoard();
            if (firstClickOnPiece){
                handler.drawDarkerSquare(squarePos.xCoord, squarePos.yCoord);
                chessBoard->renderAllPossibleMoves(v);
            }
            chessBoard->renderAllPieces();
            handler.renderFrame();
        }
    }
}