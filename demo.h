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

enum Type
{
	Empty = 0x000000,
	Sand = 0xc2b280,
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
		for (int32 j = 0; j < 20; j++)
		{
			for (int32 i = 0; i < 20; i++)
			{
				grid[j][i] = Empty;
			}
		}
	}
	~DemoWindow()
	{
		MainWindow::~MainWindow();
	}
	void run();

private:
	uint32 grid[20][20];
	
private:
	void clearParticles();
	bool containsParticle(int32 x, int32 y);
};