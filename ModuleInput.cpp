#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleImGui.h"
#include "ModuleTexture.h"
#include "ModuleModelLoader.h"
#include "ModuleTriangle.h"
#include "SDL.h"
#include <string>

ModuleInput::ModuleInput() {}

// Destructor
ModuleInput::~ModuleInput() {}

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

	keyboard = SDL_GetKeyboardState(NULL);

	std::string path;
	std::string extension;
	
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type) {

			case SDL_DROPFILE:
				path = event.drop.file;
				extension = path.substr(path.find_last_of(".") + 1);
				if (extension.compare("png") == 0 || extension.compare("bmp") == 0 || extension.compare("jpg") == 0) {
					App->texture->LoadTexture(path);
				}
				if (extension.compare("fbx") == 0 || extension.compare("FBX") == 0) {
					App->modelLoader->LoadModel(path);
					for (unsigned int i = 0; i < App->modelLoader->meshes.size(); i++) {
						App->triangle->SetupMesh(App->modelLoader->meshes[i]);
					}
				}
				break;
			
			case SDL_MOUSEMOTION :
				mouse_motion.x = event.motion.xrel / SCREEN_SIZE;
				mouse_motion.y = event.motion.yrel / SCREEN_SIZE;
				break;

			case SDL_MOUSEWHEEL :
				mouse_scroll.x = event.wheel.x;
				mouse_scroll.y = event.wheel.y;
				break;

		}
	}

	return UPDATE_CONTINUE;
}


update_status ModuleInput::Update() {
	SDL_PumpEvents();
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	App->imgui->AddLog("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

const float2& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}

const float2& ModuleInput::GetMouseScroll() const
{
	return mouse_scroll;
}
