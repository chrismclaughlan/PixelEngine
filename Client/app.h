#pragma once
#include "hwindows.h"  // first
#include "win32_window.h"
#include "win32_performance.h"

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

class App
{
private:
	Window win;
	Performance performance;
private:
	void HandleInput();
	void DoFrame();

public:
	App(int32 gridSize) : win(L"App", 800, 800), gridSize(gridSize), fpsLimit(0)
	{
		createGrid();
	}
	~App()
	{
		delete[] grid;
	}

	int32 run();
private:
	void createGrid();
	void placeParticle(int32 x, int32 y, int32 size);
	void UpdateParticles();
	void DrawParticles();
	void DrawPaintBrush();
	void clearParticles();
	bool inline emptyParticle(int32 x, int32 y);
	bool inline containsParticle(int32 x, int32 y, State::Value val);
	State::Value inline getParticle(int32 x, int32 y);
private:
	//std::string text;
	//std::string acceptedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ ";

	const int32 gridSize;
	uint8* grid;
	uint32 fpsLimit;
	PaintBrush paintBrush;
};