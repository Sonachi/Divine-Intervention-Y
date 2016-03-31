#pragma once

#include <Windows.h>

class Time
{
public:

	static void init()
	{
		QueryPerformanceFrequency(&ticksPerSecond);

		startTime = getHardwareTickSpeed();
	}

	static inline float elapsedTimeSeconds()
	{
		return ticksInSeconds(getCurrentTicks());
	}

	static inline float elapsedTimeMiliseconds()
	{
		return ticksToMiliseconds(getCurrentTicks());
	}

private:
	Time();
	~Time();

	static LARGE_INTEGER ticksPerSecond;
	static LARGE_INTEGER hardwareSpeed;

	static LARGE_INTEGER startTime;

	static inline float ticksInSeconds(LARGE_INTEGER _ticks)
	{
		return ((float)_ticks.QuadPart) / ((float)ticksPerSecond.QuadPart);
	}

	static inline float ticksToMiliseconds(LARGE_INTEGER _ticks)
	{
		return ticksInSeconds(_ticks) * 1000.0f;
	}

	static inline LARGE_INTEGER getCurrentTicks()
	{
		LARGE_INTEGER v;
		v.QuadPart = getHardwareTickSpeed().QuadPart;
		return v;
	}

	static inline LARGE_INTEGER getHardwareTickSpeed()
	{
		LARGE_INTEGER speed;
		QueryPerformanceCounter(&speed);
		return speed;
	}
};

