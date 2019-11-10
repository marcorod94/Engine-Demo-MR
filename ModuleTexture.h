#ifndef ModuleTexture_h
#define ModuleTexture_h

#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"
#include "IL/il.h"

class ModuleTexture :
	public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	bool CleanUp();
	void LoadTexture(const char*);
public:
	GLuint textureID;
	ILuint imageName;

};
#endif
