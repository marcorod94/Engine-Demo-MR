#ifndef MeshImporter_h
#define MeshImporter_h
#include "Importer.h"
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/DefaultLogger.hpp>
#define TEXTURE_PATH "Textures/"
#define DEFAULT_TEXTURE "checkers.jpg"

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;
class Mesh;
class MeshImporter : public Importer {
public:
	MeshImporter();
	~MeshImporter();
	bool Import(const char*, const char*, std::string*);
	bool Load(const char*, Mesh*);
private:
	std::string directory;
	void processNode(const aiNode*, const aiScene*);
	void processMesh(const aiMesh*);
	void processMaterials(const aiMaterial*);
	void loadMaterialTextures(const aiMaterial*, aiTextureType);
};

class AssimpLog : public Assimp::LogStream {
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
		//App->imgui->AddLog("Assimp INFO %s", message);
	}
};
#endif
