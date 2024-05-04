#include "Board.h"
#include "SDL_video.h"

Board::Board() {
    for (int i = 0; i < 2; ++i) {
        textures[i] = nullptr; 
    }

    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            boardTexture[i][j] = nullptr;
        }
    }

    boardPositionX = 0;
    boardPositionY = 0;
}

Board::~Board() {

}

void Board::init(SDL_Renderer* renderer, int posX, int posY) {
    textures[0] = IMG_LoadTexture(renderer, "./assets/EmptyBlock.png");
    textures[1] = IMG_LoadTexture(renderer, "./assets/RedBlock.png");

    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            boardTexture[i][j] = textures[0];
        }
    }

    boardPositionX = posX;
    boardPositionY = posY;
}

void Board::setBlockTexture(int x, int y, int textureindex) {
    boardTexture[x][y] = textures[textureindex];
}

void Board::render(SDL_Renderer* renderer) {
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            if (boardTexture[i][j]) {
                SDL_Rect textureRect{};

                textureRect.w = cellWidth;
                textureRect.h = cellHeight;

                textureRect.x = boardPositionX + (i * textureRect.w);
                textureRect.y = boardPositionY + (j * textureRect.h);

                SDL_RenderCopy(renderer, boardTexture[i][j], NULL, &textureRect);

            }
        }
    }

}