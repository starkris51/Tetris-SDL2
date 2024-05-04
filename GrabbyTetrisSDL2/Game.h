#pragma once
#include <SDL.h>
#include <stdlib.h>
#include <random>
#include "Tetromino.h"
#include "Board.h"
#include <iostream>
#include "board.h"

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

    void createNewTetromino();

private: 
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    Board* gameboard;
    Tetromino* currentTetromino;

    bool isRunning;
};

