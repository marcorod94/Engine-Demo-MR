#ifndef __Timer_H__
#define __Timer_H__

#include "SDL/SDL.h"
class Timer
{
public:
	Timer();
	~Timer();

	void StartTimer();
	float ReadTimer() const;
	float StopTimer();

	bool isCounting = false;
	float previousTime = 0.0f;
	float currentTime = 0.0f;
};
#endif 
