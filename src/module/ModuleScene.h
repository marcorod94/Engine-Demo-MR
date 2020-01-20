#ifndef ModuleScene_h
#define ModuleScene_h
#include "Module.h"
#include "main/GameObject.h"
#include "main/Globals.h"
#include "GL/glew.h"
#include "rapidjson/document.h"
#include "util/AABBTree.h"
#define SCENE_FILE "Scene.json"

struct ImVec2;

class ModuleScene : public Module {
public:
	GameObject* root;
	ModuleScene() {}
	~ModuleScene() {}

	bool Init();
	update_status PreUpdate();
	update_status Update();
	GameObject* CreateGameObject(const char*) const;
	GameObject* CreateGameObject(const char*, const float3*, const Quat*) const;
	void PickObject(const ImVec2 &winSize, const ImVec2 &winPos);
	bool CleanUp();
	void LoadScene();
	void SaveScene();
	AABBTree* abbTree = nullptr;
	void DrawAABBTree();
private:
	rapidjson::Document config;
};
#endif

