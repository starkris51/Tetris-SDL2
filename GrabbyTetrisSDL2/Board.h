#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Board
{
public:
	Board();
	~Board();
	void init(SDL_Renderer* renderer, int posX, int posY);
	void render(SDL_Renderer* renderer);
	void setBlockTexture(int x, int y, int textureindex);

	int getCellSize() const { return cellSize; }

private:
	int boardWidth;
	int boardHeight;
	int cellSize;

	int boardPositionX;
	int boardPositionY;

	SDL_Texture* textures[2];
	SDL_Texture* boardTexture[10][20];
	bool boardData[10][20];
};

