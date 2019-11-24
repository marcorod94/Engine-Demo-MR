#ifndef ModuleProgram_h
#define ModuleProgram_h

#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"

class ModuleProgram : public Module {
public:
	ModuleProgram() {}
	~ModuleProgram() {}

	bool Init();
	update_status Update();
	bool CleanUp();
	void LoadShaders(const char*, const char*);
public:
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint program = 0;
};
#endif
