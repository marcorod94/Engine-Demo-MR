#include "microSTimer.h"


const static float frequency = SDL_GetPerformanceFrequency();
microSTimer::microSTimer()
{
}
microSTimer::~microSTimer()
{
}

void microSTimer::StartTimer()
{
	previousTime = SDL_GetPerformanceCounter();
	currentTime = 0.0f;
	isCounting = true;
}


float microSTimer::ReadTimer() const
{
	if (isCounting)
	{
		float elapsed = (SDL_GetPerformanceCounter() - previousTime) * 1000 / frequency;
		return elapsed;
	}
	return currentTime;
}

float microSTimer::StopTimer()
{
	currentTime = (SDL_GetPerformanceCounter() - previousTime) * 1000 / frequency;
	isCounting = false;
	return currentTime;
}
