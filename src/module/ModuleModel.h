#ifndef ModuleModel_h
#define ModuleModel_h

#include "Globals.h"
#include "Module.h"
#include "ModuleImGui.h"
#include <assimp/scene.h>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/DefaultLogger.hpp>
#include <vector>
#include <string>
class ModuleModel: public Module {
public:
	std::vector<Mesh> meshes;
	std::vector<Texture> modelTextures;
	aiVector3D scaling;
	aiVector3D position;
	aiVector3D rotation;
	float radius = 0.0F;
	AABB box;
	int totalPrimitives = 0;
	int totalVertex = 0;
	int totalMaterials = 0;

	ModuleModel() {}
	~ModuleModel() {}
	bool Init();
	bool CleanUp();
	void LoadModel(std::string&);
	void UpdateTexture(Texture&);

private:
	std::string directory;
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, const char* );
	int existsFile(const char*) const;
};

class AssimpLog : public Assimp::LogStream
{
public:
	// Constructor
	AssimpLog()
	{
		// empty
	}

	// Destructor
	~AssimpLog()
	{
		// empty
	}
	// Write womethink using your own functionality
	void write(const char* message)
	{
		App->imgui->AddLog("Assimp INFO %s", message);
	}
};
#endif
