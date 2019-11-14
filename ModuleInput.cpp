#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleImGui.h"
#include "SDL.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	App->imgui->AddLog("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->imgui->AddLog("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
	static SDL_Event event;

	SDL_PumpEvents();

	keyboard = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&event) != 0)
	{
		mouse_down = false;
		if (event.type == SDL_MOUSEMOTION) {
			mouse_motion.x = event.motion.xrel / SCREEN_SIZE;
			mouse_motion.y = event.motion.yrel / SCREEN_SIZE;
			mouse.x = event.motion.x / SCREEN_SIZE;
			mouse.y = event.motion.y / SCREEN_SIZE;
			mouse_down = true;
		}
		if (event.type == SDL_MOUSEWHEEL) {
			mouse_scroll.x = event.wheel.x / SCREEN_SIZE;
			mouse_scroll.y = event.wheel.y / SCREEN_SIZE;
			LOG("Scroll x: %f", mouse_scroll.x);
			LOG("Scroll x: %y", mouse_scroll.y);
		}
		if (event.type == SDL_DROPFILE) {
			LOG("Path : %s", event.drop.file);
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	App->imgui->AddLog("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

const float2& ModuleInput::GetMousePosition() const
{
	return mouse;
}

const float2& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}

const float2& ModuleInput::GetMouseScroll() const
{
	return mouse_scroll;
}

const bool& ModuleInput::IsMouseDown() const
{
	return mouse_down;
}
