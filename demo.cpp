#include "win32_windows.h"
#include "demo.h"
#include "render.h"
#include <math.h>
#include <assert.h>

#define ConvertToTileCoord(a)\
a = (a + 1.0) * 10.0;\
a = floor(a);\
render::Render::Clamp(0.0, &a, 20.0 - 1.0);\

#define ConvertToScreenCoord(a)\
a = (a / 10) - 1;\

void DemoWindow::run()
{
	rend.ClearScreen(0x000000);

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
		ConvertToTileCoord(_x);
		ConvertToTileCoord(_y);
		// Debug
		std::string _xstr = std::to_string(_x);
		std::string _ystr = std::to_string(_y);
		SetWindowTextW(m_hwnd, CharToWString(_xstr + " " + _ystr).c_str());

		if (!containsParticle(_x, _y))
		{
			assert(_x >= 0 && _x < 20);
			assert(_y >= 0 && _y < 20);
			grid[(int32)_y][(int32)_x] = Sand;
		}
	}
	else
	{
		std::string format = "goodbye";
		SetWindowTextW(m_hwnd, CharToWString(format).c_str());
	}

	// Update tile positions
	uint32 tempGrid[20][20];
	for (int32 j = 0; j < 20; j++)
	{
		for (int32 i = 0; i < 20; i++)
		{
			tempGrid[j][i] = grid[j][i];
		}
	}
	for (int32 j = 0; j < 20; j++)
	{
		for (int32 i = 0; i < 20; i++)
		{
			if (grid[j][i] != Empty)
			{
				// Check below
				if (j - 1 >= 0)
				{
					if (grid[j - 1][i] == Empty)
					{
						// Move here
						tempGrid[j - 1][i] = grid[j][i];
						tempGrid[j][i] = Empty;
					}
					else
					{
						if (i > 0)
						{
							// check bottom left
							if (grid[j - 1][i - 1] == Empty)
							{
								// Move here
								tempGrid[j - 1][i - 1] = grid[j][i];
								tempGrid[j][i] = Empty;
							}
							else if (grid[j - 1][i + 1] == Empty && i < 20 - 1)
							{
								// Move here
								tempGrid[j - 1][i + 1] = grid[j][i];
								tempGrid[j][i] = Empty;
							}
						}
						//if (i < 20)
						//{
						//	// check bottom right
						//	if (grid[j - 1][i + 1] == Empty)
						//	{
						//		// Move here
						//		tempGrid[j - 1][i + 1] = grid[j][i];
						//		tempGrid[j][i] = Empty;
						//	}
						//}
					}
				}
			}
		}
	}

	// Copy tempGrid into grid
	for (int32 j = 0; j < 20; j++)
	{
		for (int32 i = 0; i < 20; i++)
		{
			grid[j][i] = tempGrid[j][i];
		}
	}

	/* ------------------ Render ------------------ */
	for (int32 j = 0; j < 20; j++)
	{
		for (int32 i = 0; i < 20; i++)
		{
			float _x = i;
			float _y = j;
			ConvertToScreenCoord(_x);
			ConvertToScreenCoord(_y);
			rend.DrawRect(_x, _y, _x + 0.1, _y + 0.1, grid[j][i]);
		}
	}
}

void DemoWindow::clearParticles()
{
	for (int32 j = 0; j < 20; j++)
	{
		for (int32 i = 0; i < 20; i++)
		{
			grid[j][i] = Empty;
		}
	}
}

bool DemoWindow::containsParticle(int32 x, int32 y)
{
	//ConvertToTileCoord(x);
	//ConvertToTileCoord(y);
	assert(x >= 0 && x < 20);
	assert(y >= 0 && y < 20);
	if (grid[y][x] == Empty)
	{
		return false;
	}

	return true;
}