#ifndef ModuleTexture_h
#define ModuleTexture_h

#include "Module.h"
#include "main/Globals.h"
#include "component/Material.h"
#include "util/MaterialImporter.h"

struct Texture;

class GameObject;
class ModuleTexture : public Module
{
public:
	//std::vector<Texture> loadedTextures;
	std::vector<Texture*> loadedTextures;

	MaterialImporter* importer = new MaterialImporter();
	ModuleTexture() {}
	~ModuleTexture() {}

	bool Init();
	bool CleanUp();
	unsigned LoadTexture(const char*);
	Material* CreateMaterial(GameObject*);
	void DrawTextureButton(Texture**);
	Texture* DrawTextureSelector();
	void DrawTextureSelector(unsigned*);
	void DrawTexture(const Texture*);

	void LoadSkybox(std::vector<std::string> faces)const;
	bool isLoaded;
private:
	bool drawSelector = false;
};
#endif
