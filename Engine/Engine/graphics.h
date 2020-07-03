#pragma once
#include <vector>
#include "types.h"

// TODO clean namespace
#define calculate_scales()\
float ratio = 1080 / nscale_y;\
scale_x = nscale_x * ratio;\
scale_y = nscale_y * ratio;\

class Graphics
{
	// TODO error handling
public:
	int32 width;
	int32 height;

protected:
	void* memory;
	float scale_x;
	float scale_y;

public:
	int32 getWidth() { return width; }
	int32 getHeight() { return height; }

	static void Clamp(int32, int32*, int32);  // inline
	static void Clamp(float, float*, float);  // inline
	static void Clamp(double, double*, double);  // inline
	inline void ScaleX(float*);
	inline void ScaleY(float*);

	float pxToScreenX(int32 a);  // inline
	float pxToScreenY(int32 a);  // inline
	int32 screenToPxX(float a);  // inline
	int32 screenToPxY(float a);  // inline

public:
	void ClearScreen(uint32 colour);
	void DrawPoint(float x, float y, uint32 colour);
	void DrawRectP(int32 x1, int32 y1, int32 x2, int32 y2, uint32 colour);
	void DrawRect(float x, float y, float w, float h, uint32 colour);
};