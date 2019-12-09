#ifndef ModuleProgram_h
#define ModuleProgram_h

#include "Module.h"
#include "main/Globals.h"
#include "GL/glew.h"
enum class ProgramType
{
	Default = 0,
	Flat,
	Gouraud,
	Phong,
	Blinn,
	Count
};
class ModuleProgram : public Module {
public:
	unsigned programs[int(ProgramType::Count)];
	ModuleProgram() {}
	~ModuleProgram() {}

	bool Init();
	bool CleanUp();
	unsigned CreateProgram(const char*, const char*);
	std::string* LoadFile(const char*);
	bool Compile(unsigned, std::string*);
};
#endif
