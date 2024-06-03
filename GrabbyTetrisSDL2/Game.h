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
#include <unordered_map>

const uint32_t DAS_DELAY = 167;
const uint32_t DAS_REPEAT = 17;

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

    SDL_Texture* background;

    uint32_t  lastMoveDownTime;
    uint32_t  lastMoveInputTime;

    std::vector<TetrominoType> nextTetrominos;

    TetrominoType storedTetromino;

    std::unordered_map<SDL_Scancode, uint32_t> dasTimers;
    std::unordered_map<SDL_Scancode, bool> dasActive;

    bool canHardDrop;
    bool canRotate;
    bool canStore;

    bool ableToStore;

    Board* gameboard;
    Tetromino* currentTetromino;

    bool isRunning;
};

