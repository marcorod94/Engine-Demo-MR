#ifndef __microSTimer_H__
#define __microSTimer_H__
#include "SDL/SDL.h"

class microSTimer
{
public:
	microSTimer();
	~microSTimer();


	void StartTimer();
	float ReadTimer() const;
	float StopTimer();

	bool isCounting = false;
	float previousTime = 0.0f;
	float currentTime = 0.0f;
	
};

#endif 