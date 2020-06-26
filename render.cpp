#include <vector>
#include <math.h>
#include <cmath>
#include "render.h"

namespace render
{
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

void Render::Clamp(int32 min, int32* val, int32 max)
{
	if (*val < min) *val = min;
	else if (*val > max) *val = max;
}

void Render::Clamp(float min, float* val, float max)
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

// -1.0f <> 1.0f
float Render::pxToScreenX(int32 x)
{
	return ((2.0f * x) / state.width) - 1.0f;
}

// -1.0f <> 1.0f
float Render::pxToScreenY(int32 y)
{
	return ((2.0f * y) / state.height) - 1.0f;
}

// 0 <> screenWidth
int32 Render::screenToPxX(float x)
{
	return (x + 1.0f) * (state.width / 2.0f);
}

// 0 <> screenHeight
int32 Render::screenToPxY(float y)
{
	return (y + 1.0f) * (state.height / 2.0f);
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

void Render::DrawPoint(float x, float y, uint32 colour)
{
	int32 xi = screenToPxX(x);
	int32 yi = screenToPxY(y);
	Clamp(0, &xi, state.width - 1);
	Clamp(0, &yi, state.height - 1);
	uint32* pixel = (uint32*)state.memory;
	pixel[xi + (yi * state.width)] = 0xffffff;
}

void Render::DrawRectP(int32 x1, int32 y1, int32 x2, int32 y2, uint32 colour)
{
	Clamp(0, &x1, state.width);
	Clamp(0, &x2, state.width);
	Clamp(0, &y1, state.height);
	Clamp(0, &y2, state.height);

	for (int32 y = y1; y < y2; y++)
	{
		uint32* pixel = (uint32*)state.memory + x1 + (y * state.width);
		for (int32 x = x1; x < x2; x++)
		{
			*pixel++ = colour;
		}
	}
}

void Render::DrawRect(float x1, float y1, float x2, float y2, uint32 colour)
{
	DrawRectP(
		screenToPxX(x1), 
		screenToPxY(y1), 
		screenToPxX(x2), 
		screenToPxY(y2), 
		colour
		);
}

}  // namespace render