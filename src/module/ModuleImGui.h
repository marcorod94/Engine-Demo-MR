#ifndef ModuleImGui_h
#define ModuleImGui_h

#include "Module.h"
#include "main/Globals.h"
#include "imgui.h"
#include <vector>
#include "ImGuizmo.h"

class GameObject;
enum class ShapeType;
class ModuleImGui : public Module {
public:
	ModuleImGui() {}
	~ModuleImGui() {}

	bool Init();
	update_status Update();
	bool CleanUp();
	void AddLog(const char*, ...);

	const void DrawConsoleWindow();
	std::string selected = "";
	void ShowGizmosButtons();
	bool gizmo = false;
private: 
	bool show_demo_window = true;
	
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGuiTextBuffer buffer;
	bool scrollToBottom;
	char title[100];
	char frameInfo[2000];
	std::vector<float> fps_log{0.0F};
	std::vector<float> ms_log{0.0F};
	unsigned shape = 0;

	const bool useSnap = false;
	const float snap[3] = { 1.f, 1.f, 1.f };

	GameObject* sourceGO = nullptr;
	bool showMain = true;
	bool showInfo = false;
	bool showConsole = true;
	bool showModule = false;
	bool showAbout = false;
	bool showProperties = false;
	bool showHierarchy = true;

	const void ShowModulesWindow();
	const void ShowInformationWindow(ImGuiIO&);
	const void ShowTextures(std::vector<Texture>&);
	const void DrawHierarchy(const std::vector<GameObject*>&, int&);
	const void DrawShaderProperties();
	void LoadShapes(ShapeType s);
}; 
#endif 
