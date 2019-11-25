#ifndef ModuleImGui_h
#define ModuleImGui_h

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include <vector>

class ModuleImGui : public Module {
public:
	ModuleImGui() {}
	~ModuleImGui() {}

	bool Init();
	update_status Update();
	bool CleanUp();
	void AddLog(const char*, ...);
private: 
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGuiTextBuffer buffer;
	bool scrollToBottom;
	char title[25];
	std::vector<float> fps_log{0.0F};
	std::vector<float> ms_log{0.0F};

	bool showMain = true;
	bool showInfo = false;
	bool showConsole = false;
	bool showModule = false;
	bool showAbout = false;
	bool showProperties = false;

	const void ShowModulesWindow();
	const void ShowInformationWindow(ImGuiIO&);
	const void ShowTextures(std::vector<Texture>&);
}; 
#endif 
