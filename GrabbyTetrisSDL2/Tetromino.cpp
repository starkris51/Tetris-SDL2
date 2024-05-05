#include "Tetromino.h"
#include <cstring>

static const bool tetrominoShapes[7][4][4][4] = {
    //I Piece
    {
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0},
        },
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
        },
    },
    //O
    {
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0},
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0},
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0},
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0},
        },
    },
    //T PIece
    {
        {
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0},
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0},
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,1,0,0},
            {0,0,0,0},
        },
        {
            {0,1,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0},
        },
    },
    //L piece
    {
        {
            {0,0,1,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0},
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {1,0,0,0},
            {0,0,0,0},
        },
        {
            {1,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0},
        },
    },
    //J Piece
    {
        {
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0},
        },
        {
            {0,1,1,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0},
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,0,1,0},
            {0,0,0,0},
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,0,0,0},
        },
    },
    //S Piece 
    {
        {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0},
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0},
        },
        {
            {0,0,0,0},
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
        },
        {
            {1,0,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0},
        },
    },
    //Z Piece
    {
        {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},    
            {0,0,0,0},
        },
        {
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0},
        },
        {
            {0,0,0,0},
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
        },
        {
            {0,1,0,0},
            {1,1,0,0},
            {1,0,0,0},
            {0,0,0,0},
        },
    }
};

Tetromino::Tetromino(SDL_Renderer* renderer, TetrominoType shape)
    : x(3), y(0), rotationState(0), texture(nullptr), shape(shape) {

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

    memcpy(matrix, tetrominoShapes[shape][0], sizeof(bool) * 4 * 4);
}

Tetromino::~Tetromino() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Tetromino::rotate() {
    rotationState = (rotationState + 1) % 4;
    memcpy(matrix, tetrominoShapes[shape][rotationState], sizeof(bool) * 4 * 4);
}

void Tetromino::move(int dx, int dy) {
    x += dx;
    y += dy;
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
                rect.x = (x + j) * 30;
                rect.y = (y + i) * 30;
                rect.w = 30;
                rect.h = 30;

                SDL_RenderCopy(renderer, texture, nullptr, &rect);
            }
        }
    }
}