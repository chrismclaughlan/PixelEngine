#pragma once
#include "win32_windows.h"
#include "render.h"
#include <string>
#include <vector>

inline const std::wstring CharToWString(const std::string str)
{
	//const std::string str(c);
	const std::wstring wstr = std::wstring(str.begin(), str.end());
	return wstr;
}

//const uint8 Empty = 0x10000000;
//const uint8 Sand = 0x01000000;

namespace State
{
	enum Value : uint8
	{
		Empty	= 1 << 1,
		Sand	= 1 << 2,
	};
}

class DemoWindow :public win32::MainWindow
{
public:
	DemoWindow(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		int32 gridSize,
		bool hideCursor = false,  // added param
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
		) : gridSize(gridSize), 
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

	
private:
	void clearParticles();
	bool containsParticle(int32 x, int32 y);
};