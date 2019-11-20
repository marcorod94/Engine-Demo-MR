#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"

#define NUM_MOUSE_BUTTONS 5

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();
	Uint8 GetKey(int id) const
	{
		return keyboard[id];
	}
	Uint8 GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}
	const float2& GetMouseMotion() const;
	const float2& GetMousePosition() const;
	const float2& GetMouseScroll() const;

private:
	const Uint8 *keyboard = NULL;
	Uint8	mouse_buttons[NUM_MOUSE_BUTTONS] = { 0,0,0,0,0 };
	float2 mouse_motion;
	float2 mouse;
	float2 mouse_scroll;
};