#pragma once
#include "win32_windows.h"
#include <string>

inline const std::wstring CharToWString(const std::string str)
{
	//const std::string str(c);
	const std::wstring wstr = std::wstring(str.begin(), str.end());
	return wstr;
}

class GameWindow :public win32::MainWindow
{
public:
	GameWindow(
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
	{}
public:
	void run()
	{
		// demo visuals

		uint32 colour = 0x000000;
		if (is_down(input::BUTTON_UP))
			colour = 0xff0000;
		if (is_down(input::BUTTON_DOWN))
			colour = 0x0ff000;
		if (is_down(input::BUTTON_LEFT))
			colour = 0x00fff0;
		if (is_down(input::BUTTON_RIGHT))
			colour = 0x000fff;

		renderer.ClearScreen(colour);
		renderer.DrawGridV2(100, 100);
		renderer.DrawCircleP(100, 100, 50, 0xffffff);
		renderer.DrawCircleP(input.mouse_x_pos, input.mouse_y_pos, 20, 0xff0000);

		// Draw circle where cursor is
		if (input.mouse_click)
		{
			renderer.DrawCircleP(input.mouse_x_pos, input.mouse_y_pos, 30, 0xff0000);
			std::string x1 = std::to_string(input.mouse_x_pos);
			std::string y1 = std::to_string(input.mouse_y_pos);
			std::string formatPx = "Px x: " + x1 + " y: " + y1;
			float x = pxToScreenX(input.mouse_x_pos);
			float y = pxToScreenY(input.mouse_y_pos);
			std::string x2 = std::to_string(x);
			std::string y2 = std::to_string(y);
			std::string formatSc = " Sc x: " + x2 + " y: " + y2;
			std::string format = formatPx + formatSc;
			BOOL result = SetWindowTextW(m_hwnd, CharToWString(format).c_str());
		}
		else
		{
			BOOL result = SetWindowTextW(m_hwnd, CharToWString("goodbye").c_str());
		}
	}
};