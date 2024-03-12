#pragma once
#include "sdlHandler.h"
#include "piece.h"

class PromotionWindow {
private:
    SDLHandler* handler;
    Position containerTopCornerPos;
    SDL_Rect container;

    SDL_Texture* textureArr[4];

    int widthPercent = 70;
    int heightPerecent = 25;
public:
    PromotionWindow(SDLHandler*, Piece::Team);
    void displaySelectionScreen();
    Position getMouseClickCoords();
    int getWindowWidth();
};
