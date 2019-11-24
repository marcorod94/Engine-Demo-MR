#ifndef ModuleInput_h
#define ModuleInput_h

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "SDL.h"

typedef uint8_t Uint8;
enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

class ModuleInput : public Module
{
public:
	
	ModuleInput() {}
	~ModuleInput() {}

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	Uint8 GetKey(const int id) const {
		return keyboard[id];
	}
	Uint8 GetMouseButtonDown(const int id) const {
		return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(id));
	}
	bool GetWindowEvent(const EventWindow ev) const {
		return windowEvents[ev];
	}
	const float2& GetMouseMotion() const;
	const float2& GetMouseScroll() const;

private:
	bool windowEvents[WE_COUNT];
	const Uint8* keyboard;
	float2 mouse_motion = float2::zero;
	float2 mouse_scroll = float2::zero;
};
#endif
