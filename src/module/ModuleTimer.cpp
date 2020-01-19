#include "ModuleTimer.h"
#include "SDL.h"


ModuleTimer::ModuleTimer()
{
	
}


ModuleTimer::~ModuleTimer()
{
}

bool ModuleTimer::Init() {
	realTimeTimer = new Timer();
	currentTime = SDL_GetTicks();
	previousTime += currentTime;


	realTimeTimer->StartTimer();

	return true;
}
update_status ModuleTimer::PreUpdate()
{
	return UPDATE_CONTINUE;
}
update_status ModuleTimer::Update() {

	if (onPlay && !onPause)
	{
		gameTime += (realTimeTimer->ReadTimer() - realTime) * timeScale;
	}
	realTime = realTimeTimer->ReadTimer();
	
	
	return UPDATE_CONTINUE;
}
update_status ModuleTimer::PostUpdate()
{
	return UPDATE_CONTINUE;
}

void ModuleTimer::Start() {
	frameStart = gameTime + (realTimeTimer->ReadTimer() - realTime) * timeScale;
	frameTime = realTimeTimer->ReadTimer();
}

void ModuleTimer::End() {
	//frameCount = 0;
	deltaTime = (realTimeTimer->ReadTimer() - frameStart) * timeScale;
	realDeltaTime = realTimeTimer->ReadTimer() - frameTime;
	float timeToWait = 1000.f / limitFPS - realDeltaTime;
	frameDelay = 1000 / FPS;
	dt = frameDelay / 1000;
	if (timeToWait > 0.0f)
	{
		SDL_Delay(static_cast<Uint32>(timeToWait));
	}
	gameTime += (realDeltaTime + timeToWait);
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

void ModuleTimer::SetTimeScale(float timeScale)
{
	this->timeScale = timeScale;
}



