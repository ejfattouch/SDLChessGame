//
// Created by Edward-Joseph Fattouch on 2024-03-11.
//

#include "promotionWindow.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "queen.h"

PromotionWindow::PromotionWindow(SDLHandler* handler, Piece::Team pTeam) : handler(handler) {
    containerTopCornerPos = {handler->SCREEN_W * (100-widthPercent)/200,
                           handler->SCREEN_H * (100-heightPerecent)/200};

    container = {containerTopCornerPos.xCoord,
                        containerTopCornerPos.yCoord,
                        handler->SCREEN_W * widthPercent/100,
                 handler->SCREEN_W * widthPercent/400
    };

    // Get the texture of all pieces
    Queen q = Queen(pTeam, {0,0}, handler);
    Knight k = Knight(pTeam, {0,0}, handler);
    Rook r = Rook(pTeam, {0,0}, handler);
    Bishop b = Bishop(pTeam, {0,0}, handler);

    textureArr[0] = q.getReferenceToTexture();
    textureArr[1] = k.getReferenceToTexture();
    textureArr[2] = r.getReferenceToTexture();
    textureArr[3] = b.getReferenceToTexture();
}

void PromotionWindow::displaySelectionScreen() {


    // Create new renderer for the opacity layer
    SDL_Renderer* renderer = handler->renderer;

    //Render the container for the promotion selection screen
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderFillRect(renderer, &container);


    // Iterate through textureArr
    for (int i = 0; i < 4; i++){
        SDL_Texture* texture = textureArr[i];

        int offset = 20;
        int dimensions = container.w /4 - offset;

        SDL_Rect pieceContainer = {container.x + (i * container.w/4) + offset/2,
                                   container.y + offset/2,
                                   dimensions,
                                   dimensions};
        handler->renderPiece(pieceContainer, texture);
    }
    SDL_RenderPresent(renderer);
}

Position PromotionWindow::getMouseClickCoords() {
    int xPos = handler->event.button.x - containerTopCornerPos.xCoord;
    int yPos = handler->event.button.y - containerTopCornerPos.yCoord;

    if (xPos < 0 || yPos < 0 || xPos > container.w || yPos > container.h){
        return {-1,-1};
    }
    return Position{xPos, yPos};
}

int PromotionWindow::getWindowWidth() {
    return container.w;
}
