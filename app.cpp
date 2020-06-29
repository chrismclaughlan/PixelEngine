#include "app.h"

void App::HandleInput()
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

void App::DoFrame()
{
	win.gfx().ClearScreen(0xff0000);

	// debug
	text = std::to_string(win.mouse.getPosX()) + " " + std::to_string(win.mouse.getPosY());
	win.setTitle(text);

	win.gfx().Render();  // last
}