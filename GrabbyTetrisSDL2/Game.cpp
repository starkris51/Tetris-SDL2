#include "Game.h"

Game::Game() {
	gameboard = new Board();
}

Game::~Game() {
	if (gameboard) {
		delete gameboard;
	}
}

void Game::init() {

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow("Tetrizz 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_RESIZABLE);
		if (!window) {
			std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
			isRunning = false;
			return;
		}

		renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
		if (!renderer) {
			std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
			isRunning = false;
			return;
		}

		if (gameboard) {
			gameboard->boardinit(); 
		}

		isRunning = true;
	}

	else {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		isRunning = false;
	}

}

void Game::handleEvents() {

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	}
}

void Game::render() {
	if (!renderer) { return; }

	SDL_RenderClear(renderer);

	gameboard->renderboard(renderer);

	SDL_RenderPresent(renderer);
}

void Game::update() {
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}