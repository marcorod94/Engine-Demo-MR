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
	unsigned LoadTexture(std::string&);
	Material* CreateMaterial();
	void DrawTexture(unsigned&);
	void DrawTextureSelector(unsigned&);
	void DrawTexture(Texture&);

	void LoadSkybox(std::vector<std::string> faces)const;
	bool isLoaded;
private:
	ILuint imageName;
	ILinfo imageInfo;
	bool drawSelector = false;
};
#endif
