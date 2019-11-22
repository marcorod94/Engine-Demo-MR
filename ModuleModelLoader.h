#ifndef ModuleModelLoader_h
#define ModuleModelLoader_h
#include "Globals.h"
#include "Module.h"
#include <assimp/scene.h>
#include <vector>
#include <string>
class ModuleModelLoader :
	public Module
{
public:
	std::vector<Mesh> meshes;
	aiVector3D scaling;
	aiVector3D position;
	aiVector3D rotation;
	int totalPrimitives = 0;
	int totalVertex = 0;
	int totalMaterials = 0;
	ModuleModelLoader();
	~ModuleModelLoader();
	bool Init();
	bool CleanUp();
	void LoadModel(std::string&);
	void UpdateTexture(Texture&);

private:
	std::string directory;
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, const char* );
	int existsFile(const char*);
};
#endif
