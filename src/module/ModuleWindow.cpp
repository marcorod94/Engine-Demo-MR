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
	App->imgui->AddLog("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		App->imgui->AddLog("SDL_VIDEO could not initialize! SDL_Error: %s", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		maxScreenWidth = DM.w;
		maxScreenHeight = DM.h;
		minScreenWidth = screenWidth = (int) (DM.w / aspectRatio);
		minScreenHeight = screenHeight = (int) (DM.h / aspectRatio);

		if(fullScreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (resizable) {
			flags |= SDL_WINDOW_RESIZABLE;
		}
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, flags);

		if(window == NULL)
		{
			App->imgui->AddLog("Window could not be created! SDL_Error: %s", SDL_GetError());
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
	App->imgui->AddLog("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}
void ModuleWindow::UpdateResizable() const {
	if (resizable) {
		SDL_SetWindowResizable(window, SDL_TRUE);
	} else {
		SDL_SetWindowResizable(window, SDL_FALSE);
	}

}

void ModuleWindow::UpdateFullScreen() const {
	if (fullScreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	} else {
		SDL_SetWindowFullscreen(window, 0);
	}

}

void ModuleWindow::UpdateScreenSize() const {
	SDL_SetWindowSize(window, screenWidth, screenHeight);
}

