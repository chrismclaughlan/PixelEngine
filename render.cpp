#include <vector>
#include <math.h>
#include <cmath>
#include "render.h"

#define calculate_scales()\
float ratio = 1080 / nscale_y;\
scale_x = nscale_x * ratio;\
scale_y = nscale_y * ratio;\

Render::Render()
{
	state = RenderState();

	int32 nscale_x = DEFAULT_SCALE_X;
	int32 nscale_y = DEFAULT_SCALE_Y;

	// Calculate scales where scale_y == 1080
	calculate_scales();
}

Render::Render(int32 nscale_x, int32 nscale_y)
{
	state = RenderState();

	// Calculate scales where scale_y == 1080
	calculate_scales();
}

Render::Render(RenderState* nstate, int32 nscale_x, int32 nscale_y)
{
	state = *nstate;

	// Calculate scales where scale_y == 1080
	calculate_scales();
}

void Render::sizeChangeWin32(RECT* rect)
{
	state.width = rect->right - rect->left;
	state.height = rect->bottom - rect->top;

	int32 buffer_size = state.width * state.height * sizeof(uint32);
	if (state.memory)
	{
		VirtualFree(state.memory, 0, MEM_RELEASE);
	}
	state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	state.bitmapinfo.bmiHeader.biSize = sizeof(state.bitmapinfo);
	state.bitmapinfo.bmiHeader.biWidth = state.width;
	state.bitmapinfo.bmiHeader.biHeight = state.height;
	state.bitmapinfo.bmiHeader.biPlanes = 1;
	state.bitmapinfo.bmiHeader.biBitCount = 32;
	state.bitmapinfo.bmiHeader.biCompression = BI_RGB;

	float nscale_x = state.width;
	float nscale_y = state.height;
	calculate_scales();
}

inline void Render::Clamp(int32 min, int32* val, int32 max)
{
	if (*val < min) *val = min;
	else if (*val > max) *val = max;
}

#include <stdio.h>

inline void Render::ScaleX(float* val)
{
	*val *= state.width / scale_x;
}

inline void Render::ScaleY(float* val)
{
	*val *= state.height / scale_y;
}

// Business methods //

void Render::DrawGrid(float pwidth, float pheight)
{
	// width is bad
	ScaleX(&pwidth);
	ScaleY(&pheight);
	uint32 colour = 0x000000;
	for (int y = 0; y < state.height; y += pheight)
	{
		for (int x = 0; x < state.width; x += pwidth)
		{
			DrawRectP(x, y, x+pwidth, y+ pheight, colour);
			//DrawRectOutlineP(x, y, x + nwidth, y + nheight, colour, 0.2);
			colour += 0xabcdef;
		}
	}
}

void Render::DrawGridV2(float w, float h)
{
	ScaleX(&w);
	ScaleY(&h);
	uint32 colour = 0x000000;
	for (int y = 0; y < state.height; y += h)
	{
		for (int x = 0; x < state.width; x += w)
		{
			DrawRectOutlineP(x, y, x + w, y + h, colour, 1);
			//DrawRectOutlineP(x, y, x + nwidth, y + nheight, colour, 0.2);
			colour += 0xabcdef;
		}
	}
}

void Render::ClearScreen(uint32 colour)
{
	uint32* pixel = (uint32*)state.memory;
	for (int32 y = 0; y < state.height; y++)
	{
		for (int32 x = 0; x < state.width; x++)
		{
			*pixel++ = colour;
		}
	}
}

inline void Render::DrawPointP
(int32 x, int32 y, uint32 colour)
{
	Clamp(0, &x, state.width - 1);
	Clamp(0, &y, state.height - 1);

	uint32* pixel = (uint32*)state.memory + x + (y * state.width);
	*pixel = colour;
}

// Behaves weird
void Render::DrawPoint
(float nx, float ny, uint32 colour)
{
	ScaleX(&nx);
	ScaleY(&ny);
	int32 x = nx;
	int32 y = ny;
	DrawPointP(x, y, colour);
}

// works
void Render::DrawLineP
(int32 x1, int32 y1, int32 x2, int32 y2, uint32 colour)
{
	int32 dx, sx, dy, sy, err, e2;
	dx = abs(x2 - x1);
	dy = -(abs(y2 - y1));
	if (x1 < x2)
		sx = 1;
	else
		sx = -1;
	if (y1 < y2)
		sy = 1;
	else
		sy = -1;
	err = dx + dy;
	while (true)
	{
		DrawPointP(x1, y1, colour);
		if (x1 == x2 && y1 == y2)
			break;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x1 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y1 += sy;
		}
	}
}

