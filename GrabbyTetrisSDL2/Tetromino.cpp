#include "Tetromino.h"
#include "Board.h"
#include <cstring>

Tetromino::Tetromino(SDL_Renderer* renderer, TetrominoType shape, int positionx, int positiony, int tilesize)
    : x(3), y(0), rotationState(0), texture(nullptr), shape(shape), isPlaced(false), lockPhase(0), offsetx(positionx), offsety(positiony), tilesize(tilesize) {

    tilesize = tilesize;

    memcpy(matrix, tetrominoShapes[shape][rotationState], sizeof(bool) * 4 * 4);

    switch (shape) {
    case I:
        texture = IMG_LoadTexture(renderer, "./assets/TealBlock.png");
        break;
    case O:
        texture = IMG_LoadTexture(renderer, "./assets/YellowBlock.png");
        break;
    case T:
        texture = IMG_LoadTexture(renderer, "./assets/PurpleBlock.png");
        break;
    case L:
        texture = IMG_LoadTexture(renderer, "./assets/OrangeBlock.png");
        break;
    case J:
        texture = IMG_LoadTexture(renderer, "./assets/BlueBlock.png");
        break;
    case S:
        texture = IMG_LoadTexture(renderer, "./assets/GreenBlock.png");
        break;
    case Z:
        texture = IMG_LoadTexture(renderer, "./assets/RedBlock.png");
        break;
    }

    ghostTexture = IMG_LoadTexture(renderer, "./assets/GhostBlock.png");;


    memcpy(matrix, tetrominoShapes[shape][0], sizeof(bool) * 4 * 4);
}

Tetromino::~Tetromino() {
    if (texture) {
        SDL_DestroyTexture(texture);
        SDL_DestroyTexture(ghostTexture);
    }
}

void Tetromino::rotate(bool clockwise, Board& board) {
    int oldrotationstate = rotationState;

    if (clockwise) {
        rotationState = (rotationState + 1) % 4;
    }
    else {
        rotationState = (rotationState - 1 + 4) % 4;
    }

    int transition = transitionTable[oldrotationstate][clockwise ? 0 : 1];

    memcpy(matrix, tetrominoShapes[shape][rotationState], sizeof(bool) * 4 * 4);

    for (int i = 0; i < 5; i++) {

        int dx = (shape == I) ? srsKickI[transition][i][0] : srsKickNormal[transition][i][0];
        int dy = (shape == I) ? srsKickI[transition][i][1] : srsKickNormal[transition][i][1];

        x += dx;
        y += dy;

        if (board.checkCollision(*this)) {
            x -= dx;
            y -= dy;
            continue;
        }
        else {
            memcpy(matrix, tetrominoShapes[shape][rotationState], sizeof(bool) * 4 * 4);
            return;
        }

    }

    rotationState = oldrotationstate;
    memcpy(matrix, tetrominoShapes[shape][rotationState], sizeof(bool) * 4 * 4);

}

void Tetromino::move(int dx, int dy, Board& board) {
    x += dx;
    y += dy;

    uint32_t time = 0;

    if (board.checkCollision(*this)) {
        x -= dx;
        y -= dy;

        if (dy > 0) {
            if (lockPhase == 0) {
                lockPhase = SDL_GetTicks();
            }
            if (SDL_GetTicks() - lockPhase >= 500) {
                lockPhase = 0;
                board.placeBlock(*this);
                isPlaced = true;
            }
        }
    }
}

void Tetromino::hardDrop(Board& board) {
    while (!board.checkCollision(*this)) {
        y += 1;
    }
    y -= 1;
    board.placeBlock(*this);
    isPlaced = true;
}

int Tetromino::getX() const {
    return x;
}

int Tetromino::getY() const {
    return y;
}

void Tetromino::render(SDL_Renderer* renderer) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (matrix[i][j]) {
                SDL_Rect rect{};
                rect.x = offsetx + (x + j) * tilesize;
                rect.y = offsety + (y + i) * tilesize;
                rect.w = tilesize;
                rect.h = tilesize;
                
                SDL_RenderCopy(renderer, texture, nullptr, &rect);
            }
        }
    }
}

void Tetromino::renderGhostPiece(SDL_Renderer* renderer, Board& board) {
    const auto boardCollision = board.getBoardData();
    int ghostPositionX = getX();
    int ghostPositionY = getY();
    bool stopGhost = false;
    
    while (!stopGhost) {

        ghostPositionY++;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (matrix[i][j]) {
                    int newGhostPositionX = ghostPositionX + j;
                    int newGhostPositionY = ghostPositionY + i;

                    if (newGhostPositionY >= 20) {
                        ghostPositionY--;
                        stopGhost = true;
                        break;
                    }

                    if (boardCollision[newGhostPositionX][newGhostPositionY]) {
                        ghostPositionY--;
                        stopGhost = true;
                        break;
                    }
                }
            }
        }
    }


    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (matrix[i][j]) {
                SDL_Rect rect{};
                rect.x = offsetx + (ghostPositionX + j) * tilesize;
                rect.y = offsety + (ghostPositionY + i) * tilesize;
                rect.w = tilesize;
                rect.h = tilesize;

                SDL_RenderCopy(renderer, ghostTexture, nullptr, &rect);
            }
        }
    }
}