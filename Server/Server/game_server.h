#pragma once
#include "Engine\win32_nserver.h"
#include "Engine\types.h"
#include "Game\game.h"

#include <winsock2.h>

class GameServer : public Game
{
private:
	NServer net;
public:
	//using Game::Game;  // inherit ALL construtors
	GameServer(uint16 port)
		: net(port), Game(true, "Game", 800, 800)
	{
	}
	int32 run();
private:
	void HandleNetwork();
	void decodePacket(NETWORK::Packet& packet);

	void HandleInput();
	void DoFrame();
};