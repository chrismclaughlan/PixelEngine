#pragma once
#include <Windows.h>
#include "types.h"

class FPS
{
private:
	LARGE_INTEGER frequency;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime = {};
	LARGE_INTEGER elapsedMicoseconds= {};
public:
	FPS()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startTime);
	}

	void Update()
	{
		QueryPerformanceCounter(&endTime);
		elapsedMicoseconds.QuadPart = endTime.QuadPart - startTime.QuadPart;

		elapsedMicoseconds.QuadPart *= 1000000;
		elapsedMicoseconds.QuadPart /= frequency.QuadPart;

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startTime);
	}

	const float getFps()
	{
		return 1000000.0 / elapsedMicoseconds.QuadPart;
	}
};