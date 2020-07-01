#include "game_server.h"
#include "Engine\exception.h"
#include "Engine\types.h"
#include "Game\network_protocols.h"

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
		HandleNetwork();
	}

	return 0;
}

void GameServer::HandleNetwork()
{
	if (!net.isOnline())
	{
		THROW_EXCEPTION("offline");  // TODO network exceptions
	}

	//NETWORK::Packet localPacket(data, sizeof(data));
	NETWORK::Packet foreignPacket;

	// retreive data
	if (net.receive(foreignPacket))
	{
		decodePacket(foreignPacket);
	}

	const char testGameData[] =
	{
		NETWORK::PacketData::Header::Data,
		5, 5,
		'a', 'b', 'c', 'd', 'e',
		'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o',
		'p', 'q', 'r', 's', 't',
		'1', '2', '3', '4', '5',
	};
	NETWORK::Packet testPacket(testGameData, sizeof(testGameData));

	// send data
	//net.deliver(testPacket);





	//test
	//const char testDataPing[] =
	//{
	//	NETWORK::PacketData::Header::Ping,
	//};
	//const char testGameData[] =
	//{
	//	NETWORK::PacketData::Header::Data,
	//	5, 5,
	//	'a', 'b', 'c', 'd', 'e',
	//	'f', 'g', 'h', 'i', 'j',
	//	'k', 'l', 'm', 'n', 'o',
	//	'p', 'q', 'r', 's', 't',
	//	'1', '2', '3', '4', '5',
	//};
	//NETWORK::Packet testPacket(testGameData, sizeof(testGameData) - 5);
}
void GameServer::decodePacket(NETWORK::Packet& packet)
{
	int32 i = 0;
	switch (packet.buffer[i++])
	{
		case NETWORK::PacketData::Header::Invalid:
		{
			//std::cout << " -> Invalid\n";
		} break;
		case NETWORK::PacketData::Header::Join:
		{
			net.coutIncomingAddr();
			std::cout << " -> Join\n";
		} break;
		case NETWORK::PacketData::Header::Leave:
		{
			net.coutIncomingAddr();
			std::cout << " -> Leave\n";
		} break;
		case NETWORK::PacketData::Header::Ping:
		{
			net.coutIncomingAddr();
			std::cout << " -> Ping -> ";
			for (int i = 1; i < packet.numBytes; i++)
			{
				std::cout << packet.buffer[i];
			}
			std::cout << "\n";
			// return ping
			net.deliver(packet);
		} break;
		case NETWORK::PacketData::Header::Data:
		{
			net.coutIncomingAddr();
			std::cout << " -> Data -> Size " << packet.numBytes << "\n";
			// test
			int32 gridSizeX = packet.buffer[i++];
			int32 gridSizeY = packet.buffer[i++];

			for (int32 y = 0; y < gridSizeY; y++)
			{
				for (int32 x = 0; x < gridSizeX; x++)
				{
					int32 index = (x + (y * gridSizeX)) + i;
					if (index >= packet.numBytes)
					{
						THROW_EXCEPTION("Packet cut short");
						break;
					}
					else
					{
						std::cout << packet.buffer[index];
					}
				}
				std::cout << "\n";
			}
			break;
		}
	}
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