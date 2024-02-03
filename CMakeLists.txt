cmake_minimum_required(VERSION 3.22)
project(Chess)

set(CMAKE_CXX_STANDARD 17)

# Must set the path to the main.cpp, for example: src/main.cpp if it is inside a folder
add_executable(${PROJECT_NAME} src/main.cpp
        src/sdlHandler.cpp
        src/gameLoop.cpp
        src/chessBoard.cpp
        src/piece.cpp
        src/pawn.cpp
        src/rook.cpp
        src/bishop.cpp
        src/knight.cpp
        src/queen.cpp
        src/king.cpp)

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Chess
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(Chess PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
