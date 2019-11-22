#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "SDL.h"

typedef uint8_t Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	Uint8 GetKey(int id) const
	{
		return keyboard[id];
	}
	Uint8 GetMouseButtonDown(int id) const
	{
		return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(id));
	}
	const float2& GetMouseMotion() const;
	const float2& GetMouseScroll() const;

private:
	const Uint8*	keyboard;
	float2 mouse_motion = float2::zero;
	float2 mouse_scroll = float2::zero;
};