#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Tetromino;

class Board
{
public:
	Board(int size);
	~Board();
	void init(SDL_Renderer* renderer, int posX, int posY);
	void setPosition(int posX, int posY, int size);
	void render(SDL_Renderer* renderer);
	void placeBlock(Tetromino& tetromino);
	int getBoardWidth() const;
	int getBoardHeight() const;
	void checkLines();
	bool checkCollision(Tetromino& tetromino) const;
	void deleteRow(int row);

	int getCellSize() const { return cellSize; }

	const bool(&getBoardData() const)[10][20]{
		return boardData;
	}

	const int getX() const {
		return boardPositionX;
	}

	const int getY() const {
		return boardPositionY;
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

