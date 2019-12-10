#ifndef ModuleScene_h
#define ModuleScene_h
#include "Module.h"
#include "main/GameObject.h"
#include "main/Globals.h"
#include "GL/glew.h"

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
	bool CleanUp();
};
#endif

