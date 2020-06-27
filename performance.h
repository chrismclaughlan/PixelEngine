#pragma once
#include <Windows.h>
#include "types.h"
#include <chrono>
#include <thread>

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

	// Not entirely accurate
	void LimitFps(int32 target)
	{
		while (getFps() > target)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / target));
			QueryPerformanceCounter(&elapsedMicoseconds);
		}
	}

	const float getFps()
	{
		return 1000000.0 / elapsedMicoseconds.QuadPart;
	}
};