#include "Game.h"

std::array<TetrominoType, 7> tetromino7Bag = { I, O, T, L, J, S, Z};

Game::Game()
	: window(nullptr), renderer(nullptr), event(), isRunning(true), lastMoveDownTime(0), lastMoveInputTime(0), canHardDrop(false), canRotate(false), canStore(false), storedTetromino(None) {
	gameboard = new Board;
	currentTetromino = nullptr;

	nextTetrominos = {};
}

Game::~Game() {
	delete gameboard;
	delete currentTetromino;
}

TetrominoType Game::getNextTetromino() {

	if (nextTetrominos.empty()) {
		unsigned seed = 0;
		std::array<TetrominoType, 7> shuffledTetrominos = tetromino7Bag;

		std::shuffle(shuffledTetrominos.begin(), shuffledTetrominos.end(), std::default_random_engine(seed));

		nextTetrominos = shuffledTetrominos;
	}

	TetrominoType next = nextTetrominos[0];

	std::rotate(nextTetrominos.begin(), nextTetrominos.begin() + 1, nextTetrominos.end());

	return next;
}

void Game::createNewTetromino(bool stored) {
	delete currentTetromino;

	if (!stored) {
		TetrominoType randomType = getNextTetromino();
		currentTetromino = new Tetromino(renderer, randomType);
	}
	else {
		currentTetromino = new Tetromino(renderer, storedTetromino);
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

		isRunning = true;

		gameboard->init(renderer, 0, 0);

		createNewTetromino(false);

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
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z) {
				canRotate = false;
			}
			if (event.key.keysym.sym == SDLK_c) {
				canStore = false;
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

		if (!canRotate && keyboard_state[SDL_SCANCODE_UP]) {
			canRotate = true;
			currentTetromino->rotate(true);
		}
		else if (!canRotate && keyboard_state[SDL_SCANCODE_Z]) {
			canRotate = true;
			currentTetromino->rotate(false);
		}

		if (!canStore && keyboard_state[SDL_SCANCODE_C]) {
			canStore = true;

			if (storedTetromino == None) {
				storedTetromino = currentTetromino->getShape();
				createNewTetromino(false);
				return;
			}

			createNewTetromino(true);
			std::cout << storedTetromino;

			return;
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
	if (currentTime - lastMoveDownTime >= 700) {
		lastMoveDownTime = currentTime;

		currentTetromino->move(0, 1, *gameboard);
	}

	if (currentTetromino->checkIsPlaced()) {
		createNewTetromino(false);
	}
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}