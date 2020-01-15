#ifndef ModuleScene_h
#define ModuleScene_h
#include "Module.h"
#include "main/GameObject.h"
#include "main/Globals.h"
#include "GL/glew.h"
#include "rapidjson/document.h"

struct ImVec2;

class ModuleScene : public Module {
public:
	GameObject* root;
	ModuleScene() {}
	~ModuleScene() {}

	bool Init();
	update_status PreUpdate();
	update_status Update();
	GameObject* CreateGameObject(const std::string&) const;
	GameObject* CreateGameObject(const std::string&, const float3&, const Quat&) const;
	void PickObject(const ImVec2 &winSize, const ImVec2 &winPos);
	bool CleanUp();
	void LoadScene();
	void SaveScene();
private:
	rapidjson::Document config;
};
#endif

