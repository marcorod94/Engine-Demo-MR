#ifndef ModuleTexture_h
#define ModuleTexture_h

#include "Module.h"
#include "main/Globals.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "component/Material.h"
#define TEXTURE_PATH "Textures/"
#define DEFAULT_TEXTURE "checkers.jpg"


class ModuleTexture : public Module
{
public:
	std::vector<Texture> loadedTextures;
	ModuleTexture() {}
	~ModuleTexture() {}

	bool Init();
	bool CleanUp();
	Texture LoadTexture(std::string&);
	Material* CreateMaterial();
private:
	ILuint imageName;
	ILinfo imageInfo;
};
#endif
