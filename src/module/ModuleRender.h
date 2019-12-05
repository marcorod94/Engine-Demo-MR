#ifndef ModuleRender_h
#define ModuleRender_h
#include "Module.h"
#include "main/Globals.h"
#include "SDL.h"


class ModuleRender : public Module {
public:
	ModuleRender() {}
	~ModuleRender() {}

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	SDL_GLContext glContext;
};
#endif
