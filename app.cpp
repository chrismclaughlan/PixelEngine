#include "app.h"
#include <iostream>
#include <assert.h>
#include "exception.h"

#define DISPLAY_DEBUG_CONSOLE

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
			THROW_EXCEPTION("lPressed");
		} break;
		case Mouse::Event::Type::RPressed:
		{
		} break;
		case Mouse::Event::Type::WheelDown:
		{
			assert(false);
		} break;
		case Mouse::Event::Type::Move:
		{
#ifdef DISPLAY_DEBUG_CONSOLE
			std::cout << event.getX() << " " << event.getY() << "\n";
#endif
		} break;
		}
	}

	while (!win.keyboard.charIsEmpty())
	{
		const uint8 e = win.keyboard.readChar();
		if (e == VK_BACK && !text.empty())
		{
			text.pop_back();
		}
		else if (acceptedCharacters.find(e) != std::string::npos)
		{
			// If char in acceptedCharacters
			text += std::string(1, e);
		}
#ifdef DISPLAY_DEBUG_CONSOLE

		std::cout << text << "\n";
#endif
	}
}

void App::DoFrame()
{
	win.gfx().ClearScreen(0xff0000);

	// debug
	//text = std::to_string(win.mouse.getPosX()) + " " + std::to_string(win.mouse.getPosY());
	//win.setTitle(text);

	win.gfx().Render();  // last
}