// works
void Render::DrawLine
(float x1, float y1, float x2, float y2, uint32 colour)
{
	ScaleX(&x1);
	ScaleY(&y1);
	ScaleX(&x2);
	ScaleY(&y2);

	DrawLineP(x1, y1, x2, y2, colour);
}

std::vector<Point> Render::ReturnLine
(int32 x1, int32 y1, int32 x2, int32 y2)
{
	std::vector<Point> points;
	int32 dx, sx, dy, sy, err, e2;
	dx = abs(x2 - x1);
	dy = -(abs(y2 - y1));
	if (x1 < x2)
		sx = 1;
	else
		sx = -1;
	if (y1 < y2)
		sy = 1;
	else
		sy = -1;
	err = dx + dy;
	while (true)
	{
		Point p{ x1, y1 };
		points.push_back(p);
		//DrawPointP(x1, y1, colour);
		if (x1 == x2 && y1 == y2)
			break;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x1 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y1 += sy;
		}
	}
	return points;
}

inline std::vector<Point> Render::ReturnLine
(Point v1, Point v2)
{
	return Render::ReturnLine(v1.x, v1.y, v2.x, v2.y);
}

bool compareY(const Point& p1, const Point& p2)
{
	return p1.y < p2.y;
}

//// Done in pixels
//void Render::DrawPolygon(PolygonV3 polygon)
//{
//	// Find max and min values to create bounding box
//	Point max_x{ 0, 0 };
//	Point max_y{ 0, 0 };
//	Point min_x{ 0, state.width };
//	Point min_y{ 0, state.height };
//	for (int i = 0; i < 3; i++)
//	{
//		if (polygon.points[i].x > max_x.x)
//			max_x = polygon.points[i];
//		if (polygon.points[i].x < min_x.x)
//			min_x = polygon.points[i];
//		if (polygon.points[i].y > max_y.y)
//			max_y = polygon.points[i];
//		if (polygon.points[i].y < min_y.y)
//			min_y = polygon.points[i];
//	}
//
//	std::vector<Point> edges;
//	std::vector<Point> E1 = Render::ReturnLine(polygon.points[0], polygon.points[1]);
//	std::vector<Point> E2 = Render::ReturnLine(polygon.points[1], polygon.points[2]);
//	std::vector<Point> E3 = Render::ReturnLine(polygon.points[2], polygon.points[0]);
//	int32 size = E1.size() + E2.size() + E3.size();
//	edges.reserve(size);
//	edges.insert(edges.end(), E1.begin(), E1.end());
//	edges.insert(edges.end(), E2.begin(), E2.end());
//	edges.insert(edges.end(), E3.begin(), E3.end());
//
//	std::sort(edges.begin(), edges.end(), compareY);
//	// eg: 4,0 | (2, 1), (4, 1) ...
//
//	// iterate over edges
//	int32 line = -1;
//	bool inside = false;
//	for (std::vector<Point>::size_type i = 0; i != edges.size(); i++) {
//		if (i != line)
//		{
//			// new line
//			inside = false;
//			line = edges[i].y;
//		}
//		for (int x_ = min_x.x; x_ < max_x.x; x_++)
//		{
//			if (x_ == edges[i].x && inside == false)
//				inside = true;
//			else if (x_ == edges[i].x)
//			{
//				inside = false;
//			}
//			if (inside)
//				DrawPointP(x_, line, 0xff0000);
//		}
//	}
//
//
//	// test
//	DrawLineP(polygon.points[0].x, polygon.points[0].y, polygon.points[1].x, polygon.points[1].y, 0xffffff);
//	DrawLineP(polygon.points[1].x, polygon.points[1].y, polygon.points[2].x, polygon.points[2].y, 0xffffff);
//	DrawLineP(polygon.points[2].x, polygon.points[2].y, polygon.points[0].x, polygon.points[0].y, 0xffffff);
//}

