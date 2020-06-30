#pragma once
#include "Engine\hwindows.h"  // first
#include "Engine\win32_window.h"
#include "Engine\win32_performance.h"

#define ConvertToTileCoord(a)\
a = (a + 1.0) * (gridSize/2.0);\
a = (int32)floor(a);\
Graphics::Clamp(0.0, &a, (double)gridSize - 1.0);\

#define ConvertToScreenCoord(a)\
a = (a / (gridSize/2)) - 1;\


namespace State
{
	enum Value : uint8
	{
		Empty		= 1 << 1,
		Sand		= 1 << 2,
		Water		= 1 << 3,
		Concrete	= 1 << 4,
		// block to delete what touches it (for waterfall impl.)
		// dirt (destructable) (only fall if no support)
	};
}

struct PaintBrush
{
	State::Value paint = State::Value::Sand;
	int32 size = 1;
	double x = -1;
	double y = -1;
	bool isPainting = false;
	//const int32* x = input.  ??
};

class Game
{
protected:
	Window win;
	Performance performance;
	virtual void HandleInput();
	virtual void DoFrame();

public:
	Game() : win(L"Game", 800, 800), gridSize(20), fpsLimit(0)
	{
		createGrid();
	}
	Game(int32 gridSize) : win(L"Game", 800, 800), gridSize(gridSize), fpsLimit(0)
	{
		createGrid();
	}
	Game(int32 gridSize, const char* name, int32 width, int32 height) : win(name, width, height), gridSize(gridSize), fpsLimit(0)
	{
		createGrid();
	}
	~Game()
	{
		delete[] grid;
	}

	virtual int32 run();
protected:
	void createGrid();
	void clearParticles();
	void DrawParticles();
	bool inline emptyParticle(int32 x, int32 y);
	bool inline containsParticle(int32 x, int32 y, State::Value val);
	State::Value inline getParticle(int32 x, int32 y);

	virtual void placeParticle(int32 x, int32 y, int32 size);
	virtual void UpdateParticles();
	virtual void DrawPaintBrush();
protected:
	//std::string text;
	//std::string acceptedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ ";
	const int32 gridSize;
	uint8* grid;
	uint32 fpsLimit;
	PaintBrush paintBrush;
	static constexpr const int32 maxPaintBrushSize = 10;
};