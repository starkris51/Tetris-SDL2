#include "Game.h"

Game::Game()
	: window(nullptr), renderer(nullptr), event(), isRunning(true), lastMoveDownTime(0), lastMoveInputTime(0), canHardDrop(false), canRotate(false), canStore(false), storedTetromino(None), nextTetrominos() {
	gameboard = new Board;
	currentTetromino = nullptr;

	dasTimers[SDL_SCANCODE_LEFT] = 0;
	dasTimers[SDL_SCANCODE_RIGHT] = 0;
	dasActive[SDL_SCANCODE_LEFT] = false;
	dasActive[SDL_SCANCODE_RIGHT] = false;
}

Game::~Game() {
	delete gameboard;
	delete currentTetromino;
}

TetrominoType Game::getNextTetromino() {

	if (nextTetrominos.size() <= 0) {
		std::vector<TetrominoType> shuffledTetrominos = { I, O, T, L, J, S, Z };

		auto seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
		std::shuffle(shuffledTetrominos.begin(), shuffledTetrominos.end(), std::default_random_engine(seed));

		nextTetrominos = shuffledTetrominos;

		TetrominoType next = nextTetrominos.front();

		nextTetrominos.erase(nextTetrominos.begin());

		return next;
	}

	TetrominoType next = nextTetrominos.front();

	nextTetrominos.erase(nextTetrominos.begin());

	return next;
}

void Game::createNewTetromino(bool stored) {
	delete currentTetromino;

	if (!stored) {
		TetrominoType randomType = getNextTetromino();
		currentTetromino = new Tetromino(renderer, randomType);
	}
	else {
		storedTetromino = getNextTetromino();
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

			if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
				dasActive[event.key.keysym.scancode] = false;
				dasTimers[event.key.keysym.scancode] = 0;
			}

		}
	}

	if (currentTetromino) {
        if (keyboard_state[SDL_SCANCODE_LEFT]) {
            if (!dasActive[SDL_SCANCODE_LEFT]) {
                if (currentTime - dasTimers[SDL_SCANCODE_LEFT] >= DAS_DELAY) {
                    dasActive[SDL_SCANCODE_LEFT] = true;
                    dasTimers[SDL_SCANCODE_LEFT] = currentTime;
                } else {
                    dasTimers[SDL_SCANCODE_LEFT] = currentTime;
                }
                currentTetromino->move(-1, 0, *gameboard);
            } else if (currentTime - dasTimers[SDL_SCANCODE_LEFT] >= DAS_REPEAT) {
                dasTimers[SDL_SCANCODE_LEFT] = currentTime;
                currentTetromino->move(-1, 0, *gameboard);
            }
        }

        if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            if (!dasActive[SDL_SCANCODE_RIGHT]) {
                if (currentTime - dasTimers[SDL_SCANCODE_RIGHT] >= DAS_DELAY) {
                    dasActive[SDL_SCANCODE_RIGHT] = true;
                    dasTimers[SDL_SCANCODE_RIGHT] = currentTime;
                } else {
                    dasTimers[SDL_SCANCODE_RIGHT] = currentTime;
                }
                currentTetromino->move(1, 0, *gameboard);
            } else if (currentTime - dasTimers[SDL_SCANCODE_RIGHT] >= DAS_REPEAT) {
                dasTimers[SDL_SCANCODE_RIGHT] = currentTime;
                currentTetromino->move(1, 0, *gameboard);
            }
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
			currentTetromino->rotate(true, *gameboard);
		}
		else if (!canRotate && keyboard_state[SDL_SCANCODE_Z]) {
			canRotate = true;
			currentTetromino->rotate(false, *gameboard);
		}

		if (!canStore && keyboard_state[SDL_SCANCODE_C]) {
			canStore = true;

			if (storedTetromino == None) {
				storedTetromino = currentTetromino->getShape();
				createNewTetromino(false);
				return;
			}

			createNewTetromino(true);

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

	if (currentTetromino->checkIsPlaced()) {
		createNewTetromino(false);
	}

	if (currentTime - lastMoveDownTime >= 1000) {
		lastMoveDownTime = currentTime;

		if (!currentTetromino->checkIsPlaced()) {
			currentTetromino->move(0, 1, *gameboard);
		}
	}

}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}