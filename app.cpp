#include "app.h"
#include "exception.h"
#include "defines.h"
#include <iostream>
#include <assert.h>

#define ConvertToTileCoord(a)\
a = (a + 1.0) * (gridSize/2.0);\
a = (int32)floor(a);\
Graphics::Clamp(0.0, &a, (double)gridSize - 1.0);\

#define ConvertToScreenCoord(a)\
a = (a / (gridSize/2)) - 1;\

#define MAX_PAINTBRUSH_SIZE 10

void App::createGrid()
{
	grid = new uint8[gridSize * gridSize];
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			grid[i + (j * gridSize)] = State::Value::Empty;
		}
	}
}

void App::HandleInput()
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
		case Mouse::Event::Type::MPressed:
		{
			paintBrush.paint = getParticle(paintBrush.x, paintBrush.y);
		} break;
		case Mouse::Event::Type::WheelDown:
		{
			paintBrush.size -= 1;
			if (paintBrush.size < 1)
				paintBrush.size = 1;
		} break;
		case Mouse::Event::Type::WheelUp:
		{
			paintBrush.size += 1;
			if (paintBrush.size > MAX_PAINTBRUSH_SIZE)
				paintBrush.size = MAX_PAINTBRUSH_SIZE;
		} break;
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

void App::DoFrame()
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

void App::placeParticle(int32 x, int32 y, int32 size)
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

void App::UpdateParticles()
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
					if (emptyParticle(i, j-1))
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

void App::DrawParticles()
{
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			if (emptyParticle(i, j))
				continue;

			double x = i;
			double y = j;
			double size = 2.0 / gridSize;

			ConvertToScreenCoord(x);
			ConvertToScreenCoord(y);

			uint32 colour = 0x00000000;
			State::Value value = getParticle(i, j);
			if (value & State::Value::Sand)
				colour = 0xc2b280;
			else if (value & State::Value::Water)
				colour = 0x0f5e9c;
			else if (value & State::Value::Concrete)
				colour = 0xaaaaaa;

			win.Gfx().DrawRect(x, y, x + size, y + size, colour);
		}
	}
}

void App::DrawPaintBrush()
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

void App::clearParticles()
{
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			grid[i + (j * gridSize)] = State::Value::Empty;
		}
	}
}

bool inline App::emptyParticle(int32 x, int32 y)
{
	return containsParticle(x, y, State::Value::Empty);
}

bool inline App::containsParticle(int32 x, int32 y, State::Value val)
{
	assert(x >= 0 && x < gridSize);
	assert(y >= 0 && y < gridSize);
	if (grid[x + (y * gridSize)] & val)
	{
		return true;
	}

	return false;
}

State::Value App::getParticle(int32 x, int32 y)
{
	assert(x >= 0 && x < gridSize);
	assert(y >= 0 && y < gridSize);
	return (State::Value)grid[x + (y * gridSize)];
}

int32 App::run()
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