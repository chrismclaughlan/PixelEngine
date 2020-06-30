#include "game_client.h"
#include "Engine\exception.h"
#include "Engine\types.h"

#include <iostream>

int32 GameClient::run()
{
	int32 exitCode;

	//try
	//{
	if (networkEnabled)
	{
		net->connect("127.0.0.1", 9999);
	}
	//}
	//catch (...)
	//{
	//	return -1;
	//}

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
			if (!net->isOnline())
			{
				break;
			}
		}
	}

	return 0;
}

void GameClient::HandleInput()
{
	while (!win.keyboard.keyIsEmpty())
	{
		const auto event = win.keyboard.readKey();
		switch (event.getCode())
		{
		case VK_ESCAPE:
		{
			clearParticles();
		} break;
		case 0x30:
		{
			paintBrush.paint = State::Value::Empty;
		} break;
		case 0x31:
		{
			paintBrush.paint = State::Value::Sand;
		} break;
		case 0x32:
		{
			paintBrush.paint = State::Value::Water;
		} break;
		case 0x33:
		{
			paintBrush.paint = State::Value::Concrete;
		} break;
		case VK_RETURN:
		{
			net->ping();
		} break;
		}
	}

	while (!win.mouse.isEmpty())
	{
		const auto event = win.mouse.read();
		switch (event.getType())
		{
		case Mouse::Event::Type::Move:
		{
#if DISPLAY_DEBUG_CONSOLE && DISPLAY_MOUSE_COORDS
			std::cout << event.getX() << " " << event.getY() << "\n";
#endif
			paintBrush.x = win.Gfx().pxToScreenX(event.getX());
			paintBrush.y = win.Gfx().pxToScreenY(event.getY());
			ConvertToTileCoord(paintBrush.x);
			ConvertToTileCoord(paintBrush.y);
		} break;
		case Mouse::Event::Type::LPressed:
		{
			//placeParticle(event.getX(), event.getY(), paintBrush.size);
			paintBrush.isPainting = true;
		} break;
		case Mouse::Event::Type::LReleased:
		{
			//placeParticle(event.getX(), event.getY(), paintBrush.size);
			paintBrush.isPainting = false;
		} break;
		case Mouse::Event::Type::RPressed:
		{
			// single click
			placeParticle(paintBrush.x, paintBrush.y, paintBrush.size);
		} break;
		case Mouse::Event::Type::MPressed:
		{
			paintBrush.paint = getParticle(paintBrush.x, paintBrush.y);
		} break;
		case Mouse::Event::Type::WheelDown:
		{
			paintBrush.size += 1;
			if (paintBrush.size > maxPaintBrushSize)
				paintBrush.size = maxPaintBrushSize;
		} break;
		case Mouse::Event::Type::WheelUp:
		{
			paintBrush.size -= 1;
			if (paintBrush.size < 1)
				paintBrush.size = 1;
		} break;
		}
	}

		while (!win.keyboard.charIsEmpty())
		{
			const uint8 e = win.keyboard.readChar();
			if (e == VK_BACK && !text.empty())
			{
				text.pop_back();
				if (networkEnabled)
					netSend(text.c_str());
			}
			else if (acceptedCharacters.find(e) != std::string::npos)
			{
				// If char in acceptedCharacters
				text += std::string(1, e);
				if (networkEnabled)
					netSend(text.c_str());
			}
	#ifdef DISPLAY_DEBUG_CONSOLE
	
			std::cout << text << "\n";
	#endif
		}
}

