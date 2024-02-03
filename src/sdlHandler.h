#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>

struct Position{
    int xCoord, yCoord;

    bool operator==(const Position& other) const{
        return xCoord == other.xCoord && yCoord == other.yCoord;
    }
};

class SDLHandler {
public:
    // Setting the default screen size.
    // TO-DO Make window resizable while keeping aspect ratio.
    int SCREEN_W = 720;
    int SCREEN_H = 720;

    // Create window object.
    SDL_Window* window = nullptr;
    // Create surface that will be drawn onto and contained by the window.
    SDL_Surface* screenSurface = nullptr;
    // Renderer
    SDL_Renderer* renderer;
    // Game events
    SDL_Event event;

    // Initializes SDL and renders a screen
    bool init();

    // Draw a single square at board coord x and y
    void drawSquare(int, int) const;
    // Draws the background for the board
    void drawBoard() const;
    // Draws a highlighted square
    void drawDarkerSquare(int, int, int color = 0) const;
    // Push frame to screen and clear renderFrame frame
    void renderFrame() const;
    // Frees up SDL resources after program termination
    void clearSDL() const;
    // Loads an PNG image into a SDL_Texture
    [[nodiscard]] SDL_Texture* loadImageFromPng(const std::string&) const;
    // Renders the piece texture inside the container rectangle
    void renderPiece(SDL_Rect rectContainer, SDL_Texture* texture) const;
    Position getPositionOnScreen() const;

    SDLHandler();
    ~SDLHandler();

};

