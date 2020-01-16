#ifndef __ModuleTimer_H__
#define __ModuleTimer_H__

#include "Module.h"
#include "util/Timer.h"

class ModuleTimer : public Module
{
public:
	ModuleTimer();
	~ModuleTimer();

	//bool isGlobal = false;

	bool onPlay = false;
	bool onPause = false;

	float timeScale = 1.0f;
	float previousTime = 0;
	float currentTime = 0;
	float FPS = 60;
	int frameCount = 0;
	int frameCounter = 0;
	float dt = 0;
	float elapsed = 0;
	float lag = 0;

	float frameStart;
	float frameTime;
	float frameDelay;

	void Start();
	void End();
	float Stop();
	float Play();
	void Pause();
	
	bool Init();
	update_status Update();

	Timer* realTimeTimer;
	float deltaTime = 0.0f;
	float gameTime = 0.0f;
	float realTime = 0.0f;
	float realDeltaTime = 0.0f;
};
#endif 
