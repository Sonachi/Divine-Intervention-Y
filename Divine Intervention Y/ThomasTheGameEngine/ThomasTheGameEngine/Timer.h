#pragma once
#include "Time.h"
class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void pause();
	void updateTimer();
	float millisecondsPassed();

private:
	bool paused;
	LARGE_INTEGER start_time;
	LARGE_INTEGER ticks;
	LARGE_INTEGER pause_time;
};

