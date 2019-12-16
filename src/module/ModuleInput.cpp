#include "main/Globals.h"
#include "main/Application.h"
#include "ModuleInput.h"
#include "ModuleImGui.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"
#include "ModuleScene.h"
#include "SDL.h"
#include <string>

// Called before render is available
bool ModuleInput::Init()
{
	App->imgui->AddLog("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->imgui->AddLog("SDL_EVENTS could not initialize! SDL_Error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
	static SDL_Event event;
	memset(windowEvents, false, WE_COUNT * sizeof(bool));
	keyboard = SDL_GetKeyboardState(NULL);
	std::string path;
	std::string extension;
	
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type) {
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
				break;

			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
				case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_SHOWN:
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				case SDL_WINDOWEVENT_MAXIMIZED:
				case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
				break;

			case SDL_DROPFILE:
				path = event.drop.file;
				extension = path.substr(path.find_last_of(".") + 1);
				if (extension.compare("png") == 0 || extension.compare("bmp") == 0 || extension.compare("jpg") == 0 || extension.compare("tif") == 0) {
					App->texture->LoadTexture(path);
				}
				if (extension.compare("fbx") == 0 || extension.compare("FBX") == 0) {
					App->model->LoadModel(path);
				}
				break;
			
			case SDL_MOUSEMOTION :
				mouse_motion.x = (float) (event.motion.xrel / SCREEN_SIZE);
				mouse_motion.y = (float) (event.motion.yrel / SCREEN_SIZE);
				break;

			case SDL_MOUSEWHEEL :
				mouse_scroll.x = (float) event.wheel.x;
				mouse_scroll.y = (float) event.wheel.y;
				break;
			case SDL_KEYDOWN:
				currentKey = SDL_GetKeyName(event.key.keysym.sym);
				break;


		}
	}
	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE)) {
		return UPDATE_STOP;

	}

	return UPDATE_CONTINUE;
}


update_status ModuleInput::Update() {
	SDL_PumpEvents();
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp() {
	App->imgui->AddLog("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
