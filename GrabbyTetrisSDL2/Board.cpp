#include "Board.h"

Board::Board() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            gameboard[i][j] = nullptr;
        }
    }
}

Board::~Board() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (gameboard[i][j]) {
                delete gameboard[i][j];
            }
        }
    }
}

void Board::boardinit() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (gameboard[i][j] == nullptr) {  

                gameboard[i][j] = new SDL_Rect();

                gameboard[i][j]->w = 25;
                gameboard[i][j]->h = 25;
                gameboard[i][j]->x = boardx + i * 25;
                gameboard[i][j]->y = boardy + j * 25;

            }
        }
    }
}

void Board::renderboard(SDL_Renderer* renderer) const {
    if (!renderer) return;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (!gameboard[i][j]) { return; }

            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderFillRect(renderer, gameboard[i][j]);

            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawRect(renderer, gameboard[i][j]);

        }
    }

}