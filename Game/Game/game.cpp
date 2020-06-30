#include "Engine\exception.h"
#include "Engine\defines.h"
#include "game.h"
#include <iostream>
#include <assert.h>

void Game::createGrid()
{
	grid = new uint8[gridSize * gridSize];
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			grid[i + (j * gridSize)] = State::Value::Empty;
		}
	}
}

void Game::DrawParticles()
{
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			if (emptyParticle(i, j))
				continue;

			double x = i;
			double y = j;
			double size = 2.0 / gridSize;

			ConvertToScreenCoord(x);
			ConvertToScreenCoord(y);

			uint32 colour = 0x00000000;
			State::Value value = getParticle(i, j);
			if (value & State::Value::Sand)
				colour = 0xc2b280;
			else if (value & State::Value::Water)
				colour = 0x0f5e9c;
			else if (value & State::Value::Concrete)
				colour = 0xaaaaaa;

			win.Gfx().DrawRect(x, y, x + size, y + size, colour);
		}
	}
}

void Game::clearParticles()
{
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			grid[i + (j * gridSize)] = State::Value::Empty;
		}
	}
}

bool inline Game::emptyParticle(int32 x, int32 y)
{
	return containsParticle(x, y, State::Value::Empty);
}

bool inline Game::containsParticle(int32 x, int32 y, State::Value val)
{
	assert(x >= 0 && x < gridSize);
	assert(y >= 0 && y < gridSize);
	if (grid[x + (y * gridSize)] & val)
	{
		return true;
	}

	return false;
}

State::Value Game::getParticle(int32 x, int32 y)
{
	assert(x >= 0 && x < gridSize);
	assert(y >= 0 && y < gridSize);
	return (State::Value)grid[x + (y * gridSize)];
}