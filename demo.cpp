#include "win32_windows.h"
#include "demo.h"
#include "render.h"
#include <math.h>
#include <assert.h>

#define ConvertToTileCoord(a)\
a = (a + 1.0) * (gridSize/2.0);\
a = floor(a);\
render::Render::Clamp(0.0, &a, (float)gridSize - 1.0);\

#define ConvertToScreenCoord(a)\
a = (a / (gridSize/2)) - 1;\

void DemoWindow::run()
{
	/* ----------------------- Input ----------------------- */

	// demo visuals
	if (released(input::BUTTON_ESCAPE))
	{
		clearParticles();
		rend.ClearScreen(0x000000);
	}

	/* ----------------------- Simulate ----------------------- */

	UpdateParticles();

	if (input.left_click)
	{
		// Debug
		//std::string x1 = std::to_string(input.mouse_x_pos);
		//std::string y1 = std::to_string(input.mouse_y_pos);
		//std::string formatPx = "Px x: " + x1 + " y: " + y1;
		//float x = rend.pxToScreenX(input.mouse_x_pos);
		//float y = rend.pxToScreenY(input.mouse_y_pos);
		//std::string x2 = std::to_string(x);
		//std::string y2 = std::to_string(y);
		//std::string formatSc = " Sc x: " + x2 + " y: " + y2;
		//std::string format = formatPx + formatSc;
		//SetWindowTextW(m_hwnd, CharToWString(format).c_str());
		//rend.DrawPoint(x, y, 0xffffff);

		// Get mouse coords
		float _x = rend.pxToScreenX(input.mouse_x_pos);
		float _y = rend.pxToScreenY(input.mouse_y_pos);
		ConvertToTileCoord(_x);
		ConvertToTileCoord(_y);
		if (emptyParticle(_x, _y))
		{
			grid[(int32)_x + ((int32)_y * gridSize)] = State::Value::Sand;
		}
	}
	else if (input.right_click)
	{
		// Get mouse coords
		float _x = rend.pxToScreenX(input.mouse_x_pos);
		float _y = rend.pxToScreenY(input.mouse_y_pos);
		ConvertToTileCoord(_x);
		ConvertToTileCoord(_y);
		if (emptyParticle(_x, _y))
		{
			grid[(int32)_x + ((int32)_y * gridSize)] = State::Value::Water;
		}
	}

	/* ----------------------- Render ----------------------- */

	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			float _x = i;
			float _y = j;
			ConvertToScreenCoord(_x);
			ConvertToScreenCoord(_y);

			uint32 colour = 0x00000000;
			if (containsParticle(i, j, State::Value::Sand))
			{
				colour = 0xc2b280;
			}
			else if (containsParticle(i, j, State::Value::Water))
			{
				colour = 0x0f5e9c;
			}
			rend.DrawRect(_x, _y, _x + 0.1, _y + 0.1, colour);
		}
	}

	performance.Update();
	performance.LimitFps(fpsLimit);
	std::string format = std::to_string(performance.getFps());
	SetWindowTextW(m_hwnd, CharToWString("FPS: " + format).c_str());
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

	// Update tile positions
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			if (emptyParticle(i, j))
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

					if (containsParticle(i, j, State::Value::Sand) &
						containsParticle(i, j - 1, State::Value::Water))
					{
						// Flip
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
					// Check Left
					if ((i > 0) & (i < gridSize - 1))
					{
						if (emptyParticle(i - 1, j))
						{
							grid[(i - 1) + ((j)*gridSize)] = grid[i + (j * gridSize)];
							grid[i + (j * gridSize)] = State::Value::Empty;
							continue;
						}
						else
						{
							// Check Right
							if (emptyParticle(i + 1, j))
							{
								grid[(i + 1) + (j * gridSize)] = grid[i + (j * gridSize)];
								grid[i + (j * gridSize)] = State::Value::Empty;
								continue;
							}
						}
					}
				}
			}
		}  // for .. i
	}  // for .. j
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