#pragma once
#include "Engine\win32_nclient.h"
#include "Engine\types.h"
#include "Game\game.h"

#include <winsock2.h>

class GameClient : public Game
{
private:
	NClient* net = nullptr;
	std::string text;
	std::string acceptedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ ";

public:
	//using Game::Game;  // inherit ALL construtors
	GameClient(bool enableNetwork)
		: Game(enableNetwork, "Game", 800, 800)
	{
		if (networkEnabled)
		{
			netConnect();
		}
	}
	~GameClient()
	{
		netDisconnect();
	}
	int32 run();
private:
	void netConnect()
	{
		netDisconnect();
		net = new NClient();
	}
	void netDisconnect()
	{
		if (net != nullptr)
		{
			delete net;
		}
	}

	void HandleInput();
	void DoFrame();
	void placeParticle(int32 x, int32 y, int32 size);
	void UpdateParticles();
	void DrawPaintBrush();
};