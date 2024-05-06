#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Tetromino;

class Board
{
public:
	Board();
	~Board();
	void init(SDL_Renderer* renderer, int posX, int posY);
	void render(SDL_Renderer* renderer);
	void placeBlock(Tetromino& tetromino);
	void checkLines();
	bool checkCollision(Tetromino& tetromino) const;
	void deleteRow(int row);

	int getCellSize() const { return cellSize; }

	const bool(&getBoardData() const)[10][20]{
		return boardData;
	}

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