void GameClient::DoFrame()
{
	/* ---------- Simulate ---------- */
	if (paintBrush.isPainting)
	{
		placeParticle(paintBrush.x, paintBrush.y, paintBrush.size);
	}
	UpdateParticles();

	/* ---------- Render ---------- */
	win.Gfx().ClearScreen(0x000000);

	DrawParticles();
	DrawPaintBrush();

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

void GameClient::placeParticle(int32 x, int32 y, int32 size)
{
	const int32 d = size - 1;
	for (int32 j = y - d; j <= y + d; j++)
	{
		for (int32 i = x - d; i <= x + d; i++)
		{
			if ((i < 0) | (i >= gridSize))
				continue;
			if ((j < 0) | (j >= gridSize))
				continue;
			grid[i + (j * gridSize)] = paintBrush.paint;
		}
	}
}

void GameClient::UpdateParticles()
{
	// Flip update state
	uint8 frameMask = performance.getFrameMask();

#define checkBottomLeft()\
if (i > 0)\
{\
if (emptyParticle(i-1, j-1))\
{\
grid[(i - 1) + ((j - 1) * gridSize)] = grid[i + (j * gridSize)];\
grid[i + (j * gridSize)] = State::Value::Empty;\
continue;\
}\
}\

#define checkBottomRight()\
if (i < gridSize - 1)\
{\
if (emptyParticle(i+1, j-1))\
{\
grid[(i + 1) + ((j - 1) * gridSize)] = grid[i + (j * gridSize)];\
grid[i + (j * gridSize)] = State::Value::Empty;\
continue;\
}\
}\

#define checkLeft()\
if (i > 0)\
{\
if (emptyParticle(i - 1, j))\
{\
grid[(i - 1) + ((j)*gridSize)] = grid[i + (j * gridSize)];\
grid[i + (j * gridSize)] = State::Value::Empty;\
continue;\
}\
}\

#define checkRight()\
if ((i < gridSize - 1))\
{\
if (emptyParticle(i + 1, j))\
{\
grid[(i + 1) + (j * gridSize)] = grid[i + (j * gridSize)];\
grid[i + (j * gridSize)] = State::Value::Empty;\
continue;\
}\
}\

	// Update tile positions
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			if (emptyParticle(i, j) | containsParticle(i, j, State::Value::Concrete))
			{
				continue;
			}
			else
			{
				// Check below
				if (j - 1 >= 0)
				{
					if (emptyParticle(i, j - 1))
					{
						// Move here
						grid[i + ((j - 1) * gridSize)] = grid[i + (j * gridSize)];
						grid[i + (j * gridSize)] = State::Value::Empty;
						continue;
					}

					// If particle above water; flip
					if (containsParticle(i, j, State::Value::Sand) &
						containsParticle(i, j - 1, State::Value::Water))
					{
						grid[i + (j * gridSize)] = State::Value::Water;
						grid[i + ((j - 1) * gridSize)] = State::Value::Sand;
						continue;
					}

					// Every other frame check opposite side for symmetry
					if (frameMask)
					{
						checkBottomLeft();
						checkBottomRight();
					}
					else
					{
						checkBottomRight();
						checkBottomLeft();
					}
				}

				if (containsParticle(i, j, State::Value::Water))
				{
					if (frameMask)
					{
						checkLeft();
						checkRight();
					}
					else
					{
						checkRight();
						checkLeft();
					}
				}
			}
		}  // for .. i
	}  // for .. j
}

void GameClient::DrawPaintBrush()
{
	uint32 colour = 0x00000000;
	if (paintBrush.paint & State::Value::Sand)
		colour = 0xc2b280;
	else if (paintBrush.paint & State::Value::Water)
		colour = 0x0f5e9c;
	else if (paintBrush.paint & State::Value::Concrete)
		colour = 0xaaaaaa;

	double particleSize = (double)2.0 / gridSize;

	double screenCoordX = paintBrush.x;
	double screenCoordY = paintBrush.y;
	ConvertToScreenCoord(screenCoordX);
	ConvertToScreenCoord(screenCoordY);
	win.Gfx().DrawRect(
		screenCoordX - ((paintBrush.size - 1) * particleSize),
		screenCoordY - ((paintBrush.size - 1) * particleSize),
		screenCoordX + particleSize + ((paintBrush.size - 1) * particleSize),
		screenCoordY + particleSize + ((paintBrush.size - 1) * particleSize),
		colour);
}