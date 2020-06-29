#pragma once
#include "hwindows.h"  // first
#include "win32_window.h"

class App
{
private:
	Window win;
	std::string text;  //test
private:
	void DoFrame()
	{
		win.gfx().ClearScreen(0xff0000);

		text = std::to_string(win.mouse.getPosX()) + " " + std::to_string(win.mouse.getPosY());
		win.setTitle(text);  // test
		
		win.gfx().Render();  // last
	}
	void HandleInput()
	{
		while (!win.keyboard.keyIsEmpty())
		{
			const auto event = win.keyboard.readKey();
			switch (event.getCode())
			{
			case VK_DOWN:
			{
				win.shouldClose();
			} break;
			}
		}
		
		while (!win.mouse.isEmpty())
		{
			const auto event = win.mouse.read();
			switch (event.getType())
			{
			case Mouse::Event::Type::LPressed:
			{
			} break;
			case Mouse::Event::Type::RPressed:
			{
			} break;
			case Mouse::Event::Type::WheelDown:
			{
			} break;
			case Mouse::Event::Type::Move:
			{
			} break;
			}
		}

		while (!win.keyboard.charIsEmpty())
		{
			const int8 e = win.keyboard.readChar();
			text += std::string(1, e);
			win.setTitle(text);  // test
		}
	}
public:
	App() : win(L"App Window", 600, 600) {}
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