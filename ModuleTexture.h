#ifndef ModuleTexture_h
#define ModuleTexture_h

#include "Module.h"
#include "Globals.h"
#include "IL/il.h"
#include "IL/ilu.h"


class ModuleTexture : public Module
{
public:
	std::vector<Texture> loadedTextures;
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	bool CleanUp();
	Texture LoadTexture(const char*);
private:
	ILuint imageName;
	ILinfo imageInfo;
};
#endif
