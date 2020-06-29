#include "win32_mainwindow.h"
#include "demo.h"
#include "graphics.h"
#include <math.h>
#include <assert.h>

#define ConvertToTileCoord(a)\
a = (a + 1.0) * (gridSize/2.0);\
a = (int32)floor(a);\
Graphics::Clamp(0.0, &a, (float)gridSize - 1.0);\

#define ConvertToScreenCoord(a)\
a = (a / (gridSize/2)) - 1;\

#define MAX_PAINTBRUSH_SIZE 10

void DemoWindow::run()
{
	/* ----------------------- Input ----------------------- */

	if (released(input::BUTTON_ESCAPE))
		clearParticles();

	// Paint Brush

	//input.wheelTurns = 0;
	if (input.wheelTurns < 1)
		input.wheelTurns = 1;
	else if (input.wheelTurns > MAX_PAINTBRUSH_SIZE)
		input.wheelTurns = MAX_PAINTBRUSH_SIZE;
	paintBrush.size = input.wheelTurns;

	if (pressed(input::BUTTON_1))
		paintBrush.paint = State::Value::Sand;
	else if (pressed(input::BUTTON_2))
		paintBrush.paint = State::Value::Water;
	else if (pressed(input::BUTTON_3))
		paintBrush.paint = State::Value::Concrete;
	else if (pressed(input::BUTTON_0))
		paintBrush.paint = State::Value::Empty;

	float paintBrushX = rend.pxToScreenX(input.mouse_x_pos);
	float paintBrushY = rend.pxToScreenY(input.mouse_y_pos);
	ConvertToTileCoord(paintBrushX);
	ConvertToTileCoord(paintBrushY);

	if (input.left_click)
	{
		placeParticle((int32)paintBrushX, (int32)paintBrushY, paintBrush.size);
	}
	else if (input.middle_click)
	{
		paintBrush.paint = getParticle(paintBrushX, paintBrushY);
	}

	/* ----------------------- Simulate ----------------------- */

	UpdateParticles();

	/* ----------------------- Render ----------------------- */

	rend.ClearScreen(0x000000);

	DrawParticles();
	DrawPaintBrush(paintBrushX, paintBrushY, paintBrush.size);

	/* ----------------------- FPS ----------------------- */

	performance.Update();
	if (fpsLimit > 0)
		performance.LimitFps(fpsLimit);
	//std::string format = std::to_string(performance.getFps());
	std::string format = std::to_string(input.wheelTurns);
	SetWindowTextW(m_hwnd, CharToWString("Demo Window | FPS: " + format).c_str());
}

void DemoWindow::placeParticle(int32 x, int32 y, int32 size)
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

void DemoWindow::UpdateParticles()
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

void DemoWindow::DrawParticles()
{
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			if (emptyParticle(i, j))
				continue;

			float x = i;
			float y = j;
			float size = (float)2.0 / gridSize;

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

			rend.DrawRect(x, y, x + size, y + size, colour);
		}
	}
}

void DemoWindow::DrawPaintBrush(float x, float y, int32 size)
{
	uint32 colour = 0x00000000;
	if (paintBrush.paint & State::Value::Sand)
		colour = 0xc2b280;
	else if (paintBrush.paint & State::Value::Water)
		colour = 0x0f5e9c;
	else if (paintBrush.paint & State::Value::Concrete)
		colour = 0xaaaaaa;

	float particleSize = (float)2.0 / gridSize;

	ConvertToScreenCoord(x);
	ConvertToScreenCoord(y);
	rend.DrawRect(
		x - ((size - 1) * particleSize),
		y - ((size - 1) * particleSize),
		x + particleSize + ((size - 1) * particleSize),
		y + particleSize + ((size - 1) * particleSize),
		colour);
}

void DemoWindow::clearParticles()
{
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			grid[i + (j * gridSize)] = State::Value::Empty;
		}
	}
}

bool inline DemoWindow::emptyParticle(int32 x, int32 y)
{
	return containsParticle(x, y, State::Value::Empty);
}

bool inline DemoWindow::containsParticle(int32 x, int32 y, State::Value val)
{
	assert(x >= 0 && x < gridSize);
	assert(y >= 0 && y < gridSize);
	if (grid[x + (y * gridSize)] & val)
	{
		return true;
	}

	return false;
}

State::Value DemoWindow::getParticle(int32 x, int32 y)
{
	assert(x >= 0 && x < gridSize);
	assert(y >= 0 && y < gridSize);
	return (State::Value)grid[x + (y * gridSize)];
}