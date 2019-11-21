#ifndef ModuleTexture_h
#define ModuleTexture_h

#include "Module.h"
#include "Globals.h"
#include "IL/il.h"
#include "IL/ilu.h"

#define TEXTURE_PATH "Textures/"
#define DEFAULT_TEXTURE "checkers.jpg"

class ModuleTexture : public Module
{
public:
	std::vector<Texture> loadedTextures;
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	bool CleanUp();
	Texture LoadTexture(std::string&);
private:
	ILuint imageName;
	ILinfo imageInfo;
};
#endif
