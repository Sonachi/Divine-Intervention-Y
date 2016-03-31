#include "Timer.h"
#include <iostream>


Timer::Timer()
{
	paused = false;

	ticks.QuadPart = 0;
	pause_time.QuadPart = 0;
	start_time.QuadPart = 0;
}


Timer::~Timer()
{
}

void Timer::start()
{
	if (!paused)
	{
		start_time.QuadPart = Time::elapsedTimeMiliseconds();
		ticks = start_time;
	}
	if (paused)
	{
		start_time.QuadPart = Time::elapsedTimeMiliseconds() - pause_time.QuadPart;
		paused = false;
	}
}

void Timer::pause()
{
	if (!paused)
	{
		paused = true;
		pause_time.QuadPart = Time::elapsedTimeMiliseconds() - start_time.QuadPart;
	}
}
void Timer::updateTimer()
{
	if (!paused)
	{
		ticks.QuadPart = Time::elapsedTimeMiliseconds();
		std::cout << "Seconds: " << ticks.QuadPart - start_time.QuadPart << "\n";
	}
}

float Timer::millisecondsPassed()
{
	return (float)ticks.QuadPart - (float)start_time.QuadPart;
}
