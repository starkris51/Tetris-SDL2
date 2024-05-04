#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Tetromino.h"
#include <iostream>

class Board
{
public:
	Board();
	~Board();
	void init(SDL_Renderer* renderer, int posX, int posY);
	void render(SDL_Renderer* renderer);
	void placeBlock(const bool(&matrix)[4][4], int x, int y, TetrominoType shape);
	bool checkCollision(const bool(&matrix)[4][4], int x, int y);

	int getCellSize() const { return cellSize; }

private:
	int boardWidth;
	int boardHeight;
	int cellSize;

	int boardPositionX;
	int boardPositionY;

	SDL_Texture* textures[8];
	SDL_Texture* boardTexture[10][20];
	bool boardData[10][20];
};

