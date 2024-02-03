#include <iostream>
#include "sdlHandler.h"

bool SDLHandler::init() {
    // Verify if  SDL could initialize properly. If not, close the program.
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "SDL could not initialize. SDL_Error: " << SDL_GetError();
        clearSDL();
        return false;
    }
    // Verify if SDL_image could initialize properly.
    else if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
        std::cout << "SDL_image could not initialize. SDL_Image error: " << IMG_GetError();
        clearSDL();
        return false;
    }
    else{
        // Create a window.
        window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
        if (window == nullptr){
            std::cout << "Window could not be created. SDL_Error: " << SDL_GetError();
        }
        else{
            // Create the renderer
            renderer = SDL_CreateRenderer(window, -1, 0);
            // Set blend mode to enable alpha blending with RenderDraw functions
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        }
    }
    return true;
}

void SDLHandler::drawSquare(int x, int y) const {
    // When index start at 0, white squares happen when the sum of x and y is pair.
    if ((x+y)%2 == 0) {
        SDL_SetRenderDrawColor(renderer, 238, 238, 210, 255);
    }
    else{
        SDL_SetRenderDrawColor(renderer, 118, 150, 86, 255);
    }
    // Create a SDL_Rect rectangle object to draw.
    SDL_Rect rect;
    // SCREEN_W and SCREEN_H will be changed once the app is responsive.
    rect.x = x * SCREEN_W/8;
    rect.y = y * SCREEN_H/8;
    rect.w = SCREEN_W/8;
    rect.h = SCREEN_H/8;

    SDL_RenderFillRect(renderer, &rect);
}

void SDLHandler::drawBoard() const{

    for (int y = 0; y < 8; y++){
        for (int x = 0; x < 8; x++){
            drawSquare(x, y);
        }
    }
}

void SDLHandler::drawDarkerSquare(int x, int y, int color) const {
    if (color == 0){
        SDL_SetRenderDrawColor(renderer, 252, 141, 104, 200);
    }
    else if (color == 1){
        SDL_SetRenderDrawColor(renderer, 52, 201, 235, 100);
    }

    SDL_Rect rectDark;
    rectDark.x = x * SCREEN_W/8;
    rectDark.y = y * SCREEN_H/8;
    rectDark.w = SCREEN_W/8;
    rectDark.h = SCREEN_H/8;

    SDL_RenderFillRect(renderer, &rectDark);
}


void SDLHandler::clearSDL() const {
    SDL_FreeSurface(screenSurface);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

SDLHandler::SDLHandler() {
    SDLHandler::init();
}

SDLHandler::~SDLHandler() {
    clearSDL();
}

void SDLHandler::renderFrame() const {
    SDL_RenderPresent(renderer);
}

SDL_Texture *SDLHandler::loadImageFromPng(const std::string& fileName) const{
    SDL_Surface* loadedImage;
    loadedImage = IMG_Load(fileName.c_str());

    if (loadedImage == nullptr){
        std::cout << "Could not load texture from file: " << fileName << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
    return texture;
}

void SDLHandler::renderPiece(SDL_Rect rectContainer, SDL_Texture *texture) const {
    if (texture != nullptr)
    {
        SDL_RenderCopy(renderer, texture, nullptr, &rectContainer);
    }
    else
    {
        std::cout << "DrawRectangle: texture was nullptr" << std::endl;
    }
}

Position SDLHandler::getPositionOnScreen() const {
    int xPos = event.button.x / (SCREEN_W/8);
    int yPos = event.button.y / (SCREEN_H/8);

//    std::cout << xPos << " " << yPos << std::endl;

    Position pos {xPos, yPos};
    return pos;
}

