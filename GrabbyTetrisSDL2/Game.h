#pragma once
#include <SDL.h>
#include <stdlib.h>
#include <random>
#include <array>
#include "Tetromino.h"
#include "Board.h"
#include <chrono>
#include <iostream>
#include <algorithm>


class Game
{
public: 
    Game();
    ~Game();
    void init();

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() const { return isRunning; };

    TetrominoType getNextTetromino();

    void createNewTetromino(bool stored);

private: 
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    uint32_t  lastMoveDownTime;
    uint32_t  lastMoveInputTime;

    std::vector<TetrominoType> nextTetrominos;

    TetrominoType storedTetromino;

    bool canHardDrop;
    bool canRotate;
    bool canStore;

    Board* gameboard;
    Tetromino* currentTetromino;

    bool isRunning;
};

