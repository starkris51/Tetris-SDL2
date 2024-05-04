#include "Board.h"
#include "SDL_video.h"

Board::Board() 
    : cellSize(30), boardWidth(10), boardHeight(20), boardPositionX(0), boardPositionY(0) {

    for (int i = 0; i < 8; ++i) {
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
    for (int i = 0; i < 8; ++i) {
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
    textures[2] = IMG_LoadTexture(renderer, "./assets/YellowBlock.png");
    textures[3] = IMG_LoadTexture(renderer, "./assets/BlueBlock.png");
    textures[4] = IMG_LoadTexture(renderer, "./assets/GreenBlock.png");
    textures[5] = IMG_LoadTexture(renderer, "./assets/PurpleBlock.png");
    textures[6] = IMG_LoadTexture(renderer, "./assets/TealBlock.png");
    textures[7] = IMG_LoadTexture(renderer, "./assets/OrangeBlock.png");

    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            boardTexture[i][j] = textures[0];
        }
    }

    boardPositionX = posX;
    boardPositionY = posY;
}

bool Board::checkCollision(const bool(&matrix)[4][4], int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (matrix[i][j]) {
                int boardX = (x) + j;
                int boardY = (y) + i;

                if (boardData[boardX][boardY]) {
                    return true;
                }

                if (boardY < 0 || boardY >= boardHeight) {
                    return true;
                }

            }
        }
    }

    return false;
}

void Board::placeBlock(const bool(&matrix)[4][4], int x, int y, TetrominoType shape) {

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (matrix[i][j]) {
                int boardX = (x) + j;
                int boardY = (y) + i;

                if (boardX >= 0 && boardX < boardWidth && boardY >= 0 && boardY < boardHeight) {
                    boardData[boardX][boardY] = true;

                    switch (shape) {
                    case I:
                        boardTexture[boardX][boardY] = textures[6];
                        break;
                    case O:
                        boardTexture[boardX][boardY] = textures[2];
                        break;
                    case T:
                        boardTexture[boardX][boardY] = textures[5];
                        break;
                    case L:
                        boardTexture[boardX][boardY] = textures[7];
                        break;
                    case J:
                        boardTexture[boardX][boardY] = textures[3];
                        break;
                    case S:
                        boardTexture[boardX][boardY] = textures[4];
                        break;
                    case Z:
                        boardTexture[boardX][boardY] = textures[1];
                        break;
                    }
                } else {
                    std::cerr << "Out-of-bounds position: (" << boardX << ", " << boardY << ")" << std::endl;
                }

            }
        }
    }
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