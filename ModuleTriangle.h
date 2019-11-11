#ifndef ModuleTriangle_h
#define ModuleTriangle_h
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"

class ModuleTriangle : public Module {
public:
	ModuleTriangle();
	~ModuleTriangle();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
private:
	void ShowGrid();
	void ShowAxis();
private:
	GLuint vbo, ebo, vao;
};
#endif

