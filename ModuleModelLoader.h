#ifndef ModuleModelLoader_h
#define ModuleModelLoader_h
#include "Globals.h"
#include "Module.h"
#include <assimp/scene.h>
#include <vector>
class ModuleModelLoader :
	public Module
{
public:
	std::vector<Mesh> meshes;

	ModuleModelLoader();
	~ModuleModelLoader();
	bool Init();
	bool CleanUp();
	void LoadModel(const char*);
	void UpdateTexture(Texture&);

private:
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, char* );
};
#endif
