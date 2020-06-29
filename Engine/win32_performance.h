#pragma once
#include "hwindows.h"  // first
#include "types.h"
#include <chrono>
#include <thread>
#include <assert.h>

class Performance
{
public:
	LARGE_INTEGER frequency;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime = {};
	LARGE_INTEGER elapsedMicoseconds= {};
	LARGE_INTEGER elapsedMicosecondsCarry= {};
public:
	Performance()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startTime);
	}
public:
	void Update()
	{
		assert(frequency.QuadPart > 0);

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
		assert(target > 0);
		while (getFps() > target)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / target));
			QueryPerformanceCounter(&elapsedMicoseconds);
		}
	}

	const float getFps()
	{
		if (elapsedMicoseconds.QuadPart > 0)
		{
			return (float)(1000000.0 / elapsedMicoseconds.QuadPart);
		}
		else
		{
			return 0.0;
		}
	}

	const bool hasTimePassed(float seconds)
	{
		if (elapsedMicoseconds.QuadPart > 0)
		{
			// check if second has passed
			if (elapsedMicosecondsCarry.QuadPart > 1000000.0 * seconds)
			{
				elapsedMicosecondsCarry.QuadPart = 0;
				return true;
			}
			else
			{
				elapsedMicosecondsCarry.QuadPart += elapsedMicoseconds.QuadPart;
			}
		}

		return false;
	}

	const uint8 getFrameMask()
	{
		return startTime.QuadPart & (long)1 << 7;
	}
};