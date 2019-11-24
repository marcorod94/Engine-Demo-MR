#ifndef ModuleScene_h
#define ModuleScene_h
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"

class ModuleScene : public Module {
public:
	ModuleScene() {}
	~ModuleScene() {}

	bool Init();
	update_status PreUpdate();
	update_status Update();
	void SetupMesh(Mesh&);
	bool CleanUp();
private:
	void ShowGrid() const;
	void ShowAxis() const;
	void DrawMesh(Mesh&);
private:
	GLuint vbo, ebo, vao;
};
#endif
