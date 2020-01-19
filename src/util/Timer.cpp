#include "Timer.h"



Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::StartTimer()
{
	previousTime = SDL_GetTicks();
	currentTime = 0.0f;
	isCounting = true;
}


float Timer::ReadTimer() const
{
	if (isCounting)
	{
		float elapsed = SDL_GetTicks() - previousTime;
		return elapsed;
	}
	return currentTime;
}

float Timer::StopTimer()
{
	currentTime = SDL_GetTicks() - previousTime;
	isCounting = false;
	return currentTime;
}
