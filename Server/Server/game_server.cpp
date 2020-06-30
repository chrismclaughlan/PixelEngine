#include "game_server.h"
#include "Engine\exception.h"
#include "Engine\types.h"

#include <iostream>

int32 GameServer::run()
{
	int32 exitCode;
	while (true)  // connect to main server first?
	{
		if (Window::processMessages(&exitCode))
		{
			return exitCode;
		}
		HandleInput();
		DoFrame();

		if (networkEnabled)
		{
			if (!net.isOnline())
			{
				break;
			}
		}
	}

	return 0;
}

void GameServer::HandleInput()
{
	while (!win.keyboard.keyIsEmpty())
	{
		const auto event = win.keyboard.readKey();
		switch (event.getCode())
		{
		case VK_ESCAPE:
		{
			if (event.isPressed())
				std::cout << "VK_ESCAPE\n";
		} break;
		}
	}

	while (!win.mouse.isEmpty())
	{
		const auto event = win.mouse.read();
		switch (event.getType())
		{
		}
	}

	//	while (!win.keyboard.charIsEmpty())
	//	{
	//		const uint8 e = win.keyboard.readChar();
	//		if (e == VK_BACK && !text.empty())
	//		{
	//			text.pop_back();
	//		}
	//		else if (acceptedCharacters.find(e) != std::string::npos)
	//		{
	//			// If char in acceptedCharacters
	//			text += std::string(1, e);
	//		}
	//#ifdef DISPLAY_DEBUG_CONSOLE
	//
	//		std::cout << text << "\n";
	//#endif
	//	}
}

void GameServer::DoFrame()
{
	/* ---------- Simulate ---------- */
	NETWORK::Packet packet;
	packet = net.receive();
	if (packet.numBytes > 0)
	{
		std::cout << packet.buffer << std::endl;
	}

	/* ---------- Render ---------- */
	win.Gfx().ClearScreen(0x000000);

	DrawParticles();

	performance.Update();
	if (fpsLimit > 0)
	{
		performance.LimitFps(fpsLimit);
	}
#if DISPLAY_DEBUG_CONSOLE && DISPLAY_FPS
	if (performance.hasTimePassed(1.0))
	{
		std::cout << "FPS: " << performance.getFps() << "\n";
	}
#endif

	win.Gfx().Render();  // last
}