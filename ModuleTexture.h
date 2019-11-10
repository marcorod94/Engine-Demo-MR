#ifndef ModuleTexture_h
#define ModuleTexture_h

#include "Module.h"
#include "Globals.h"
#include "IL/il.h"


class ModuleTexture :
	public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	bool CleanUp();
	Texture LoadTexture(const char*);
private:
	ILuint imageName;
};
#endif
