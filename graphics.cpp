#include <vector>
#include <math.h>
#include <cmath>
#include "graphics.h"

void Graphics::Clamp(int32 min, int32* val, int32 max)
{
	if (*val < min) *val = min;
	else if (*val > max) *val = max;
}

void Graphics::Clamp(float min, float* val, float max)
{
	if (*val < min) *val = min;
	else if (*val > max) *val = max;
}

#include <stdio.h>

inline void Graphics::ScaleX(float* val)
{
	*val *= width / scale_x;
}

inline void Graphics::ScaleY(float* val)
{
	*val *= height / scale_y;
}

// -1.0f <> 1.0f
float Graphics::pxToScreenX(int32 x)
{
	return ((2.0f * x) / width) - 1.0f;
}

// -1.0f <> 1.0f
float Graphics::pxToScreenY(int32 y)
{
	return ((2.0f * y) / height) - 1.0f;
}

// 0 <> screenWidth
int32 Graphics::screenToPxX(float x)
{
	return (x + 1.0f) * (width / 2.0f);
}

// 0 <> screenHeight
int32 Graphics::screenToPxY(float y)
{
	return (y + 1.0f) * (height / 2.0f);
}

void Graphics::ClearScreen(uint32 colour)
{
	uint32* pixel = (uint32*)memory;
	for (int32 y = 0; y < height; y++)
	{
		for (int32 x = 0; x < width; x++)
		{
			*pixel++ = colour;
		}
	}
}

void Graphics::DrawPoint(float x, float y, uint32 colour)
{
	int32 xi = screenToPxX(x);
	int32 yi = screenToPxY(y);
	Clamp(0, &xi, width - 1);
	Clamp(0, &yi, height - 1);
	uint32* pixel = (uint32*)memory;
	pixel[xi + (yi * width)] = 0xffffff;
}

void Graphics::DrawRectP(int32 x1, int32 y1, int32 x2, int32 y2, uint32 colour)
{
	Clamp(0, &x1, width);
	Clamp(0, &x2, width);
	Clamp(0, &y1, height);
	Clamp(0, &y2, height);

	for (int32 y = y1; y < y2; y++)
	{
		uint32* pixel = (uint32*)memory + x1 + (y * width);
		for (int32 x = x1; x < x2; x++)
		{
			*pixel++ = colour;
		}
	}
}

void Graphics::DrawRect(float x1, float y1, float x2, float y2, uint32 colour)
{
	DrawRectP(
		screenToPxX(x1), 
		screenToPxY(y1), 
		screenToPxX(x2), 
		screenToPxY(y2), 
		colour
		);
}