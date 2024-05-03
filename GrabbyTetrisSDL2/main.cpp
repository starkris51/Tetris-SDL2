#include <iostream>
#include <SDL.h>
#include "Game.h"

Game *game = nullptr;

//skibidi

int main(int argc, char* argv[])
{
    game = new Game();

    game->init();

    while (game->running()) {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();

    return 0;
}