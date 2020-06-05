#pragma once
#include <windows.h>
#include "types.h"

const float DEFAULT_SCALE = 0.01f;

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
	float scale;
public:
	Render();
	Render(float);
	Render(RenderState*, float);
	//~Render();

	RenderState* getRenderState() { return &state; }
	void sizeChangeWin32(RECT*);

	inline void Clamp(int32, int32*, int32);
	inline void Scale(float*);

	// Business methods //

	void DrawGrid(int32, int32);

	void ClearScreen(uint32);

	// Point
	inline void DrawPoint(int32, int32, uint32);

	// Line
	void DrawLineP(int32, int32, int32, int32, uint32);
	void DrawLine(float, float, float, float, uint32);

	// Rectangle
	void DrawRectP(int32, int32, int32, int32, uint32);
	void DrawRect(float, float, float, float, uint32);
	void DrawRectOutlineP(float, float, float, float, uint32, float);
	void DrawRectOutline(float, float, int32, int32, uint32, float);

	// Triangle
	void DrawTriangleP(int32, int32, int32, int32, uint32);
	void DrawTriangle(float, float, float, float, uint32);
};