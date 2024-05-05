#pragma once
#include <SDL.h>
#include <SDL_image.h>

enum TetrominoType {
	I=0, O=1, T=2, L=3, J=4, S=5, Z=6
};

class Tetromino
{
public:
	Tetromino(SDL_Renderer* renderer, TetrominoType shape);
	~Tetromino();

	void render(SDL_Renderer* renderer);
	void move(int dx, int dy);
	void rotate();

	const bool(&getMatrix() const)[4][4]{
		return matrix;
	}

	int getX() const;
	int getY() const;

	TetrominoType getShape() const { return shape; };

private:
	SDL_Texture* texture;
	bool matrix[4][4];
	int x, y;
	int rotationState;
	TetrominoType shape;
};

