#pragma once
#include <SDL.h>
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

private: 
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    Board* gameboard ;

    bool isRunning;
};

