#include "ModuleTimer.h"
#include "SDL.h"


ModuleTimer::ModuleTimer()
{
	realTimeTimer = new Timer();
}


ModuleTimer::~ModuleTimer()
{
}

bool ModuleTimer::Init() {
	currentTime = SDL_GetTicks();
	previousTime += currentTime;


	realTimeTimer->StartTimer();

	return true;
}

update_status ModuleTimer::Update() {

	//for test purposes, 
	if (onPlay)
	{
		gameTime += (realTimeTimer->ReadTimer() - realTime) * timeScale;
	}
	realTime = realTimeTimer->ReadTimer();
	frameDelay = 1000 / FPS;
	dt = frameDelay / 1000;
	
	return UPDATE_CONTINUE;
}


void ModuleTimer::Start() {

	// = SDL_GetTicks();
	//currentTime = 0;
	//onPlay = true;
	frameStart = gameTime + (realTimeTimer->ReadTimer() - realTime) * timeScale;
	frameTime = realTimeTimer->ReadTimer();
}

void ModuleTimer::End() {

	deltaTime = realTimeTimer->ReadTimer() - frameStart;
	float timeToWait = 0.0f;

	if (timeToWait > 0.0f)
	{
		SDL_Delay(static_cast<Uint32>(timeToWait));
	}

}


float ModuleTimer::Play() {
	gameTime = 0.0f;
	onPause = false;
	onPlay = true;
	return currentTime;
}

float ModuleTimer::Stop() {

	onPlay = false;
	return currentTime;
}

void ModuleTimer::Pause() {
	onPause = true;
}



