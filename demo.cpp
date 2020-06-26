#include "win32_windows.h"
#include "demo.h"
#include "render.h"
#include <math.h>

#define ConvertToTileCoord(a)\
a = (a + 1.0) * 10.0;\
a = floor(a);\
render::Render::Clamp(0.0, &a, 20.0 - 1.0);\

void DemoWindow::run()
{
	/* ------------------ Input ------------------ */

	// demo visuals
	if (released(input::BUTTON_ESCAPE))
		resetTiles();


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
		tiles[_x + (_y * 20)].type = Sand;
	}
	else
	{
		SetWindowTextW(m_hwnd, CharToWString("goodbye").c_str());
	}

	// Update tile positions
//#define withinXBounds() tiles[i].coord.x >= -1.0 && tiles[i].coord.x <= -1.0f
//#define withinYBounds() tiles[i].coord.y >= -1.0 && tiles[i].coord.y <= -1.0f
	//for (std::vector<Tile>::size_type i = 0; i != tiles.size(); i++) {
	//	if (tiles[i].type == Sand)
	//	{
	//		// Check below
	//		if (tiles[i].coord.y > -1.0f)
	//		{
	//			if (containsTile(tiles[i].coord.x, tiles[i].coord.y - 0.1f))
	//			{
	//				// Check below and left
	//				// Check below and right
	//			}
	//			else
	//			{
	//				// Fall
	//				tiles[i].coord.y -= 0.1 * 0.001 * performance.delta_time;
	//			}
	//		}
	//	}
	//}

	/* ------------------ Render ------------------ */

	for (std::vector<Tile>::iterator tile = tiles.begin(); tile != tiles.end(); ++tile)
	{
		rend.DrawRect(tile->coord.x, tile->coord.y, tile->coord.x + 0.1, tile->coord.y + 0.1, tile->type);
	}
}

void DemoWindow::resetTiles()
{
	for (std::vector<Tile>::iterator tile = tiles.begin(); tile != tiles.end(); ++tile)
	{
		tile->type = Empty;
	}
}

bool DemoWindow::containsTile(float x, float y)
{
	ConvertToTileCoord(x);
	ConvertToTileCoord(y);
	if (tiles[x + (y * 20)].type == Empty)
	{
		return false;
	}

	return true;
}