#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();
	void UpdateResizable();
	void UpdateFullScreen();
	void UpdateScreenSize();

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
	int screenWidth = 640;
	int screenHeight = 480;
	bool fullScreen = false;
	bool resizable = false;
};

#endif // __ModuleWindow_H__