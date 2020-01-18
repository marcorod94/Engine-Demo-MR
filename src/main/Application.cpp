#include "Application.h"
#include "module/ModuleWindow.h"
#include "module/ModuleRender.h"
#include "module/ModuleInput.h"
#include "module/ModuleScene.h"
#include "module/ModuleProgram.h"
#include "module/ModuleImGui.h"
#include "module/ModuleTexture.h"
#include "module/ModuleCamera.h"
#include "module/ModuleModel.h"
#include "module//ModuleDebugDraw.h"
#include "module/ModuleFileSystem.h"

using namespace std;

Application::Application() {
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(input = new ModuleInput());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(debugDraw = new ModuleDebugDraw());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(filesys = new ModuleFileSystem());
	modules.push_back(model = new ModuleModel());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(imgui = new ModuleImGui());
}

Application::~Application() {
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init() {
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update() {
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp() {
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
