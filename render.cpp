#include "render.h"

Render::Render()
{
	state = RenderState();
	scale = DEFAULT_SCALE;
}

Render::Render(float nscale)
{
	state = RenderState();
	scale = nscale;
}

Render::Render(RenderState* nstate, float nscale)
{
	state = *nstate;
	scale = nscale;
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
}

inline void Render::Clamp(int32 min, int32* val, int32 max)
{
	if (*val < min) *val = min;
	else if (*val > max) *val = max;
}

inline void Render::Scale(float* val)
{
	*val *= state.height * scale;
}

// Business methods //

void Render::DrawGrid(int32 width, int32 height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			DrawRectOutline(x, y, 10, 10, 0x00ffff, 1);
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

inline void
Render::DrawPoint(int32 x, int32 y, uint32 colour)
{
	Clamp(0, &x, state.width - 1);
	Clamp(0, &y, state.height - 1);

	uint32* pixel = (uint32*)state.memory + x + (y * state.width);
	*pixel = colour;
}

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
		DrawPoint(x1, y1, colour);
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

void Render::DrawLine
(float x1, float y1, float x2, float y2, uint32 colour)
{
	Scale(&x1);
	Scale(&y1);
	Scale(&x2);
	Scale(&y2);

	DrawLineP(x1, y1, x2, y2, colour);
}

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

void Render::DrawRect(float x, float y, float w, float h, uint32 colour)
{
	Scale(&x);
	Scale(&y);
	Scale(&w);
	Scale(&h);

	// Convert to pixels
	int32 x1 = x - w;
	int32 x2 = x + w;
	int32 y1 = y - h;
	int32 y2 = y + h;
	DrawRectP(x1, y1, x2, y2, colour);
}

void Render::DrawRectOutlineP
(float x1, float y1, float x2, float y2, uint32 colour, float thickness)
{
	Scale(&x1);
	Scale(&y1);
	Scale(&x2);
	Scale(&y2);
	Scale(&thickness);

	// Convert to pixels
	int32 _x1 = x1;
	int32 _x2 = x2;
	int32 _y1 = y1;
	int32 _y2 = y2;
	int32 _t = thickness;
	DrawRectP(_x1, _y1, _x1 + _t, _y2, colour);
	DrawRectP(_x1, _y2 - _t, _x2, _y2, colour);
	DrawRectP(_x2 -_t, _y1, _x2, _y2, colour);
	DrawRectP(_x1, _y1, _x2, _y1 + _t, colour);
}

void Render::DrawRectOutline
(float x, float y, int32 w, int32 h, uint32 colour, float thickness)
{
	float window_x_limit = (state.width / (state.height * scale));
	float window_y_limit = (state.height / (state.height * scale));
	float tile_x_size = window_x_limit / w;
	float tile_y_size = window_y_limit / h;
	float x1 = (window_x_limit * x / h);
	float y1 = (window_y_limit * y / h);
	float x2 = (window_x_limit * x / h) + tile_x_size;
	float y2 = (window_y_limit * y / h) + tile_y_size;
	Render::DrawRectOutlineP(x1, y1, x2, y2, colour, thickness);
}

void Render::DrawTriangleP
(int32 x1, int32 y1, int32 x2, int32 y2, uint32 colour)
{
	Clamp(0, &x1, state.width);
	Clamp(0, &x2, state.width);
	Clamp(0, &y1, state.height);
	Clamp(0, &y2, state.height);

	float m = (y2 - y1) / (x2 - x1);
	for (int32 y = y1; y < y2; y++)
	{
		int32 new_x = (y + x1) / m;
		uint32* pixel = (uint32*)state.memory + new_x + (y * state.width);
		for (int32 x = new_x; x < x2; x++)
		{
			*pixel++ = colour;
		}
	}
}

void Render::DrawTriangle
(float x1, float y1, float x2, float y2, uint32 colour)
{
	Scale(&x1);
	Scale(&y1);
	Scale(&x2);
	Scale(&y2);

	DrawTriangleP((int32)x1, (int32)y1, (int32)x2, (int32)y2, colour);
}