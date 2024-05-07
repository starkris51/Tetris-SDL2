#include "Game.h"

TetrominoType getRandomTetrominoType() {
	static std::default_random_engine engine((unsigned)time(0));
	std::uniform_int_distribution<int> distribution(0, 6);

	return static_cast<TetrominoType>(distribution(engine));
}

Game::Game()
	: window(nullptr), renderer(nullptr), event(), isRunning(1), lastMoveDownTime(0), lastMoveInputTime(0), canHardDrop(false) {
	gameboard = new Board;
	currentTetromino = nullptr;
}

Game::~Game() {
	delete gameboard;
	delete currentTetromino;
}

void Game::createNewTetromino() {
	delete currentTetromino;
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

		lastMoveDownTime = SDL_GetTicks();

	}

	else {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		isRunning = false;
	}

}

void Game::handleEvents() {
	uint32_t currentTime = SDL_GetTicks();
	const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);


	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
			isRunning = false;
		}

		if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_SPACE) {
				canHardDrop = false;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				currentTetromino->rotate();
			}
		}
	}

	if (currentTetromino) {
		if (keyboard_state[SDL_SCANCODE_LEFT] && currentTime - lastMoveInputTime >= 100) {
			lastMoveInputTime = currentTime;
			currentTetromino->move(-1, 0, *gameboard);
		} else if (keyboard_state[SDL_SCANCODE_RIGHT] && currentTime - lastMoveInputTime >= 100) {
			lastMoveInputTime = currentTime;
			currentTetromino->move(1, 0, *gameboard);
		}

		if (keyboard_state[SDL_SCANCODE_DOWN] && currentTime - lastMoveInputTime >= 50) {
			lastMoveInputTime = currentTime;
			currentTetromino->move(0, 1, *gameboard);
		}

		if (!canHardDrop && keyboard_state[SDL_SCANCODE_SPACE]) {
			canHardDrop = true;
			currentTetromino->hardDrop(*gameboard);
		}
	}
}

void Game::render() {
	if (!renderer) { return; }

	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	gameboard->render(renderer);

	currentTetromino->render(renderer);

	currentTetromino->renderGhostPiece(renderer, *gameboard);

	SDL_RenderPresent(renderer);
}

void Game::update() {
	uint32_t currentTime = SDL_GetTicks();
	if (currentTime - lastMoveDownTime >= 200) {
		lastMoveDownTime = currentTime;

		currentTetromino->move(0, 1, *gameboard);
	}

	if (currentTetromino->checkIsPlaced()) {
		createNewTetromino();
	}
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}