#ifndef ModuleModel_h
#define ModuleModel_h

#include "main/Globals.h"
#include "Module.h"
#include "ModuleImGui.h"
#include <assimp/scene.h>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/DefaultLogger.hpp>
#include <vector>
#include <string>

struct par_shapes_mesh_s;
class GameObject;
class Mesh;
class Material;
class ModuleModel: public Module {
public:
	ModuleModel() {}
	~ModuleModel() {}
	bool Init();
	bool CleanUp();
	bool LoadSphere(const char*, const math::float3&, const math::Quat&, float, unsigned, unsigned, const math::float4&, GameObject*);
	const void LoadModel(std::string&);
	void UpdateTexture(Texture&);

private:
	std::string directory;
	void processNode(const aiNode*, const aiScene*, GameObject*);
	void processMesh(const aiMesh*, GameObject*);
	void processMaterials(const aiMaterial*, GameObject*);
	void loadMaterialTextures(const aiMaterial*, aiTextureType, const char*, Material*);
	int existsFile(const char*) const;
	void GenerateMesh(const char*, const float3&, const Quat&, par_shapes_mesh_s*, GameObject*);

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
