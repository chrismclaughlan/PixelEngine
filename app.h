#pragma once
#include "hwindows.h"  // first
#include "win32_window.h"

class App
{
private:
	Window win;
	std::string text;  //test
	std::string acceptedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ ";
private:
	void HandleInput();
	void DoFrame();

public:
	App() : win(L"App", 600, 600) {}
	~App() {}

	// TODO revice exitCode
	int32 Run()
	{
		int32 exitCode;
		while (true)
		{
			if (Window::processMessages(&exitCode))
			{
				return exitCode;
			}
			HandleInput();
			DoFrame();
		}

		return 0;
	}
};