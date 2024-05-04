#include "Board.h"
#include "SDL_video.h"

Board::Board() 
    : cellSize(30), boardWidth(10), boardHeight(20), boardPositionX(0), boardPositionY(0) {

    for (int i = 0; i < 2; ++i) {
        textures[i] = nullptr; 
    }

    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            boardTexture[i][j] = nullptr;
            boardData[i][j] = false;
        }
    }
}

Board::~Board() {
    for (int i = 0; i < 2; ++i) {
        SDL_DestroyTexture(textures[i]);
    }

    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            SDL_DestroyTexture(boardTexture[i][j]);
        }
    }

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
    boardData[x][y] = true;
}

void Board::render(SDL_Renderer* renderer) {
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            if (boardTexture[i][j]) {
                SDL_Rect textureRect{};

                textureRect.w = cellSize;
                textureRect.h = cellSize;

                textureRect.x = boardPositionX + (i * textureRect.w);
                textureRect.y = boardPositionY + (j * textureRect.h);

                SDL_RenderCopy(renderer, boardTexture[i][j], NULL, &textureRect);

            }
        }
    }

}