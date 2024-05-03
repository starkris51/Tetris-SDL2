#pragma once
#include <SDL.h>
#include <iostream>
#include "Board.h"

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

private: 
    Board* gameboard;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    bool isRunning;
};

