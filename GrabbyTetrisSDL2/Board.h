#pragma once
#include <SDL.h>

class Board
{
public:
	Board();
	~Board();
	void boardinit();
	void renderboard(SDL_Renderer* renderer) const;

private:
	SDL_Rect* gameboard[10][20];
	int boardx = 0;
	int boardy = 0;
};

