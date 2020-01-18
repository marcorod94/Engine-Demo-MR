#ifndef ModuleTexture_h
#define ModuleTexture_h

#include "Module.h"
#include "main/Globals.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "component/Material.h"
#include "util/MaterialImporter.h"
#define TEXTURE_PATH "Textures/"
#define DEFAULT_TEXTURE "checkers.jpg"

struct Texture;

class GameObject;
class ModuleTexture : public Module
{
public:
	std::vector<Texture> loadedTextures;

	MaterialImporter* importer = new MaterialImporter();
	ModuleTexture() {}
	~ModuleTexture() {}

	bool Init();
	bool CleanUp();
	unsigned LoadTexture(std::string&);
	Material* CreateMaterial(GameObject*);
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
