#pragma once
#include <windows.h>
#include <vector>
#include "types.h"

namespace render
{
const float DEFAULT_SCALE_X = 16;
const float DEFAULT_SCALE_Y = 9;

struct RenderState
{
	int32 width;
	int32 height;
	void* memory;
	BITMAPINFO bitmapinfo;
};

class Render
{
private:
	RenderState state;
	float scale_x;
	float scale_y;

public:
	Render();
	Render(int32, int32);
	Render(RenderState*, int32, int32);
	//~Render();

	int32 getWidth() { return state.width; }
	int32 getHeight() { return state.height; }

	RenderState* getRenderState() { return &state; }
	void sizeChangeWin32(RECT*);

	inline void Clamp(int32, int32*, int32);
	inline void Clamp(float, float*, float);
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
}  // namespace render