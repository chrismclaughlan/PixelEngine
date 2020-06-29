#pragma once
#include "win32_windows.h"  // first
#include "win32_window.h"

class App
{
private:
	Window win;
	//std::string text;
private:
	void DoFrame()
	{
		win.Gfx().ClearScreen(0xff0000);
		
		win.Gfx().Render();  // last
	}
	void HandleInput()
	{
		while (!win.keyboard.KeyIsEmpty())
		{
			const auto event = win.keyboard.ReadKey();
			switch (event.GetCode())
			{
			case VK_DOWN:
			{
				assert(false);
			} break;
			}
		}
		
		while (!win.mouse.IsEmpty())
		{
			const auto event = win.mouse.Read();
			switch (event.GetType())
			{
			case Mouse::Event::Type::LPressed:
			{
				//assert(false);
			} break;
			case Mouse::Event::Type::WheelDown:
			{
				//assert(false);
			} break;
			case Mouse::Event::Type::Move:
			{
				//assert(false);
			} break;
			}
		}

		//while (!win.keyboard.CharIsEmpty())
		//{
		//	const uint8 e = win.keyboard.ReadChar();
		//	text += std::string(1, e);
		//	win.SetTitle(text);
		//}
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
			if (Window::ProcessMessages(&exitCode))
			{
				return exitCode;
			}
			HandleInput();
			DoFrame();
		}

		return 0;
	}
};