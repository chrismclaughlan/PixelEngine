#pragma once
#include "Engine\hwindows.h"  // first
#include "Engine\win32_window.h"
#include "Engine\win32_network.h"
#include "Engine\win32_performance.h"

#include <iostream>

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
	// TODO error handling
protected:
	Window win;
	Performance performance;
	virtual void HandleInput() = 0;
	virtual void DoFrame() = 0;
	bool networkEnabled;

public:
	Game(bool enableNetwork, const char* name, int32 width, int32 height)
		: 
		networkEnabled(enableNetwork), 
		win(name, width, height), 
		gridSize(20), 
		fpsLimit(0)
	{
		createGrid();
	}
	~Game()
	{
		delete[] grid;
	}

	virtual int32 run() = 0;
protected:
	void createGrid();
	void clearParticles();
	void DrawParticles();
	bool inline emptyParticle(int32 x, int32 y);
	bool inline containsParticle(int32 x, int32 y, State::Value val);
	State::Value inline getParticle(int32 x, int32 y);
protected:
	//std::string text;
	//std::string acceptedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ ";
	const int32 gridSize;
	uint8* grid;
	uint32 fpsLimit;
	PaintBrush paintBrush;
	static constexpr const int32 maxPaintBrushSize = 10;
};