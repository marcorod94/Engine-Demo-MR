#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();
	Uint8 GetKey(int id) const
	{
		return keyboard[id];
	}
	const float2& GetMouseMotion() const;
	const float2& GetMousePosition() const;

private:
	const Uint8 *keyboard = NULL;
	float2 mouse_motion;
	float2 mouse;
};