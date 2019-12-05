#ifndef Application_h
#define Application_h

#include<list>
#include "Globals.h"
#include "module/Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleScene;
class ModuleProgram;
class ModuleImGui;
class ModuleTexture;
class ModuleCamera;
class ModuleModel;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleScene* scene = nullptr;
	ModuleProgram* program = nullptr;
	ModuleImGui* imgui = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleModel* model = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;
#endif
