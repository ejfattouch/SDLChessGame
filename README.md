
# SDL2 Chess Clone

## Overview
This project is a simple Chess game implementation using the SDL2 library in C/C++. The goal is to create a visually appealing and functional Chess game that can be played on any platform supported by SDL2.



## Features

- Classic Chess gameplay following standard rules.
- User-friendly interface designed with SDL2 for easy interaction.
- Graphical representation of the Chess board and pieces.
- Legal move validation.
- Custom positions can be initalized by loading the FEN of the position.


## Dependencies
- SDL2 library
- SDL2_image library (for loading image assets)
- CMake (for building the project)
## Installation

1 - Ensure you have SDL2 and SDL2_image installed on your system.

2 - Clone the repository

```bash
git clone https://github.com/ejfattouch/SDLChessGame
```

3 - Navigate to the project directory: 
```bash
cd sdl2-chess-clone
```

4 - Create a build directory: 
```bash
mkdir build && cd build
```

5 - Generate build files with CMake: 
```bash
cmake ..
```
6 - Build the project: 
```bash
cmake --build .
```    
## Usage

1 - Run the executable generated in the `build` directory.



## Controls

- Click on a piece to select it.
- Click on a valid destination to move the selected piece.
- Press `R` twice to resign on your turn
- Press `F` to flip the board.
- Presing `D` initiates the draw sequence, press `D` again to accept the draw.
- To close the game just click on your OS's respective window exit button
## License

This project is licensed under the MIT License. See the `LICENSE` file for details.


## Acknowledgements

This project was inspired by the desire to learn SDL2 and practice C/C++ programming skills. Special thanks to the SDL2 development team for providing such a versatile library.


