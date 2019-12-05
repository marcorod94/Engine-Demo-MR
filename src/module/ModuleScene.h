#ifndef ModuleScene_h
#define ModuleScene_h
#include "Module.h"
#include "main/GameObject.h"
#include "main/Globals.h"
#include "GL/glew.h"

class ModuleScene : public Module {
public:
	GameObject* root;
	bool showGrid = true;
	bool showAxis = true;
	ModuleScene() {}
	~ModuleScene() {}

	bool Init();
	update_status PreUpdate();
	update_status Update();
	GameObject* CreateGameObject();
	void SetupMesh(Mesh&);
	bool CleanUp();
private:
	void DrawGrid() const;
	void DrawAxis() const;
	void DrawMesh(Mesh&);
private:
	GLuint vbo, ebo, vao;
};
#endif