// works
void Render::DrawRectP
(int32 x1, int32 y1, int32 x2, int32 y2, uint32 colour)
{
	Clamp(0, &x1, state.width);
	Clamp(0, &x2, state.width);
	Clamp(0, &y1, state.height);
	Clamp(0, &y2, state.height);

	for (int32 y = y1; y < y2; y++)
	{
		uint32* pixel = (uint32*)state.memory + x1
			+ (y * state.width);
		for (int32 x = x1; x < x2; x++)
		{
			*pixel++ = colour;
		}
	}
}

// works
void Render::DrawRect(float x, float y, float w, float h, uint32 colour)
{
	ScaleX(&x);
	ScaleY(&y);
	ScaleX(&w);
	ScaleY(&h);

	// Convert to pixels
	int32 x1 = x - w;
	int32 x2 = x + w;
	int32 y1 = y - h;
	int32 y2 = y + h;
	DrawRectP(x1, y1, x2, y2, colour);
}

// works
void Render::DrawRectOutlineP
(int32 x1, int32 y1, int32 x2, int32 y2, uint32 colour, int32 t)
{
	DrawRectP(x1, y1, x1 + t, y2, colour);
	DrawRectP(x1, y2 - t, x2, y2, colour);
	DrawRectP(x2 - t, y1, x2, y2, colour);
	DrawRectP(x1, y1, x2, y1 + t, colour);
}

// scale thickness?
void Render::DrawRectOutline
(float x1, float y1, float x2, float y2, uint32 colour, float thickness)
{
	ScaleX(&x1);
	ScaleY(&y1);
	ScaleX(&x2);
	ScaleY(&y2);

	//float thickness_x = thickness;
	//ScaleX(&thickness_x);
	//ScaleY(&thickness);

	// Convert to pixels
	int32 _x1 = x1;
	int32 _x2 = x2;
	int32 _y1 = y1;
	int32 _y2 = y2;
	int32 _tx = thickness;
	int32 _ty = thickness;
	DrawRectP(_x1, _y1, _x1 + _tx, _y2, colour);
	DrawRectP(_x1, _y2 - _ty, _x2, _y2, colour);
	DrawRectP(_x2 - _tx, _y1, _x2, _y2, colour);
	DrawRectP(_x1, _y1, _x2, _y1 + _ty, colour);
}

void Render::DrawRectOutlineCentreP
(int32 x, int32 y, int32 w, int32 h, uint32 colour, float thickness)
{
	float x1 = x - (w / 2);
	float y1 = y - (h / 2);
	float x2 = x + (w / 2);
	float y2 = y + (h / 2);
	Render::DrawRectOutlineP(x1, y1, x2, y2, colour, thickness);
}

void Render::DrawRectOutlineCentre
(float x, float y, float w, float h, uint32 colour, float thickness)
{
	ScaleX(&x);
	ScaleY(&y);
	ScaleX(&w);
	ScaleY(&h);

	float x1 = x - (w / 2);
	float y1 = y - (h / 2);
	float x2 = x + (w / 2);
	float y2 = y + (h / 2);

	// Convert to pixels
	int32 _x1 = x1;
	int32 _x2 = x2;
	int32 _y1 = y1;
	int32 _y2 = y2;
	int32 t = thickness;
	Render::DrawRectOutlineP(x1, y1, x2, y2, colour, t);
}

//// BROKEN when x > y
//void Render::DrawTriangleP
//(int32 x1, int32 y1, int32 x2, int32 y2, uint32 colour)
//{
//	Clamp(0, &x1, state.width);
//	Clamp(0, &x2, state.width);
//	Clamp(0, &y1, state.height);
//	Clamp(0, &y2, state.height);
//
//	float m = (y2 - y1) / (x2 - x1);
//	for (int32 y = y1; y < y2; y++)
//	{
//		int32 new_x = (y + x1) / m;
//		uint32* pixel = (uint32*)state.memory + new_x + (y * state.width);
//		for (int32 x = new_x; x < x2; x++)
//		{
//			*pixel++ = colour;
//		}
//	}
//}
//
//// BROKEN when x > y
//void Render::DrawTriangle
//(float x1, float y1, float x2, float y2, uint32 colour)
//{
//	ScaleY(&x1);
//	ScaleY(&y1);
//	ScaleY(&x2);
//	ScaleY(&y2);
//
//	DrawTriangleP((int32)x1, (int32)y1, (int32)x2, (int32)y2, colour);
//}