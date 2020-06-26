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

struct Coordinate
{
	float x;
	float y;
};

enum Type
{
	Empty = 0x000000,
	Sand = 0xc2b280,
};

struct Tile
{
	Coordinate coord;	// bottom left screen coord of 0.1f by 0.1f tile
	Type type = Sand;	// details behaviour and contains colour of tile
};

class DemoWindow :public win32::MainWindow
{
public:
	DemoWindow(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		bool hideCursor = false,  // added param
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
		) : win32::MainWindow::MainWindow(
			lpWindowName, dwStyle, hideCursor, dwExStyle, 
			x, y, nWidth, nHeight, hWndParent, hMenu)
	{
		for (float j = -1.0; j < 1.0; j += 0.1)
		{
			for (float i = -1.0; i < 1.0; i += 0.1)
			{
				Coordinate coords = { i, j };
				Tile tile = { coords, Empty };
				tiles.push_back(tile);
			}
		}
	}
	void run();

private:
	std::vector<Tile> tiles;
	bool containsTile(float x, float y);
	
private:
	void resetTiles();
};