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
update_status ModuleInput::PreUpdate()
{
	static SDL_Event event;

	SDL_PumpEvents();

	keyboard = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&event) != 0)
	{
		
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			LOG("mouse down");
			mouse_buttons[event.button.button - 1] = 1;
		}

		if (event.type == SDL_MOUSEBUTTONUP) {
			LOG("mouse UP evento %d: valor %d: ", event.button.button, mouse_buttons[event.button.button - 1]);
			if (mouse_buttons[event.button.button - 1]) {
				mouse_buttons[event.button.button - 1] = 0;
			} else {
				mouse_buttons[event.button.button - 1] = 1;
			}
		}
		if (event.type == SDL_MOUSEMOTION) {
			mouse_motion.x = event.motion.xrel / SCREEN_SIZE;
			mouse_motion.y = event.motion.yrel / SCREEN_SIZE;
			mouse.x = event.motion.x / SCREEN_SIZE;
			mouse.y = event.motion.y / SCREEN_SIZE;
		}
		if (event.type == SDL_MOUSEWHEEL) {
			mouse_scroll.x = event.wheel.x;
			mouse_scroll.y = event.wheel.y;
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
