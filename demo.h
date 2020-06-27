#pragma once
#include "win32_performance.h"
#include "win32_mainwindow.h"
#include "render.h"
#include <string>
#include <vector>

inline const std::wstring CharToWString(const std::string str)
{
	//const std::string str(c);
	const std::wstring wstr = std::wstring(str.begin(), str.end());
	return wstr;
}

namespace State
{
	enum Value : uint8
	{
		Empty	= 1 << 1,
		Sand	= 1 << 2,
		Water	= 1 << 3,
	};
}

struct PaintBrush
{
	State::Value paint = State::Value::Sand;
	int32 size = 1;
	//const int32* x = input.  ??
};

class DemoWindow :public win32::MainWindow
{
public:
	DemoWindow(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		int32 gridSize,			// added param
		uint32 _fpsLimit = 30,	// added param: fpsLimit > 0
		bool hideCursor = false,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
		) : gridSize(gridSize), fpsLimit(_fpsLimit),
		win32::MainWindow::MainWindow(
			lpWindowName, dwStyle, hideCursor, dwExStyle, 
			x, y, nWidth, nHeight, hWndParent, hMenu)
	{
		grid = new uint8[gridSize*gridSize];
		for (int32 j = 0; j < gridSize; j++)
		{
			for (int32 i = 0; i < gridSize; i++)
			{
				grid[i + (j * gridSize)] = State::Value::Empty;
			}
		}
	}
	~DemoWindow()
	{
		MainWindow::~MainWindow();
		delete[] grid;
	}
	void run();

private:
	const int32 gridSize;
	uint8* grid;
	Performance performance;
	uint32 fpsLimit;
	PaintBrush paintBrush;
	
private:
	void placeParticle(int32 x, int32 y, int32 size);
	void UpdateParticles();
	void DrawParticles();
	void DrawPaintBrush(float x, float y, int32 size);
	void clearParticles();
	bool inline emptyParticle(int32 x, int32 y);
	bool inline containsParticle(int32 x, int32 y, State::Value val);
	State::Value inline getParticle(int32 x, int32 y);
};