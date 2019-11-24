#ifndef ModuleWindow_h
#define ModuleWindow_h

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
	void UpdateResizable() const;
	void UpdateFullScreen() const;
	void UpdateScreenSize() const;

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
	int maxScreenWidth, maxScreenHeight, minScreenWidth, minScreenHeight, screenWidth, screenHeight;
	bool fullScreen = false;
	bool resizable = false;
private:
	float aspectRatio = 1.8f;
};

#endif
