#pragma once
#include "hwindows.h"
#include "graphics.h"

class Win32Graphics : public Graphics
{
private:
	HDC hdc;
	BITMAPINFO bitmapinfo;
public:
	Win32Graphics(HWND hwnd, HDC hdc);
	void Render();
	void ChangeSize(RECT* rect);
};