#ifndef ModuleImGui_h
#define ModuleImGui_h

#include "Module.h"
#include "Globals.h"
#include "imgui.h"

class ModuleImGui : public Module {
public:
	ModuleImGui();
	~ModuleImGui();

	bool Init();
	update_status Update();
	bool CleanUp();
public: 
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}; 
#endif 
