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

private:
	int boardWidth = 10;
	int boardHeight = 20;
	int cellWidth = 30;
	int cellHeight = 30;

	int boardPositionX;
	int boardPositionY;

	SDL_Texture* textures[2];
	SDL_Texture* boardTexture[10][20];
};

