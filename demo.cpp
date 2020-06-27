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
	// Flip update state
	uint8 updateMask = performance.begin_time.QuadPart & (long)1 << 7;

	/* ------------------ Input ------------------ */

	// demo visuals
	if (released(input::BUTTON_ESCAPE))
	{
		clearParticles();
		rend.ClearScreen(0x000000);
	}

	/* ------------------ Simulate ------------------ */

	// Draw circle where cursor is
	if (input.mouse_click)
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
		// Debug
		std::string _xstr = std::to_string(_x);
		std::string _ystr = std::to_string(_y);
		SetWindowTextW(m_hwnd, CharToWString(_xstr + " " + _ystr).c_str());

		ConvertToTileCoord(_x);
		ConvertToTileCoord(_y);
		if (!containsParticle(_x, _y))
		{
			assert(_x >= 0 && _x < gridSize);
			assert(_y >= 0 && _y < gridSize);
			grid[(int32)_x + ((int32)_y * gridSize)] = State::Value::Sand | updateMask;
		}
	}
	else
	{
		std::string format = std::to_string(updateMask);
		SetWindowTextW(m_hwnd, CharToWString(format).c_str());
	}

	// Update tile positions
	uint8* tempGrid = new uint8[gridSize * gridSize];
	std::copy(&grid[0], &grid[0] + (gridSize * gridSize), &tempGrid[0]);

	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			if (grid[i + (j * gridSize)] & State::Value::Empty)
			{
				continue;
			}
			else
			{
				// Check below
				if (j - 1 >= 0)
				{
					if (grid[i + ((j - 1) * gridSize)] & State::Value::Empty)
					{
						// Move here
						tempGrid[i + ((j - 1) * gridSize)] = grid[i + (j * gridSize)];
						tempGrid[i + (j * gridSize)] = State::Value::Empty | updateMask;
					}
					else
					{
						if (i > 0)
						{
							// check bottom left
							if (grid[(i - 1) + ((j - 1) * gridSize)] & State::Value::Empty)
							{
								if (grid[(i - 1) + ((j - 1) * gridSize)] & updateMask)
								{
								}
								else
								{
									continue;
								}
								// Move here
								tempGrid[(i - 1) + ((j - 1) * gridSize)] = grid[i + (j * gridSize)];
								tempGrid[i + (j * gridSize)] = State::Value::Empty | updateMask;
								continue;
							}
						}
						if (i < gridSize - 1)
						{
							// check bottom right
							if (grid[(i + 1) + ((j - 1) * gridSize)] & State::Value::Empty)
							{
								if (grid[(i - 1) + ((j - 1) * gridSize)] & updateMask)
								{
								}
								else
								{
									continue;
								}
								// Move here
								tempGrid[(i+1) + ((j-1) * gridSize)] = grid[i + (j * gridSize)];
								tempGrid[i + (j * gridSize)] = State::Value::Empty | updateMask;
								continue;
							}
						}
					}
				}
			}
		}
	}

	// Copy tempGrid into grid
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			grid[i + (j * gridSize)] = tempGrid[i + (j * gridSize)];
		}
	}
	delete[] tempGrid;

	/* ------------------ Render ------------------ */
	for (int32 j = 0; j < gridSize; j++)
	{
		for (int32 i = 0; i < gridSize; i++)
		{
			float _x = i;
			float _y = j;
			ConvertToScreenCoord(_x);
			ConvertToScreenCoord(_y);

			uint32 colour = 0x00000000;
			if (grid[i + (j * gridSize)] & State::Value::Sand)
			{
				colour = 0xc2b280;
			}
			rend.DrawRect(_x, _y, _x + 0.1, _y + 0.1, colour);
		}
	}
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

bool DemoWindow::containsParticle(int32 x, int32 y)
{
	//ConvertToTileCoord(x);
	//ConvertToTileCoord(y);
	assert(x >= 0 && x < gridSize);
	assert(y >= 0 && y < gridSize);
	if (grid[x + (y * gridSize)] & State::Value::Empty)
	{
		return false;
	}

	return true;
}