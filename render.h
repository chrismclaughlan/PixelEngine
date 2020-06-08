#pragma once
#include <windows.h>
#include <vector>
#include "types.h"

const float DEFAULT_SCALE_X = 16;
const float DEFAULT_SCALE_Y = 9;

//struct Point
//{
//	int32 x;
//	int32 y;
//	//uint32 colour;
//};
//
//struct PolygonV3
//{
//	//int32 x[3];
//	//int32 y[3];
//	Point points[3];
//};

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

	RenderState* getRenderState() { return &state; }
	void sizeChangeWin32(RECT*);

	inline void Clamp(int32, int32*, int32);
	inline void ScaleX(float*);
	inline void ScaleY(float*);

	// Business methods //

	void DrawGrid(float, float);
	void DrawGridV2(float, float);

	void ClearScreen(uint32);

	// Point
	inline void DrawPointP(int32, int32, uint32);
	void DrawPoint(float, float, uint32);

	// Line
	void DrawLineP(int32, int32, int32, int32, uint32);
	void DrawLine(float, float, float, float, uint32);
	//inline static std::vector<Point> ReturnLine(Point, Point);
	//static std::vector<Point> ReturnLine(int32, int32, int32, int32);
	//void DrawPolygon(PolygonV3);

	// Rectangle
	void DrawRectP(int32, int32, int32, int32, uint32);
	void DrawRect(float, float, float, float, uint32);
	void DrawRectOutlineP(int32, int32, int32, int32, uint32, int32);
	void DrawRectOutline(float, float, float, float, uint32, float);
	void DrawRectOutlineCentreP(int32, int32, int32, int32, uint32, float);
	void DrawRectOutlineCentre(float, float, float, float, uint32, float);

	// Triangle
	//void DrawTriangleP(int32, int32, int32, int32, uint32);
	//void DrawTriangle(float, float, float, float, uint32);
};