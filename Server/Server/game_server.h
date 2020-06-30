#pragma once
#include "Engine\types.h"
#include "Game\game.h"

class GameClient : public Game
{
private:
	//void HandleInput();
	//void DoFrame();
public:
	GameClient(int32 gridSize) : Game(gridSize) {}
	//int32 run();

private:
	//void placeParticle(int32 x, int32 y, int32 size);
	//void UpdateParticles();
	//void DrawPaintBrush();
};