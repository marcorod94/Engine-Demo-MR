#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleImGui.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	App->imgui->AddLog("Init SDL window & surface\n");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		App->imgui->AddLog("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(fullScreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (resizable) {
			flags |= SDL_WINDOW_RESIZABLE;
		}
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, flags);

		if(window == NULL)
		{
			App->imgui->AddLog("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	App->imgui->AddLog("Destroying SDL window and quitting all SDL systems\n");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}
void ModuleWindow::UpdateResizable() {
	if (resizable) {
		SDL_SetWindowResizable(window, SDL_TRUE);
	} else {
		SDL_SetWindowResizable(window, SDL_FALSE);
	}

}

void ModuleWindow::UpdateFullScreen() {
	if (fullScreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	} else {
		SDL_SetWindowFullscreen(window, 0);
	}

}

void ModuleWindow::UpdateScreenSize() {
	SDL_SetWindowSize(window, screenWidth, screenHeight);
}

