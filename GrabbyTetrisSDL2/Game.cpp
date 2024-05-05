#include "Game.h"

TetrominoType getRandomTetrominoType() {
	static std::default_random_engine engine((unsigned)time(0));
	std::uniform_int_distribution<int> distribution(0, 6);

	return static_cast<TetrominoType>(distribution(engine));
}

Game::Game()
	: window(nullptr), renderer(nullptr), event(), isRunning(1) {
	gameboard = new Board;
	currentTetromino = nullptr;
}

Game::~Game() {
	delete gameboard;
	delete currentTetromino;
}

void Game::createNewTetromino() {
	TetrominoType randomType = getRandomTetrominoType();
	currentTetromino = new Tetromino(renderer, randomType);
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

		isRunning = true;

		gameboard->init(renderer, 0, 0);

		createNewTetromino();

	}

	else {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		isRunning = false;
	}

}

void Game::handleEvents() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
		}

		if (event.type == SDL_KEYDOWN && currentTetromino) {
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				currentTetromino->rotate();
				break;

			case SDLK_LEFT:
				currentTetromino->move(-1, 0);
				break;

			case SDLK_RIGHT: 
				currentTetromino->move(1, 0);
				break;

			case SDLK_DOWN:
				currentTetromino->move(0, 1);
				if (gameboard->checkCollision(*currentTetromino)) {
					gameboard->placeBlock(*currentTetromino);
					createNewTetromino();
				}
				break;
			case SDLK_z:
				delete currentTetromino;
				createNewTetromino();
				break;

			default:
				break;
			}
		}
	}

	
}

void Game::render() {
	if (!renderer) { return; }

	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	gameboard->render(renderer);

	if (currentTetromino) {
		currentTetromino->render(renderer);
	}

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